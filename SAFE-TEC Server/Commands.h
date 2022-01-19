#pragma once
#include "Database.h"
#include "UserConnections.h"
#include <iostream>
#include <json/json.h>
#include <vector>
#include "Error.h"
#include "jwt/jwt.hpp"
#define DB_PASSWORD "Gudini2306%"
using namespace jwt::params;

enum class COMMAND
{
	error = 0,
	auth = 1,
	signUp = 2,
	zone = 3,
	system = 4,
	addzone = 5
};

std::string Error(int code);

std::string NewCommand(UserConnection* user, std::string strjson);
bool newauth(Json::Value json);
std::string CreateResponseAuth(bool auth);


int SignUp(Json::Value json);
int SignUp(Json::Value json, int &userid);
std::string CreateResponseSignUp(int code, int userid);

vector<Zone> GetZone(Json::Value json, int &zoneid);
std::string CreateResponseZone(vector<Zone> zone, int zoneid);

vector <System> GetSystem(Json::Value json, int& zoneid);
std::string CreateResponseSystem(vector<System> system, int zoneid);

void update(Json::Value& a, Json::Value& b);

int AddZone(Json::Value json);
