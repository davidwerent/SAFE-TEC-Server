#pragma once
#include <string>
#include <fstream>
#include "json/json.h"
class ErrorLog
{
	std::string message;
	int status_code;
public:
	static Json::Value noError;
	static int GetCodeError();
	static int ERROR_CODE;
	static std::string ERROR_MESSAGE;
	ErrorLog();
	ErrorLog(int code);
	ErrorLog(std::string message, int code);
	ErrorLog(Json::Value json);
	Json::Value GetJson();
	Json::Value	GetJsonSuccess();
	
};
