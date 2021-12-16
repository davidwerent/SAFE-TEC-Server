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


int SignUp(Json::Value json);
int SignUp(Json::Value json, int &userid);
std::string CreateResponseSignUp(int code, int userid);

Zone GetZone(Json::Value json, int &zoneid);
std::string CreateResponseZone(Zone zone, int zoneid);