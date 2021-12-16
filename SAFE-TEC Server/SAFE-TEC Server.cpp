#include <iostream>
#include <uwebsockets/App.h>
#include <string>
#include <json/json.h>
#include "UserConnections.h"
#include "Database.h"
#include "Commands.h"
//#include "C:\\cpp\\json-develop\\include\\nlohmann\\json.hpp"
/*
ws = new WebSocket("ws://localhost:8888/");
ws.onmessage=({data})=>console.log(data);
ws.send("somecommand");
*/
int main()
{
    int port = 1457;
    unsigned long latest_user_id = 0;
    std::string strjson = R"({ 
  "Authorize": {
    "login"     : "admin",
    "password"  : "qwert",
    "device_id" : "HD023-GHQPOG-ASDHJBKSA-ASDQ29311-912"
  },
  "SignIn": {
    "login"     : "spiguzov@safe-tec.ru",
    "password"  : "qwert123",
    "user_id"   : "1239009998",
    "device_id" : "HD023-GHQPOG-ASDHJBKSA-ASDQ29311-912"
  },
  "Profile"    : {
    "email"    : "spiguzov@safe-tec.ru",
    "role"     : 4,
    "user_id"  : "10000056236",
    "fullname" : "Semen Piguzov",
    "phone"    : 79339990895,
    "position" : "director",
    "company"  : "PAO ROSNEFT",
    "photo"    : "http://safe-tec.ru/photo/123.png"
  },
  "Zone" : {
    "name"        : "Post ASN",
    "description" : "some multiline text",
    "address"     : "Moscow, Lucky street 19",
    "phone"       : 74952520005,
    "photo"       : "somelink",
    "owner"       : "user_ID from Profile",
    "managerST"   : "email of manager ST",
    "zoneID"      : "unique zone ID",
    "system"      : [ 
    {
      "name" : "SAFETROLL",
      "description" : "ultra wide multiline text",
      "photo" : "link",
      "serialnumber" : "10000293NS",
      "length" : 120,
      "heigthaccess" : true,
      "date_start" : "19.01.2020",
      "date_inspection" : "20.01.2021",
      "systemID" : "unique system ID"
    }
  ]
    
  },
  "order" : {
    "name"  : "JamieMantisShrimp",
    "description"  : "Pacific Ocean",
    "photo"    : true,
    "isSolve" : false,
    "zoneID"  : "1232712",
    "systemID" : "123"
  }
})";
    Json::Value root;
    Json::Reader reader;

    

    std::string strjson2 = R"({
  "method": "auth",
  "data": { 
    "email": "alex@gmail.com",
    "password": "qweASD123",
    "deviceId": "5"
  }
}
)";
    //cout<<NewCommand(NULL, strjson2);
    //std::cout << "RESPONSE:\n" << CreateResponseAuth() << std::endl;
    //ws.send("{  \"method\": \"auth\",  \"data\": {    \"email\": \"alex@gmail.com\",    \"password\": \"qweASD123\",    \"deviceId\": \"HASJ2-009A-2933-12GASJJWQP{\"  }}");
    //ws.send("{  \"method\": \"signUp\",  \"data\": {    \"email\": \"newuser@gmail.com\",    \"password\": \"qwerty\",    \"fullname\": \"Ivan Ivanov\",    \"deviceId\": \"00000-009A-0000-12GASJJWQP{\"  }}");

    uWS::App().ws<UserConnection>("/*", {

        .compression = uWS::DISABLED,
        .maxPayloadLength = 16 * 1024 * 1024,
        .idleTimeout = 600,
        .maxBackpressure = 16 * 1024 * 1024,
        .closeOnBackpressureLimit = false,
        .resetIdleTimeoutOnSend = true,
        .sendPingsAutomatically = false,
        .upgrade = nullptr,
        .open = [&latest_user_id](auto* ws) {
            UserConnection* data = (UserConnection*)ws->getUserData();
            data->user_id=latest_user_id++;
            std::cout << "New user connected #" <<data->user_id<< std::endl;
            
            ws->subscribe("broadcast");
            ws->subscribe("user#" + std::to_string(data->user_id));
            ws->publish("broadcast", "hello client!\n");

        },
        .message = [](auto* ws, std::string_view message, uWS::OpCode opCode) {
             UserConnection* data = (UserConnection*)ws->getUserData();
             std::string c{ message };
             std::cout << message << std::endl;
             //RunCommand(data, c);
             std::string answer = NewCommand(data, c);
             
             //std::cout << data->login << " " << data->password << std::endl;
             ws->send(answer, uWS::OpCode::TEXT);
            
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
            if (token)
                std::cout << "Server started successfully on port " << port << std::endl;
            else std::cout << "Server failed to start" << std::endl;
            }).run();
          
}