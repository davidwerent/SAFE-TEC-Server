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
			else if (method == "zones")
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
	int user_id_response = 0;
	int zone_id_response = 0;
	Zone z;
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
		if (SignUp(json,user_id_response)==0) {
			std::cout << "registration complete\n";
			return CreateResponseSignUp(1,user_id_response);
		}
		else if (SignUp(json)==2) {
			std::cout << "registration duplicated\n";
			return CreateResponseSignUp(0,0);
		}
		else {
			std::cout << "registration code error #" << SignUp(json) << std::endl;
			return CreateResponseSignUp(2,0);
		}
		break;
	case COMMAND::zone:
		std::cout << "zone method requested\n";
		
		z=GetZone(json, zone_id_response);
		return CreateResponseZone(z, 1);
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
	std::string login	 = json["data"]["email"].asString();
	std::string password = json["data"]["password"].asString();
	std::string fullname = json["data"]["fullname"].asString();
	std::string deviceId = json["data"]["deviceId"].asString();
	//std::cout << "will write to SQL\n";
	user = s.xInsertToTable(login, password, fullname, deviceId);
	return s.GetError();
	
}
int SignUp(Json::Value json, int &userid)
{
	Database s("localhost", "root", "Gudini2306%", "userlist", 3306);
	UserConnection user;
	std::string login = json["data"]["email"].asString();
	std::string password = json["data"]["password"].asString();
	std::string fullname = json["data"]["fullname"].asString();
	std::string deviceId = json["data"]["deviceId"].asString();
	//std::cout << "will write to SQL\n";
	user = s.xInsertToTable(login, password, fullname, deviceId);
	s.LoadClientFromTable(login, "",0);
	userid = s.Client.user_id;
	return s.GetError();

}
std::string CreateResponseSignUp(int code, int userid)
{	
	
	Json::Value response;
	response["status"] = code;
	response["message"] = "its registation response";
	if (code==1)		response["data"]["register_status"] = "Accepted";
	else if (code==0)	response["data"]["register_status"] = "Duplicate";
	else				response["data"]["register_status"] = "Declined";
	response["data"]["userId"] = userid;
	return response.toStyledString();
}

Zone GetZone(Json::Value json, int &zoneid)
{
	
	Database s("localhost", "root", "Gudini2306%", "zone", "zones", 3306);
	Zone zone = s.xLoadZoneFromTable(json["data"]["owner"].asString());
	


	return zone;
}

std::string CreateResponseZone(Zone zone, int zoneid)
{
	Json::Value response;
	
	response["status"] = 1;
	response["message"] = "строка из с++";
	response["data"]["name"] = zone.name;
	response["data"]["description"] = zone.description;
	response["data"]["address"] = zone.address;
	response["data"]["phone"] = zone.phone;
	response["data"]["photo"] = zone.photo;
	response["data"]["owner"] = zone.owner;
	response["data"]["managerST"] = zone.managerST;
	response["data"]["zoneID"] = zone.zone_id;
			
		
	
	
	
	

	return response.toStyledString();
}


