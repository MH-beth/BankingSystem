#include <iostream>
#include <string>
#include <vector>
#include <mysql.h>
#include <Windows.h>
#include <stdlib.h>
#include <ctime>
#include <sstream>

// includes 
#include "./randomAdress.h"
// end Includes
using namespace std;
// global Variables
MYSQL* conn;
MYSQL_RES* res;
MYSQL_ROW row;
int qstate;
std::vector <std::string> userData;
// end global variables

class dbResponse {
public:
	static void ConnectionFunction()
	{
		conn = mysql_init(0);
		if (!conn)
		{
			std::cout << "Error : " << mysql_errno(conn) << std::endl;
			Sleep(10000);
		}
		conn = mysql_real_connect(conn, "localhost", "root", "", "bank", 0, NULL, 0);
		if (!conn)
		{
			std::cout << "ERROR AT REAL CONN :" << mysql_errno(conn) << std::endl;
			Sleep(1523);
		}

	}
};

std::string unencrypt(string str)
{
	for (int i = 0; (i < 100 && str[i] != '\0'); i++)
	{
		str[i] -= 2;
	}
	return str;
}

class bankUser {
public:
	std::string passworden()
	{
		std::string pass;
		for (int i = 0; i < this->m_password.size(); i++)
		{
			pass += "*";
		}
		return pass;
	}
	bankUser(std::vector <std::string> userData)
	{
		string word = userData[4];
		int resf; 
		std::stringstream ss;
		ss << word;
		ss >> resf;
		this->m_username = userData[1];
		this->m_password = userData[2];
		this->m_enPassword = passworden();
		this->m_email = userData[3];
		this->m_fname = userData[8];
		this->m_lname = userData[9];
		this->m_balance = resf;
		this->m_adress = userData[5];
		this->m_statue = userData[7];
		this->balanceString = std::to_string(this->m_balance);
	}
	
