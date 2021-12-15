#pragma once
#include "Database.h"
#include "UserConnections.h"
#include <iostream>
#include <json/json.h>
#include <vector>

enum class COMMAND
{
	error = 0,
	auth = 1,
	signUp = 2
};


std::string NewCommand(UserConnection* user, std::string strjson);
bool newauth(Json::Value json);
bool Auth(std::string login, std::string password);
std::string CreateResponseAuth();




