#include "Database.h"
#include <string>
#include <iostream>
#include <stdlib.h>

Database::Database()
{
	Client.login = "";
	Client.password = "";
	Client.role = 0;
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
	ActiveTableName = "test";
	qstate = 0;
	conn = mysql_init(0);
	conn = mysql_real_connect(conn, IP.c_str(), login.c_str(),
		password.c_str(), dbname.c_str(), port, NULL, 0);
	if (conn) {
		cout << "Connection with SQL server is good!\n"; isConnect = true;
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
	qstate = mysql_query(conn, "DESCRIBE test");
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

	/*
	qstate = mysql_query(conn, "DESCRIBE test");
	if (!qstate)
	{
		res = mysql_store_result(conn);
		cout << "type defining\n";
		while (row = mysql_fetch_row(res))
		{
			//cout << row[0] << "\t" << row[1] << endl;
			string temp(row[1]); // � row[1] ��������� ���� �����
			if (temp == "int")
			{
				cout << "ITS INT!!!!!!\n";

			}
			else
			{
				cout << "ITS STRING!!!!!!\n";

			}
			//cout << row[0] << endl; � row[0] ��������� �������� �����
		} cout << endl;
		//for (int i = 0; i < type.size(); ++i)
			//cout << "bool " << i << " is " << type[i] << endl;
	}
	else cout << "Query failed: " << mysql_error(conn) << endl;*/
	string s;
	qstate = mysql_query(conn, "SELECT * FROM users");
	if (!qstate)
	{
		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res))
		{
			//cout << "LOGIN " << row[0] << "\t\t, PWD: " << row[1] << "\t, Role: " << row[2] << endl;
			if ((row[0] == _login) && (row[1] == _password))
			{
				Client.login = row[0];
				s = row[2];
				Client.role = atoi(s.c_str());
			}

		}
	}
	else cout << "Query failed: " << mysql_error(conn) << endl;
}
UserConnection Database::xLoadUserFromTable(string _login, string _password)
{
	string s;
	UserConnection user;
	qstate = mysql_query(conn, "SELECT * FROM users");
	if (!qstate)
	{
		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res))
		{
			//cout << "LOGIN " << row[0] << "\t\t, PWD: " << row[1] << "\t, Role: " << row[2] << endl;
			if ((row[0] == _login) && (row[1] == _password))
			{
				user.login = row[0];
				user.password = row[1]; //потом переделать на хеш от пароля
				s = row[2];
				Client.role = atoi(s.c_str());
			}
		}
	}
	else cout << "Query failed: " << mysql_error(conn) << endl;
	return user;
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
int Database::GetAccessLevel()
{
	return Client.role;
}