	void AccountData()
	{
		std::cout << "Account " << this->m_username << " Information" << std::endl;
		printf("| %-10s | %-10s | %-10s | %-10s | %-10s | %-10s | %-10s | %-10s |\n", "username", "password", "email", "Firstname", "Lastname", "balance", "Adress", "statue");
		printf("| %-10s | %-10s | %-10s | %-15s | %-10s | %-10s | %-10s | %-10s |\n", this->m_username, this->m_password, this->m_email, this->m_fname, this->m_lname, this->balanceString, this->m_adress, this->m_statue);
		system("PAUSE");
		std::cout << "Balance : " << this->m_balance << " " << userData[6] <<std::endl;
		system("PAUSE");
		this->m_balance++;
		std::cout << "New Balance : " << this->m_balance << std::endl;
		system("PAUSE");
	}
	void UpdateUser()
	{
		userData[1] = this->m_username;
		userData[2] = this->m_password;
		userData[3] = this->m_email;
		userData[4] = std::to_string(this->m_balance);
		userData[5] = this->m_adress;
		userData[7] = this->m_statue;
		userData[8] = this->m_fname;
		userData[9] = this->m_lname;
	}
	void SendMoney()
	{
		std::vector <std::string> transactionData;
		std::string adress;
		bool good = true;
		double amount(0);
		std::cout << "     --- Send Money ---    " << std::endl;
		std::cout << "Caption : You should write the right adress to send money , otherwise your money will be lost !" << std::endl;
		std::cout << "Adress : ";
		std::cin >> adress;
		if ( adress.size() != 14)
		{
			std::cout << "Invalid Adress !" << std::endl;
			good = false;
			system("PAUSE");
		}
		if (good)
		{
			std::vector <std::string> userd;
			std::cout << "Checking the adress validity." << std::endl;
			std::string quert = "select * from users where adresse = '" + adress + "'";
			const char* qy = quert.c_str();
			qstate = mysql_query(conn, qy);
			if (!qstate)
			{
				res = mysql_store_result(conn);
				while (row = mysql_fetch_row(res))
				{
					userd.push_back(row[4]);
					userd.push_back(row[5]);
					userd.push_back(row[6]);
					userd.push_back(row[1]);
				}
			}
			else
			{
				std::cout << "This adress doesn't exist ! or an error have occured : " << mysql_errno(conn) << std::endl;
				system("PAUSE");
				good = false;
			}
			std::cout << "Set the amount you want to send to " << userd[3]  << std::endl;
			std::cout << "Amount : ";
			std::cin >> amount;
			if (amount > this->m_balance)
			{
				std::cout << "You haven't enough money !" << std::endl;
				system("PAUSE");
				good = false;
			}
			else
			{
				this->m_balance -= amount;
				std::cout << "Your New Balance is " << this->m_balance << std::endl;
				std::cout << "Sending " << amount << " $ to " << userd[3] << " " << userd[1] << std::endl;
				Sleep(2000);
				transactionData.push_back(this->m_adress);
				transactionData.push_back(adress);
				bool pass = true;
				string words = userd[0];
				double lol = atof(words.c_str());
				double fBalance = lol + amount;
				std::string fBalances = std::to_string(fBalance);
				std::cout << userd[1] << std::endl;
				std::string qu1 = "UPDATE users SET balance ='" + fBalances + "' WHERE username='" + userd[3] + "'";
				const char* query = qu1.c_str();
				qstate = mysql_query(conn, query);
				if (!qstate)
				{
					pass = true;
				}
				else
				{
					std::cout << "An error have occured : " << mysql_errno(conn) << std::endl;
					system("PAUSE");
					pass = false;
				}
				if (pass)
				{
					bool pass2 = false;
					std::string que = "UPDATE users SET balance='" + std::to_string(this->m_balance) + "' WHERE username='" + this->m_username + "'";
					const char* qo = que.c_str();
					qstate = mysql_query(conn, qo);
					if (!qstate)
					{
						std::cout << "Logging the transaction ..." << std::endl;
						pass2 = true;
						Sleep(2000);
					}
					else
					{
						std::cout << "An error Have occured : " << mysql_errno(conn) << std::endl;
						system("PAUSE");
						good = false;
					}
					if (pass2)
					{
						std::string resd = std::to_string(amount);
						std::string qu2 = "insert into transactions(adresse1, adress2, amount) values ('" + transactionData[0] + "','" + transactionData[1] + "','" + resd + "')";
						const char* query2 = qu2.c_str();
						qstate = mysql_query(conn, query2);
						if (!qstate)
						{
							this->UpdateUser();
							std::cout << "Transaction Sucess ! " << std::endl;
							system("PAUSE");
							good = false;
						}
						else {
							std::cout << "An error have occured at the edn of the transaction : " << mysql_errno(conn) << std::endl;
							system("PAUSE");
							good = false;
						}
					}

				}
					
			}
		}
	}
	void Transaction1()
	{
		system("CLS");
		std::string qa = "select  * from transactions where adresse1='" + this->m_adress + "'";
		const char* ql = qa.c_str();
		qstate = mysql_query(conn, ql);
		if (!qstate)
		{
			res = mysql_store_result(conn);
			while (row = mysql_fetch_row(res))
			{
				printf("| %-10s | %-10s | %-10s | %-10s |\n", "ID" , "Your Adress" , "Adress you sent" , "Amount");
				printf("| %-10s | %-10s | %-10s | %-10s |\n", row[0], row[1], row[2], row[4]);
			}
			system("PAUSE");
		}
		else
		{
			std::cout << "An Error Have Occured : " << mysql_errno(conn) << std::endl;
		}
	}
	void Transaction2()
	{
		system("CLS");
		std::string qa = "select  * from transactions where adresse2='" + this->m_adress + "'";
		const char* ql = qa.c_str();
		qstate = mysql_query(conn, ql);
		if (!qstate)
		{
			res = mysql_store_result(conn);
			while (row = mysql_fetch_row(res))
			{
				printf("| %-10s | %-10s | %-10s | %-10s |\n", "ID", "Adress Who Sent", "Your Adress", "Amount");
				printf("| %-10s | %-10s | %-10s | %-10s |\n", row[0], row[1], row[2], row[4]);
			}
			system("PAUSE");
		}
		else
		{
			std::cout << "An Error Have Occured : " << mysql_errno(conn) << std::endl;
		}
	}


private :
	std::string m_username;
	std::string m_password;// unencrypted !
	std::string m_email;
	std::string m_fname;
	std::string m_lname;
	std::string m_enPassword;
	double m_balance;
	std::string balanceString;
	std::string m_adress;
	std::string m_statue;
};

std::string encrypt(string str)
{
	for (int i = 0; (i < 100 && str[i] != '\0'); i++)
	{
		str[i] += 2;
	}
	return str;
}


