#include "UserConnections.h"

UserConnection::UserConnection()
{
	login = "";
	password = "";
	user_id = 0;
	role = 0;
	fullname = "";
	phone = "";
	position = "";
	company = "";
	photo = "";
	device_id = "";
}

UserConnection::UserConnection(const UserConnection& data)
{
	login = data.login;
	password = data.password;
	user_id = data.user_id;
}

UserConnection::UserConnection(std::string _login, std::string _password, int id)
{
	login = _login;
	password = _password;
	user_id = id;
}

std::string UserConnection::print()
{
	return login + " " + password + " " + std::to_string(user_id);
}

void UserConnection::setUserDataFromJSON(std::string stringofJSON)
{
	Json::Reader reader;
	Json::Value json;
	bool parsing = reader.parse(stringofJSON.c_str(), json);
	if (!parsing) {
		std::cout << "Failer to deserialize!\n";
	}
	login = json.get("login", "null").asString();
	password = json.get("password", "null").asString();
	user_id = json.get("user_id", 0).asLargestInt();
}

bool UserConnection::isValid()
{
	if (login.empty() && password.empty())
		return false;
	else return true;
	
}
