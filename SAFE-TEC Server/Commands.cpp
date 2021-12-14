#include "Commands.h"


void RunCommand(std::string strjson)
{
	Json::Value json;
	Json::Reader reader;
	bool parsing = reader.parse(strjson.c_str(), json);
	if (!parsing) {
		cout << "Failed to parse " << reader.getFormattedErrorMessages() << endl;
		return ;
	}

	std::vector<std::string> types_of_json;
	for (int i = 0; i < json.size(); i++) {
		//std::cout << i<<" - " << json.getMemberNames()[i] << "\n";
		types_of_json.push_back(json.getMemberNames()[i]);
	}
	for (int i = 0; i < types_of_json.size(); i++) {
		if (types_of_json[i] == "Authorize")
		{
			if (Auth(json["Authorize"].get("login", "nologin").asString(), json["Authorize"].get("password", "nopwd").asString())) {
				std::cout << "auth complete!\n";
			}
			else
			{
				std::cout << "auth failed!\n";
			}
		}
	}
	//std::cout << json.toStyledString();
//	std::cout << json["jamie"].getMemberNames()[0] << "=" << json["jamie"].get("followers", "none followers");

	

	/*switch (command)
	{
	case COMMAND::AUTH:
		std::cout << "AUTH COMPLETE!\n";
		break;
	default:
		std::cout << "Unknown JSON response\n";
		break;
	}*/
	std::cout << std::endl;
}
void RunCommand(UserConnection* user, std::string strjson)
{
	Json::Value json;
	Json::Reader reader;
	bool parsing = reader.parse(strjson.c_str(), json);
	if (!parsing) {
		cout << "Failed to parse " << reader.getFormattedErrorMessages() << endl;
		return;
	}

	std::vector<std::string> types_of_json;
	for (int i = 0; i < json.size(); i++) {
		//std::cout << i<<" - " << json.getMemberNames()[i] << "\n";
		types_of_json.push_back(json.getMemberNames()[i]);
	}
	std::string login, password;
	for (int i = 0; i < types_of_json.size(); i++) {
		if (types_of_json[i] == "Authorize")
		{
			login = json["Authorize"].get("login", "nologin").asString();
			password = json["Authorize"].get("password", "nopwd").asString();
			if (Auth(login,password)) {
				std::cout << "auth complete!\n";
				user->login = login;
				user->password = password;
			}
			else
			{
				std::cout << "auth failed!\n";
			}
		}
	}
}
bool Auth(std::string login, std::string password)
{
	Database s("localhost", "root", "Gudini2306%", "Userlist", 3306);
	UserConnection user;
	user = s.xLoadUserFromTable(login, password);
	if (user.isValid()) return true;
	else return false;
}
 