bool isChar(char c)
{
	return ((c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z'));
}

bool isDigit(const char c)
{
	return (c >= '0' && c <= '9');
}


bool is_valid(string email)
{
	if (!isChar(email[0])) {


		return 0;
	}

	int At = -1, Dot = -1;


	for (int i = 0;
		i < email.length(); i++) {

		if (email[i] == '@') {

			At = i;
		}

		else if (email[i] == '.') {

			Dot = i;
		}
	}

	if (At == -1 || Dot == -1)
		return 0;

	if (At > Dot)
		return 0;


	return !(Dot >= (email.length() - 1));
}

bool checkExistance(std::string value, std::string table, std::string section)
{
	std::string query = "select * from " + table + " where " + section + " ='" + value + "'";
	const char* qp = query.c_str();
	qstate = mysql_query(conn, qp);
	if (!qstate)
	{
		return true;
	}
	else {
		return false;
	}
}

std::vector <std::string> signUp()
{
	std::vector <std::string> data;
	std::string password, conf;
	std::string datas;
	system("title Banking System : Sign Up");
	std::cout << " --- Banking System : Sign Up --- " << std::endl;
	std::cout << "Firstname :";
	std::cin >> datas;
	data.push_back(datas);
	datas = "";
	std::cout << "Lastname : ";
	std::cin >> datas;
	data.push_back(datas);
	datas = "";
	std::cout << "Username :";
	std::cin >> datas;
	data.push_back(datas);
	datas = "";
	std::cout << "Password : ";
	std::cin >> password;
	std::cout << "Confirm Your Password : ";
	std::cin >> conf;
	if (password == conf)
	{
		data.push_back(encrypt(password));
		std::cout << "email : ";
		std::cin >> datas;
		if (is_valid(datas))
		{
			data.push_back(datas);
			datas = "";
			std::cout << "Your Actual Balance :";
			std::cin >> datas;
			data.push_back(datas);
			datas = "";
			data.push_back(randomAdress());
			std::cout << "Your Adress Is : " << data[6] << std::endl;
		}
		else
		{
			std::cout << "Error :  Invalid Email Adress !" << std::endl;
			system("PAUSE");

		}
	}
	else
	{
		std::cout << "Error : Password doesn't match" << std::endl;
		system("PAUSE");
		std::cout << "Password : ";
		std::cin >> password;
		std::cout << "Confirm Your Password : ";
		std::cin >> conf;
	}
	return data;
}


void storeData(std::vector <std::string> data)
{
	bool usernameExist, emailExist, adressExist;
	std::vector <std::string> usernames;
	std::vector <std::string> emails;
	std::vector <std::string> adress;
	std::string qkl = "select * from users";
	const char* qn = qkl.c_str();
	qstate = mysql_query(conn, qn);
	if (!qstate)
	{
		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res))
		{
			usernames.push_back(row[1]);
			emails.push_back(row[3]);
			adress.push_back(row[9]);
		}
	}
	else
	{
		std::cout << "An error have occured : " << mysql_errno(conn) << std::endl;
		system("PAUSE");
	}
	for (int i = 0; i < usernames.size(); i++)
	{
		if (data[1] == usernames[i])
		{
			usernameExist = true;
			break;
		}
		else
		{
			usernameExist = false;
		}
	}
	if (usernames.size() == 0)
	{
		usernameExist = false;
	}
	for (int j = 0; j < emails.size(); j++)
	{
		if (data[4] == emails[j])
		{
			emailExist = true;
			break;
		}
		else
		{
			emailExist = false;
		}
	}
	if (emails.size() == 0)
	{
		emailExist = false;
	}
	for (int k = 0; k < adress.size(); k++)
	{
		if (data[6] == adress[k])
		{
			adressExist = true;
			break;
		}
		else
		{
			adressExist = false;
		}
	}
	if(usernameExist || emailExist)
	{
		std::cout << "Username or Email is Alread used please retry with another one" << std::endl;
		system("PAUSE");
	}
	if (adressExist)
	{
		std::string newAdress = randomAdress();
		data[6] = newAdress;
	}
	else
	{
		std::string qml = "insert into users(username,password,email,fname,lname,balance,adresse,statue) values ('" + data[2] + "','" + data[3] + "','" + data[4] + "','" + data[0] + "','" + data[1] + "','" + data[5] + "','" + data[6] + "','normal')";
		const char* q = qml.c_str();
		qstate = mysql_query(conn, q);
		if (!qstate)
		{
			std::cout << "User Succesfully Added !" << std::endl;
			system("PAUSE");
		}
		else std::cout << "Error : " << mysql_errno(conn) << std::endl;
	}
}

