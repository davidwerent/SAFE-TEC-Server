#pragma once
#include "Database.h"
#include "UserConnections.h"
#include <iostream>
#include <json/json.h>
#include <vector>

enum class COMMAND
{
	AUTH = 1,
	GETZONE = 2
};

void RunCommand(std::string strjson);
void RunCommand(UserConnection* user, std::string strjson);

bool Auth(std::string login, std::string password);