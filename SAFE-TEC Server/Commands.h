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
	signUp = 2,
	zone = 3
};


std::string NewCommand(UserConnection* user, std::string strjson);
bool newauth(Json::Value json);

std::string CreateResponseAuth();
std::string CreateResponseSignUp(int code);
int SignUp(Json::Value json);



