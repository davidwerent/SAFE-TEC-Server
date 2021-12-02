#pragma once

#include <mysql.h>
#include <string>
#include <vector>
#include "UserConnections.h"

using namespace std;
struct ClientData
{
	string login;
	string password;
	int role; //
};
class Database
{
	MYSQL* conn;
	MYSQL_ROW row;
	MYSQL_RES* res;
	int qstate;
	string ActiveTableName;
	ClientData Client;
	bool isConnect;

	int SQLtoint(MYSQL_ROW m);
public:
	Database();
	Database(string IP, string login, string password, string dbname, int port);
	void printQuery(string q);
	void CreateTable(string name);
	void test(string name);
	void ChangeActiveTable(string q);
	int GetRowCount();
	int GetColCount();
	void GetTypeInfo();
	void LoadClientFromTable(string _login, string _password, int _role);
	UserConnection xLoadUserFromTable(string _login, string _password);
	bool isLogin();
	bool isConnected();
	int GetAccessLevel();
};
