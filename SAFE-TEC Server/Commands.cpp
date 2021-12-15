#include "Commands.h"


std::string NewCommand(UserConnection* user, std::string strjson)
{
	COMMAND command;
	Json::Value json;
	Json::Reader reader;
	std::string login, password;
	bool parsing = reader.parse(strjson.c_str(), json);
	if (!parsing) {
		std::cout << "Failed to parse " << reader.getFormattedErrorMessages() << std::endl;
		COMMAND::error;
		return "";
	}
	std::vector<std::string> types_of_json;
	for (int i = 0; i < json.size(); i++) {
		std::cout << i << " - " << json.getMemberNames()[i] << "\n";
		types_of_json.push_back(json.getMemberNames()[i]);
	}
	for (int i = 0; i < types_of_json.size(); i++) {
		if (types_of_json[i] == "method")
		{
			auto method = json["method"].asString();
			if (method == "auth")
				command = COMMAND::auth;
			else if (method == "signUp")
				command = COMMAND::signUp;
			else if (method == "zone")
				command = COMMAND::zone;

			//use else-if next for new command (method from JSON)
			// add new method to enum class COMMAND and create func call in SWITCH(COMMAND) below
			//##################################################
			else {
				command = COMMAND::error;
				//придумать обработчик ошибок 
			}
		}
	}
	std::cout << json.toStyledString() << "\n";
	switch (command)
	{
	case COMMAND::error:
		
		break;
	case COMMAND::auth:
		if (newauth(json)) {
			std::cout << "auth complete!\n";
			return CreateResponseAuth();
		}
		else cout << "auth FAILED!\n";
		break;
	case COMMAND::signUp:
		if (SignUp(json)) {
			std::cout << "registration complete\n";
			return CreateResponseSignUp(1);
		}
		else if (SignUp(json)==2) {
			std::cout << "registration duplicated\n";
			return CreateResponseSignUp(0);
		}
		else {
			std::cout << "registration code error #" << SignUp(json) << std::endl;
			return CreateResponseSignUp(2);
		}
		break;
	case COMMAND::zone:

		break;
	default:
		return "error#1";
		break;
	}
}
bool newauth(Json::Value json)
{
	Database s("localhost", "root", "Gudini2306%", "userlist", 3306);
	UserConnection user;
	std::string login = json["data"]["email"].asString();
	std::string password = json["data"]["password"].asString();
	std::string deviceId = json["data"]["deviceId"].asString();
	std::cout << "will loaded from sql\n";
	user = s.xLoadUserFromTable(login, password);
	user.print();
	//std::cout << "user from DB=" << user.login << " and from JSON=" << login << std::endl;
	//std::cout << "pswd from DB=" << user.password << " and from JSON=" << password << std::endl;
	if ((user.login == login) && (user.password == password)) return true;
	else return false;
}
std::string CreateResponseAuth()
{
	Json::Value response;
	response["status"] = 1;
	response["message"] = "Log in success!\n";
	response["data"]["token"] = "tokenGU8ZHVBPWSSSJ98";
	return response.toStyledString();
}
int SignUp(Json::Value json)
{
	Database s("localhost", "root", "Gudini2306%", "userlist", 3306);
	UserConnection user;
	std::string login = json["data"]["email"].asString();
	std::string password = json["data"]["password"].asString();
	std::string fullname = json["data"]["fullname"].asString();
	std::string deviceId = json["data"]["deviceId"].asString();
	//std::cout << "will write to SQL\n";
	user = s.xInsertToTable(login, password, fullname, deviceId);
	if (!s.CheckEmail(login)) return 1;
	else if (s.GetQState()) return 2;
	else return 0;
}
std::string CreateResponseSignUp(int code)
{
	Json::Value response;
	response["status"] = 1;
	response["message"] = "its registation response";
	if (code==1) response["data"]["register_status"] = "Accepted";
	else if (code==0) response["data"]["register_status"] = "Duplicate";
	else response["data"]["register_status"] = "Declined";
	return response.toStyledString();
}
