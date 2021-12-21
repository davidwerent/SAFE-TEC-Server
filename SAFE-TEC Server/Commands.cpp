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
			try {
				auto method = json["method"].asString();
				if (method == "auth")
					command = COMMAND::auth;
				else if (method == "signUp")
					command = COMMAND::signUp;
				else if (method == "zones")
					command = COMMAND::zone;
				else if (method == "systems")
					command = COMMAND::system;


				//use else-if next for new command (method from JSON)
				// add new method to enum class COMMAND and create func call in SWITCH(COMMAND) below
				//##################################################
				else throw 2;
			}
			catch (int) {
				command = COMMAND::error;
			}
		}
	}
	std::cout << json.toStyledString() << "\n";
	int user_id_response = 0;
	int zone_id_response = 0;
	vector<Zone> z;
	vector<System> s;
	switch (command)
	{
	case COMMAND::error:
		
		break;
	case COMMAND::auth:
		if (newauth(json)) {
			std::cout << "auth complete!\n";
			return CreateResponseAuth(1);
		}
		else {
			cout << "auth FAILED!\n";
			return CreateResponseAuth(0);
		}
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
	case COMMAND::system:
		std::cout << "system method requested\n";

		s = GetSystem(json, zone_id_response); //не уверен что тут должна быть та же переменная а не отдельная или вызов из json
		return CreateResponseSystem(s, 1);
		break;
	default:
		return "error#1";
		break;
	}
}
bool newauth(Json::Value json)
{
	ErrorLog::noError = json;
	Database s("localhost", "root", DB_PASSWORD, "userlist", 3306);
	UserConnection user;
	std::string login	 = json["data"]["email"].asString();
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
std::string CreateResponseAuth(bool auth)
{
	ErrorLog log(ErrorLog::noError);
	/*Json::Value response;
	if (auth) {
		response["status"] = 1;
		response["message"] = "Log in success!\n";
	}
	else {
		response["status"] = 0;
		response["message"] = "Auth failed\n";
	}
	response["data"]["token"] = "tokenGU8ZHVBPWSSSJ98";
	return response.toStyledString();*/
	Json::Value response;
	Json::Value temp = log.GetJson();
	if (auth) {
		response["data"]["token"] = "tokenGU8ZHBHSJAKSDJ";
		update(response, temp);
		std::cout << response.toStyledString() << std::endl;
		return response.toStyledString();
	}
	else
	{
		log.ERROR_CODE = 2;
		log.ERROR_MESSAGE = "auth failed!";
		temp = log.GetJson();
		response["data"]["token"] = "tokenGU8ZHBHSJAKSDJ";
		update(response, temp);
		std::cout << response.toStyledString() << std::endl;
		return response.toStyledString();
	}
}
int SignUp(Json::Value json)
{
	ErrorLog::noError = json;
	Database s("localhost", "root", DB_PASSWORD, "userlist", 3306);
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
	ErrorLog::noError = json;
	Database s("localhost", "root", DB_PASSWORD, "userlist", 3306);
	UserConnection user;
	std::string login	 = json["data"]["email"].asString();
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
	ErrorLog log(ErrorLog::noError);
	Json::Value jlog = log.GetJson();
	

	Json::Value response;
	//response["status"] = code;
	//response["message"] = "its registation response";
	if (code == 1) {
		jlog = log.GetJsonSuccess();
		response["data"]["register_status"] = "Accepted";
		update(response, jlog);
		std::cout << response.toStyledString() << std::endl;
		return response.toStyledString();
	}
	else if (code == 0) {
		log.ERROR_CODE = 3;
		log.ERROR_MESSAGE = "This email is already exist! Registration Failed!"; 
		jlog = log.GetJson();
		response["data"]["register_status"] = "Duplicate";
		update(response, jlog);
		std::cout << response.toStyledString() << std::endl;
		return response.toStyledString();
	}
	else {
		log.ERROR_CODE = 4;
		log.ERROR_MESSAGE = "Registration failed. Ask an administrator";
		jlog = log.GetJson();
		response["data"]["register_status"] = "Declined";
		update(response, jlog);
		std::cout << response.toStyledString() << std::endl;
		return response.toStyledString();

	}
	response["data"]["userId"] = userid;
	return response.toStyledString();
}

vector<Zone> GetZone(Json::Value json, int &zoneid)
{
	ErrorLog::noError = json;
	Database s("localhost", "root", DB_PASSWORD, "zone", "zones", 3306);
	vector<Zone> zone = s.xLoadZoneFromTable(json["data"]["owner"].asString());
	return zone;
}

std::string CreateResponseZone(vector<Zone> zone, int zoneid)
{
	ErrorLog log(ErrorLog::noError);
	
	Json::Value response;
	Json::Value zres; //zone response
	//response["status"] = 1;
	//response["message"] = "list of zones by email";
	
	for (int i = 0; i < zone.size(); i++)
	{
		zres["zoneID"]		= zone[i].zone_id;
		zres["name"]		= zone[i].name;
		zres["description"] = zone[i].description;
		zres["address"]		= zone[i].address;
		zres["phone"]		= zone[i].phone;
		zres["photo"]		= zone[i].photo;
		zres["owner"]		= zone[i].owner;
		zres["managerST"]	= zone[i].managerST;
		response["data"]["zones"].append(zres);
	}
	Json::Value jlog = log.GetJson();
	update(response, jlog);
	//std::cout << "TOTAL JSON BELOW=========\n";
	//std::cout << response.toStyledString();
	return response.toStyledString();
}

vector<System> GetSystem(Json::Value json, int& zoneid)
{
	Database s("localhost", "root", DB_PASSWORD, "systems", "systemstable", 3306);
	vector <System> system = s.xLoadSystemFromTable(json["data"]["zoneID"].asInt());

	return system;
}

std::string CreateResponseSystem(vector<System> system, int zoneid)
{
	Json::Value response;
	Json::Value system_response;
	response["status"] = 1;
	response["message"] = "list of systems by zoneID";
	for (int i = 0; i < system.size(); i++)
	{
		system_response["systemID"]			  = system[i].system_id;
		system_response["name"]				  = system[i].name;
		system_response["serialNumber"]		  = system[i].serialNumber;
		system_response["height"]			  = system[i].height;
		system_response["length"]			  = system[i].length;
		system_response["access"]			  = system[i].access;
		system_response["startOperationDate"] = system[i].startOperationDate;
		system_response["lastSeenDate"]		  = system[i].lastSeenDate;
		system_response["description"]		  = system[i].description;
		system_response["zone_id"]			  = system[i].zone_id;
		response["data"]["systems"].append(system_response);
	}

	std::cout << "FULL JSON BELOW=============\n";
	std::cout << response.toStyledString() << endl;
	return response.toStyledString();
}

void update(Json::Value& a, Json::Value& b) {
	

	for (const auto& key : b.getMemberNames()) {
		if (a[key].type() == Json::objectValue && b[key].type() == Json::objectValue) {
			update(a[key], b[key]);
		}
		else {
			a[key] = b[key];
		}
	}
}
