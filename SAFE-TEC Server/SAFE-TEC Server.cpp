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
    int port = 1456;
    unsigned long latest_user_id = 10;
    
    std::string strjson = R"({ 
  "Authorize": {
    "login"     : "admin",
    "password"  : "qwert"
  },
  "SignIn": {
    "login"     : "spiguzov@safe-tec.ru",
    "password"  : "qwert123",
    "user_id"   : "1239009998"
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

    


    
    //RunCommand(strjson);

    uWS::App().ws<UserConnection>("/*", {
        .open = [&latest_user_id](auto* ws) {
            UserConnection* data = (UserConnection*)ws->getUserData();
            std::cout << "New user connected" << std::endl;
            ws->subscribe("broadcast");
            ws->subscribe("user#" + std::to_string(data->user_id));
            ws->publish("broadcast", "hello client!\n");

        },
        .message = [](auto* ws, std::string_view message, uWS::OpCode opCode) {
             UserConnection* data = (UserConnection*)ws->getUserData();
             std::string c{ message };
             std::cout << message << std::endl;
             RunCommand(data, c);
             //std::cout << data->login << " " << data->password << std::endl;
             std::string answer{ message };
             answer.append(" \nbut sorry i cant do it, yet ;(");
            
               
             ws->publish("broadcast", answer);
        },
        .close = [](auto* ws, int code, std::string_view message)
        {
           
        },
        }).listen(port, [&port](auto* token) {
            if (token)
                std::cout << "\nServer started successfully on port " << port << std::endl;
            else std::cout << "Server failed to start" << std::endl;
            }).run();
          
}