bool loginSucess(std::string username, std::string password)
{
	try {
		int bst;
		std::string queryd = "select * from users";
		const char* qmp = queryd.c_str();
		std::vector <std::string>* usernames = new std::vector <std::string>;
		std::vector <std::string>* passwords = new std::vector <std::string>;
		qstate = mysql_query(conn, qmp);
		if (!qstate)
		{
			res = mysql_store_result(conn);
			while (row = mysql_fetch_row(res))
			{
				usernames->push_back(row[1]);
				passwords->push_back(row[2]);
			}
		}
		else
		{
			std::cout << "Error : " << mysql_errno(conn) << std::endl;
			system("PAUSE");
		}
		for (int i = 0; i < usernames->size(); i++)
		{
			if (username == usernames->at(i))
			{
				bst = i;
				break;
			}
			else
			{
				bst = NULL;
			}
		}
		std::string pass;
		std::string resPass;
		if (bst != NULL)
		{
			pass = passwords->at(bst);
			resPass = unencrypt(pass);
		}
		else
		{
			return false;
		}
		if (resPass == password)
		{
			std::string qo = "select * from users where username ='" + username + "'";
			const char* qs = qo.c_str();
			qstate = mysql_query(conn, qs);
			if (!qstate)
			{
				res = mysql_store_result(conn);
				while (row = mysql_fetch_row(res))
				{
					for (int i = 0; i <= 9; i++)
					{
						userData.push_back(row[i]);
						std::cout << row[i] << std::endl;
					}
				}
			}
			delete usernames;
			delete passwords;
			return true;
		}
		else
		{
			return false;
		}
	}
	catch (std::string e) {
		std::cout << "Error" << e << std::endl;
		system("PAUSE");
		return false;
	}
}


void connecteds(std::vector <std::string> userData)
{
	bool connected = true;
	bankUser user(userData);
	while (connected)
	{
		std::cout << "Connected Sucessfully to " << userData[1] << std::endl;
		Sleep(2000);
		std::string qj = "title Banking System :" + userData[1];
		const char* qp = qj.c_str();
		system(qp);
		system("cls");
		system("cls");
		std::string command;
		std::cout << "    Banking System : Main menue : " << std::endl;
		std::cout << "1. See the Account Balance !" << std::endl;
		std::cout << "2. Send Money To An adress ! " << std::endl;
		std::cout << "3. See all My transactions" << std::endl;
		std::cout << "[EXIT] Disconnect" << std::endl;
		std::cin >> command;
		if (command == "1")
		{
			system("cls");
			system("title Banking System : Account Data ");
			user.AccountData();
			system("PAUSE");
		}
		if (command == "2")
		{
			system("cls");
			system("title Banking System : Send Money!");
			user.SendMoney();
		}
		if (command == "3")
		{
			std::string com;
			system("cls");
			system("title Banking System Transactions Menue");
			std::cout << "1. Money You Sent" << std::endl;
			std::cout << "2. Money You received " << std::endl;
			std::cout << "What dd you want to chose : ";
			std::cin >> com;
			if (com == "1")
			{
				user.Transaction1();
			}
			if (com == "2")
			{
				user.Transaction2();
			}
			
		}
	}
}


	
int main()
{
	system("title Banking System ");
	bool running = true;
	while (running)
	{
		system("cls");
		dbResponse::ConnectionFunction();
		std::string command;
		std::cout << "                    ---- Banking System ----" << std::endl;
		std::cout << "1. Login" << std::endl;
		std::cout << "2. Sign Up" << std::endl;
		std::cout << "3. Exit" << std::endl;
		std::cout << "Command between (1,2,3) : ";
		std::cin >> command;
		if (command == "1")
		{
			try {
				system("cls");
				system("title Banking System : Login");
				std::string usernameLo, passwordLo;
				std::cout << "                      ---- Banking System :  Login " << std::endl;
				std::cout << "Username :";
				std::cin >> usernameLo;
				std::cout << "Password : ";
				std::cin >> passwordLo;
				bool connected = loginSucess(usernameLo, passwordLo);
				if (connected)
				{
					connecteds(userData);
				}
				else
				{
					std::cout << "username doesn't exists or/password error" << std::endl;
					system("PAUSE");
				}
			}
			catch (std::string e){
			std::cout << "Error Catcehd : " << e << std::endl;
			system("PAUSE");
			}
		}
		if (command == "2")
		{
			system("cls");
			system("title Banking System : Sign up");
			std::string fname, lname, username, password, conf, email, adresse, statue;
			int balance;
			std::vector <std::string> data = signUp();
			storeData(data);
		}
		if (command == "3")
		{
			system("cls");
			std::cout << "Existing The Programm ..." << std::endl;
			Sleep(2000);
			std::cout << "Exited !" << std::endl;
			running = false;
			return 0;
		}
		if (command == "test")
		{
			std::cout << "These are the part of the software where functions are being tested" << std::endl;
			system("PAUSE");
		}
	}
}

// end Programing
