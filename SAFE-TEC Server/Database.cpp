#include "Database.h"
#include <string>
#include <iostream>
#include <stdlib.h>

Database::Database()
{
	Client.login = "";
	Client.password = "";
	Client.role = 0;
	Client.user_id = 0;
	conn = mysql_init(0);
	qstate = 0;
	ActiveTableName = " ";
	isConnect = false;
}
Database::Database(string IP, string login, string password, string dbname, int port)
{
	Client.login = "";
	Client.password = "";
	Client.role = 0;
	ActiveTableName = "user";
	qstate = 0;
	error = 0;
	conn = mysql_init(0);
	conn = mysql_real_connect(conn, IP.c_str(), login.c_str(),
		password.c_str(), dbname.c_str(), port, NULL, 0);
	if (conn) {
		//cout << "Connection with SQL server is good!\n"; 
		isConnect = true;
	}
	else {
		cout << "Failed CLASS connection!\n"; isConnect = false;
	}
}
Database::Database(string IP, string login, string password, string dbname, string table_name, int port)
{
	Client.login = "";
	Client.password = "";
	Client.role = 0;
	ActiveTableName = table_name;
	qstate = 0;
	error = 0;
	conn = mysql_init(0);
	conn = mysql_real_connect(conn, IP.c_str(), login.c_str(),
		password.c_str(), dbname.c_str(), port, NULL, 0);
	if (conn) {
		//cout << "Connection with SQL server is good!\n"; 
		isConnect = true;
	}
	else {
		cout << "Failed CLASS connection!\n"; isConnect = false;
	}
}
void Database::printQuery(string q)
{
	qstate = mysql_query(conn, q.c_str());
	if (!qstate)
	{
		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res))
		{
			cout << "ID: " << row[0] << ", Name: " << row[1] << ", Value: " << row[2] << endl;
		}
	}
	else cout << "Query failed: " << mysql_error(conn) << endl;
}
void Database::CreateTable(string name)
{
	string q = "CREATE TABLE ";
	q.append(name);
	qstate = mysql_query(conn, q.c_str());
}
void Database::test(string name)
{
	int count = 0;
	qstate = mysql_query(conn, name.c_str());
	if (!qstate)
	{
		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res))
		{
			cout << "ID: " << row[0] << ", Name: " << row[1] << "\t, Value: " << row[2] << endl;
			count++;
		}
		cout << "number of row is " << count << endl;
	}
	else cout << "Query failed: " << mysql_error(conn) << endl;
}
int Database::GetRowCount()
{
	int count = 0;
	string temp = "SELECT * FROM ";
	temp.append(ActiveTableName);
	qstate = mysql_query(conn, temp.c_str());
	if (!qstate)
	{
		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res))
		{
			//cout << "ID: " << row[0] << ", Name: " << row[1] << ", Value: " << row[2] << endl;
			count++;
		}
		return count;
	}
	else return 0;
}
int Database::GetColCount()
{
	int count = 0;
	string temp = "SELECT * FROM ";
	temp.append(ActiveTableName);
	qstate = mysql_query(conn, temp.c_str());
	if (!qstate)
	{
		res = mysql_store_result(conn);
		count = mysql_num_fields(res);
		return count;
	}
	else return 0;
}
void Database::GetTypeInfo()
{
	qstate = mysql_query(conn, "DESCRIBE user");
	if (!qstate)
	{
		res = mysql_store_result(conn);
		cout << "Field \tType\n";
		while (row = mysql_fetch_row(res))
		{
			//cout << "ID: " << row[0] << ", Name: " << row[1] << ", Value: " << row[2] << endl;
			cout << row[0] << "\t" << row[1] << endl;
		}
	}
	else cout << "Query failed: " << mysql_error(conn) << endl;
}
int Database::SQLtoint(MYSQL_ROW m)
{
	return 1;
}
void Database::LoadClientFromTable(string _login, string _password, int _role) //������� � � �������� ������������� ������� � �������.
{
	string s;
	qstate = mysql_query(conn, "SELECT * FROM user");
	if (!qstate)
	{
		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res))
		{
			//cout << "LOGIN " << row[0] << "\t\t, PWD: " << row[1] << "\t, Role: " << row[2] << endl;
			if ((row[1] == _login))
			{
				Client.login = row[1];
				s = row[0];
				Client.user_id = atoi(s.c_str());
			}

		}
	}
	else cout << "Query failed: " << mysql_error(conn) << endl;
}
UserConnection Database::xLoadUserFromTable(string _login, string _password)
{
	string s;
	UserConnection user;
	qstate = mysql_query(conn, "SELECT * FROM user");
	if (!qstate)
	{
		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res))
		{
			//cout << "LOGIN " << row[1] << "\t\t, PWD: " << row[2] << "\t, Role: " << row[3] << endl;
			if ((row[1] == _login)){
				user.user_id	= atoi(row[0]);
				user.login		= row[1];
				user.password	= row[2]; //потом переделать на хеш от пароля
				user.role		= atoi(row[3]);
				user.fullname	= row[4];
				user.phone		= row[5];
				user.position	= row[6];
				user.company	= row[7];
				user.photo		= row[8];
				user.device_id	= row[9];

			}
		}
	}
	else cout << "Query failed: " << mysql_error(conn) << endl;
	return user;
}
Zone Database::xLoadZoneFromTable(string owner)
{
	Zone zone;
	qstate = mysql_query(conn, "SELECT * FROM zones");
	if (!qstate) {
		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res))
		{
			zone.name = row[1];
			zone.description = row[2];
			zone.address = row[3];
		}
	}


	return zone;
}
UserConnection Database::xInsertToTable(string login, string password, string fullname, string deviceID)
{
	UserConnection user;

	
	if (!CheckEmail(login)) {
		cout << "no login found\n";
		error = 0;
	}
	else {//cout << "this login is already exist!!!!\n";
		error = 2;
		return user;
	} 
	string sql_command = "INSERT INTO user(email, password, fullname, device_id) VALUES ('";
	sql_command.append(login+"', '"+password+"', '"+fullname+"', '"+deviceID+"');");
	/*example sql:
INSERT INTO user(email, password, fullname, device_id)
->VALUES('alex@gmail.com', 'qweASD123', 'Alexandr Migachev','SOME_DEVICE_ID_STRING');     */
	qstate = mysql_query(conn, sql_command.c_str());
	if (!qstate) {
		cout << "sql query success!\n";
	}
	else cout << "Query failed: " << mysql_error(conn) << endl;
	return user;
}
int Database::xGetUserId(std::string login)
{
	return 0;
}
void Database::ChangeActiveTable(string q)
{
	ActiveTableName = q;
}
bool Database::isLogin()
{
	if (Client.login != "")
		return true;
	else return false;
}
bool Database::isConnected()
{
	return isConnect;
}
bool Database::CheckEmail(string _login)
{
	string s;
	UserConnection user;
	qstate = mysql_query(conn, "SELECT * FROM user");
	if (!qstate)
	{
		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res))
		{
			//cout << "LOGIN " << row[1] << "\t\t, PWD: " << row[2] << "\t, Role: " << row[3] << endl;
			if ((row[1] == _login)) {
				user.login = row[1];
				return true;
			}
		}
		return false;
		error = 2;
	}
	else cout << "Query failed: " << mysql_error(conn) << endl;
}
int Database::GetAccessLevel()
{
	return Client.role;
}