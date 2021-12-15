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
			if (json["method"].asString() == "auth")
				command = COMMAND::auth;
			else if (json["method"].asString() == "signUp")
				command = COMMAND::signUp;
			//use else-if next for new command (method from JSON)
			// add new method to enum class COMMAND and create func call in SWITCH(COMMAND)
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
			cout << "auth complete!\n";
			return CreateResponseAuth();
		}
		else cout << "auth FAILED!\n";
		break;
	case COMMAND::signUp:
		
		break;
	default:
		return "error#1";
		break;
	}

	
}
	
bool newauth(Json::Value json)
{
	Database s("localhost", "root", "admin", "userlist", 3306);
	UserConnection user;
	std::string login = json["data"]["email"].asString();
	std::string password = json["data"]["password"].asString();
	std::string deviceId = json["data"]["deviceId"].asString();
	cout << "will loaded from sql\n";
	user = s.xLoadUserFromTable(login, password);
	user.print();
	//std::cout << "user from DB=" << user.login << " and from JSON=" << login << std::endl;
	//std::cout << "pswd from DB=" << user.password << " and from JSON=" << password << std::endl;
	if ((user.login == login) && (user.password == password)) return true;
	else return false;
}


bool Auth(std::string login, std::string password)
{
	/* {
		"status": "status_code",
			"messages" : "some_inf_string",
			"data" : {
			"token": "token_string"
		}
	}*/
}


std::string CreateResponseAuth()
{
	Json::Value response;
	response["status"] = "1";
	response["message"] = "Loging complete!\n";
	response["data"]["token"] = "tokenGU8ZHVBPWSSSJ98";


	return response.toStyledString();
}
 
