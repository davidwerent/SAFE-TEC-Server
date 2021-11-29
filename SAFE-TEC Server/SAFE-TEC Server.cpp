#include <iostream>
#include <uwebsockets/App.h>
#include <string>


#include <json/json.h>
#include "UserConnections.h"


//#include "C:\\cpp\\json-develop\\include\\nlohmann\\json.hpp"


/*
ws = new WebSocket("ws://localhost:8888/");
ws.onmessage=({data})=>console.log(data);
ws.send("somecommand");
*/
std::string JSONtoUserConnection(std::string stringOfJson)
{
    Json::Reader reader;
    Json::Value json;
    bool parsing = reader.parse(stringOfJson.c_str(), json);
    if (!parsing) {
        return "Failed to deserialize!\n";
    }
    return json.get("login", "null").asString();

};

int main()
{
    int port = 8888;
    unsigned long latest_user_id = 10;
    
    std::string strjson = R"({"login":"admin"})";

    Json::Value root;
    Json::Reader reader;

    bool parsing = reader.parse(strjson.c_str(), root);
    if (!parsing)
    {
        std::cout << "Failed to parse" << reader.getFormattedErrorMessages();
        return 0;
    }
    std::cout <<"login= "<< root.get("login", "a default value if not exist\n").asString() << std::endl;
    
    std::cout << std::endl;

    uWS::App().ws<UserConnection>("/*", {
        .open = [&latest_user_id](auto* ws) {
            UserConnection* data = (UserConnection*)ws->getUserData();
                       

        },
        .message = [](auto* ws, std::string_view message, uWS::OpCode opCode) {
          
        },
        .close = [](auto* ws, int code, std::string_view message)
        {
           
        },
        }).listen(port, [&port](auto* token) {
            if (token)
                std::cout << "Server started successfully on port " << port << std::endl;
            else std::cout << "Server failed to start" << std::endl;
            }).run();
          
}