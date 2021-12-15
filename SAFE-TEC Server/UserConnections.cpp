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

void UserConnection::print()
{
	std::cout << "\nuser_id\t\t=" << user_id;
	std::cout << "\nemail\t\t=" << login;
	std::cout << "\npassword\t=" << password;
	std::cout << "\nrole\t\t=" << role;
	std::cout << "\nfullname\t=" << fullname;
	std::cout << "\nphone\t\t=" << phone;
	std::cout << "\nposition\t=" << position;
	std::cout << "\ncompany\t\t=" << company;
	std::cout << "\nphoto\t\t=" << photo;
	std::cout << "\ndevice_id\t=" << device_id;
	std::cout << std::endl;
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
	if (!login.empty() && !password.empty())
		return false;
	else return true;
	
}
