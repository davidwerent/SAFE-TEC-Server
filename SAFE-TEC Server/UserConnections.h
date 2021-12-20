#pragma once
#include <string>
#include <iostream>
#include <json/json.h>
class UserConnection {
public:
    std::string login;
    std::string password;
    unsigned long user_id;

    int role;
    std::string fullname;
    std::string phone;
    std::string position;
    std::string company;
    std::string photo;
    std::string device_id;


    UserConnection();
    UserConnection(const UserConnection& data);
    UserConnection(std::string _login, std::string _password, int id);
    void print();
    void setUserDataFromJSON(std::string stringofJSON);
    bool isValid();


};
