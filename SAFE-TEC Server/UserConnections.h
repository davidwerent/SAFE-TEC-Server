#pragma once
#include <string>
#include <iostream>
#include <json/json.h>
class UserConnection {
public:
    std::string login;
    std::string password;
    unsigned long user_id;
    UserConnection();
    UserConnection(const UserConnection& data);
    UserConnection(std::string _login, std::string _password, int id);
    std::string print();
    void setUserDataFromJSON(std::string stringofJSON);
    bool isValid();


};
void RunCommand(std::string command);