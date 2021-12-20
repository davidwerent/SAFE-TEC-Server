#include "Error.h"

int ErrorLog::GetCodeError()
{
	return 0;
}

ErrorLog::ErrorLog()
{
	message = "no error";
	status_code = 1;
}

ErrorLog::ErrorLog(int code)
{
	message = noError["message"].asString();
	status_code = noError["status_code"].asInt();
}

ErrorLog::ErrorLog(std::string message, int code)
{
	this->message = message;
	this->status_code = code;
}

ErrorLog::ErrorLog(Json::Value json)
{
	std::vector<std::string> types_of_json;
	for (int i = 0; i < json.size(); i++)
		types_of_json.push_back(json.getMemberNames()[i]);
	for (int i = 0; i < types_of_json.size(); i++)
	{
		if (types_of_json[i] == "method")
		{
			auto method = json["method"].asString();
			if (method == "auth") {
				this->message = "Log in success!";
				this->status_code = 1;
			}
			else if (method == "signUp") {
				this->message = "Registration response";
				this->status_code = 1;
			}
			else if (method == "zones") {
				this->message = "list of zones by owner email";
				this->status_code = 1;
			}
			else if (method == "systems") {
				this->message = "list of system by zoneID";
				this->status_code = 1;
			}
			else {
				this->message = "unknown message";
				this->status_code = 123;
			}
		}
	}
}

Json::Value ErrorLog::GetJson()
{
	Json::Value res;
	res["status_code"] = ERROR_CODE;
	res["message"] = ERROR_MESSAGE;
	return res;
}

Json::Value ErrorLog::GetJsonSuccess()
{
	Json::Value res;
	res["message"] = message;
	res["status_code"] = status_code;
	return res;
}

Json::Value ErrorLog::noError = Json::Value();
int ErrorLog::ERROR_CODE = 1;
std::string ErrorLog::ERROR_MESSAGE = "no error";

void example_error_in_func()
{
	ErrorLog log(ErrorLog::noError);
	log.GetJson();
	log.ERROR_CODE = 123;
	log.ERROR_MESSAGE = "some message of error";
	
}
