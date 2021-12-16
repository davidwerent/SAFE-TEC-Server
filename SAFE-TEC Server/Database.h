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
	int user_id;
};
struct Zone
{
	int zone_id;
	string name; 
	string description;
	string address; 
	string phone;
	string photo;
	string owner;
	string managerST;
};
class Database
{
	MYSQL* conn;
	MYSQL_ROW row;
	MYSQL_RES* res;
	int qstate;
	string ActiveTableName;
	
	bool isConnect;
	int error;

	int SQLtoint(MYSQL_ROW m);
public:
	ClientData Client;
	Zone zone;
	Database();
	Database(string IP, string login, string password, string dbname, int port);
	Database(string IP, string login, string password, string dbname, string table_name, int port);
	void printQuery(string q);
	void CreateTable(string name);
	void test(string name);
	void ChangeActiveTable(string q);
	int GetRowCount();
	int GetColCount();
	int GetQState() { return qstate; };
	int GetError() { return error; };
	void GetTypeInfo();
	void LoadClientFromTable(string _login, string _password, int _role);
	UserConnection xLoadUserFromTable(string _login, string _password);
	Zone xLoadZoneFromTable(string owner);
	UserConnection xInsertToTable(string login, string password, string fullname, string deviceID);
	int xGetUserId(std::string login);
	bool isLogin();
	bool isConnected();
	bool CheckEmail(string _login);
	int GetAccessLevel();
};
