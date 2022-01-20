#include <iostream>
#include <uwebsockets/App.h>
#include <string>
#include <string_view>
#include <json/json.h>
#include "UserConnections.h"
#include "Database.h"
#include "Error.h"
#include "Commands.h"
#include <windows.h>
#include <tchar.h>
#include "jwt/jwt.hpp"
#include "Interface.h"
#include <map>
//#include "C:\\cpp\\json-develop\\include\\nlohmann\\json.hpp"
/*
ws = new WebSocket("ws://localhost:8888/");
ws.onmessage=({data})=>console.log(data);
ws.send("somecommand");
*/

using namespace jwt::params;
void SetLucidaFont()
{
    HANDLE StdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX info;
    memset(&info, 0, sizeof(CONSOLE_FONT_INFOEX));
    info.cbSize = sizeof(CONSOLE_FONT_INFOEX);              // prevents err=87 below
    if (GetCurrentConsoleFontEx(StdOut, FALSE, &info))
    {
        info.FontFamily = FF_DONTCARE;
        info.dwFontSize.X = 0;  // leave X as zero
        info.dwFontSize.Y = 14;
        info.FontWeight = 400;
        _tcscpy_s(info.FaceName, L"Lucida Console");
        if (SetCurrentConsoleFontEx(StdOut, FALSE, &info))
        {
        }
    }
};
int main()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    setlocale(LC_ALL, "Russian");
    int port = 1456;
    
    unsigned long latest_user_id = 1;
 
    Json::Value root;
    Json::Reader reader;
    
    std::string_view strjson2 = R"({
  "method": "systems",
  "data": { 
    "email": "alex@gmail.com",
    "password": "qweASD123",
    "deviceId": "5"
  }
}
)";
    map <string, string> m;
    m["method"] = "jwt method";
    auto key = "secret";
    jwt::jwt_object obj{ algorithm("HS256"),payload(m),secret(key) };
    auto enc_str = obj.signature(); //encode
    std::cout << enc_str << std::endl;
    //decode
    auto dec_obj = jwt::decode(enc_str, algorithms({ "HS256" }), secret(key));
    std::cout << dec_obj.header() << " - is header\n";
    std::cout << dec_obj.payload() << " - is payload\n";


    

   /* bool parsing = reader.parse(strjson2.c_str(), root);
    if (!parsing) {
        std::cout << "Failed to parse " << reader.getFormattedErrorMessages() << std::endl;
    }
    ErrorLog::jsonerr = root;
    ErrorLog log(ErrorLog::jsonerr);
    std::cout << log.GetJson().toStyledString() << std::endl;*/
    


    // Java команды для клиента из браузера
    //ws.send("{  \"method\": \"auth\",  \"data\": {    \"email\": \"alex@gmail.com\",    \"password\": \"qweASD123\",    \"deviceId\": \"HASJ2-009A-2933-12GASJJWQP{\"  }}");
    //ws.send("{  \"method\": \"signUp\",  \"data\": {    \"email\": \"newuser@gmail.com\",    \"password\": \"qwerty\",    \"fullname\": \"Ivan Ivanov\",    \"deviceId\": \"00000-009A-0000-12GASJJWQP{\"  }}");
    //ws.send("{  \"method\": \"zones\", \"token\":\"tokenASISK2810AJSDJKGASLDK\",  \"data\": {    \"owner\": \"alex@gmail.com\"}}");
    //ws.send("{  \"method\": \"systems\", \"token\":\"tokenASISK2810AJSDJKGASLDK\",  \"data\": {    \"zoneID\": 1 }}");
    //ws.send("eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJtZXRob2QiOiJqd3QgbWV0aG9kIn0.2hTaiqofhGCoTtXevv70hEXsPajjLkz-76MDyJsKAI0");
    //ws.send("{  \"method\": \"addZone\",  \"data\": {    \"name\": \"new zone #1\",    \"description\": \"some multiline text will here lorem ipsum dolor si umet\", \"phone\": \"+79010001234\",  \"owner\": \"alex@gmail.com\",  \"managerST\": \"it@safe-tec.ru\"  }}");

    uWS::SSLApp(
        {
            .key_file_name = "C:\\cpp\\key.pem",
            .cert_file_name = "C:\\cpp\\certificate.pem",
            .passphrase = "1234"
        }
    ).ws<UserConnection>("/*", {

        .compression = uWS::DISABLED,
        .maxPayloadLength = 16 * 1024 * 1024,
        .idleTimeout = 600,
        .maxBackpressure = 16 * 1024 * 1024,
        .closeOnBackpressureLimit = false,
        .resetIdleTimeoutOnSend = true,
        .sendPingsAutomatically = false,
        .upgrade = nullptr,
        .open = [&latest_user_id, &hConsole](auto* ws) {
            UserConnection* data = (UserConnection*)ws->getUserData();
            data->user_id=latest_user_id++;
            GREEN; std::cout << "New user connected #" << data->user_id << std::endl; WHITE;
           

        },
        .message = [](auto* ws, std::string_view message, uWS::OpCode opCode) {
             UserConnection* data = (UserConnection*)ws->getUserData();
             std::string c{ message };
             //std::cout << message << std::endl;
             //RunCommand(data, c);
             std::string answer = NewCommand(data, c);
             //std::cout << data->login << "\n";
             //std::cout << data->login << " " << data->password << std::endl;
             ws->send(answer, uWS::OpCode::BINARY);
            
             auto user_channel = "user#" + std::to_string(data->user_id);
             //ws->publish("broadcast", answer);
        },
        .drain = [](auto* ws) {
        
        },
        .ping= [](auto* ws, std::string_view pingmes) {

        },
        .pong = [](auto* ws, std::string_view pongmes) {

        },
        .close = [](auto* ws, int code, std::string_view message)
        {
            UserConnection* data = (UserConnection*)ws->getUserData();
            std::cout << "User #" << data->user_id << " has disconnected!\n";
        },
        }).listen(port, [&port](auto* token) {
            if (token) {
               // std::cout << "\n\n\n\n\n\n\n\n\n\n";
                std::cout << "Server started successfully on port " << port << std::endl;
            }
            else std::cout << "Server failed to start" << std::endl;
            }).run();
          
}