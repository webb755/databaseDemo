/* Program name: M08 final Programming project
*  Author: Ben Webb
*  Date last updated: 5/14/23
* Purpose: create a database based on library disign laid out in previous modules
*/


#include <iostream>
#include <string>
#include <iomanip>
#include "sqlite3.h"

using namespace std;

void printMainMenu();
void addData(sqlite3 *);
void updateData(sqlite3 *);
void removeData(sqlite3 *);
string chooseTable();	//output table table name for SQL call
int mainMenu();



int callback_function(void* data, int number_of_columns, char** column_values, char** column_names);

int main()
{
	int choice;

	sqlite3 *mydb;

	int rc; 

	//Need to add code to open the database.
	sqlite3* database_connection;
	int rc = 0;
	int rc = sqlite3_open("Library.db", &database_connection); //sqlite3_open() returns 0 if open runs correctly
	char **errmsg = "oops";  /* Error msg written here */	
	if (rc){
		// If the connection doesn't open successfully print an error message from the database and end the program
		std::cerr << "Error opening database: " << sqlite3_errmsg(DB);
		sqlite3_close(database_connection);
		return (-1);
	}
		
	
	cout << "Welcome to the public library\n\n" << endl;
	choice = mainMenu();
	while (true)
	{
		switch (choice) 
		{
			case 1: addData(mydb);		break;
			case 2: updateData(mydb); 	break;
			case 3: removeData(mydb);	break;
			case -1: return 0;
			default: cout << "That is not a valid choice." << endl;
		}
		cout << "\n\n";
		choice = mainMenu();
	}
	sqlite3_close(database_connection);
}

void printMainMenu() 
{
	cout << "Please choose an option (enter -1 to quit):  " << endl;
	cout << "1. add data" << endl;
	cout << "2. update data" << endl;
	cout << "3. delete data" << endl;
}

int mainMenu()
{
	int choice = 0;
	
	printMainMenu();
	cin >> choice;
	while ((!cin || choice < 1 || choice > 3) && choice  != -1)
	{		
		if (!cin)
		{
			cin.clear();
			cin.ignore(1000,'/n');
		}
		cout << "That is not a valid choice." << endl << endl;
		printMainMenu();
		cin >> choice;
	} 
	return choice;
}

int callback_function(void* data, int number_of_columns, char** column_values, char** column_names){
    //loop below prints the column name and all its contents to console like SQL SELECT
    for (i = 0; i < number_of_columns; i++)
        std::cout << column_names[i] << " = " << column_values[i] << std::endl;
    return 0;
}


///////////////////////////////////menu options//////////////////////////////

void addData(sqlite3 *db){
	sqlite3_stmt *pRes;
	string m_strLastError;
	string tableName = chooseTable();
	int attributeCount = 0;
	string query;
	
	string attributes[10];	//this array holds strings for neede attributes
	string attribbuteValues[10];	//holds values to be added
	
	if (tableName == "member"){
		attributes[0] = "card_num";
		attributes[1] = "account_num";
		attributes[2] = "name";
		attributes[3] = "street_address";
		attributes[4] = "city";
		attributes[5] = "state";
		attributes[6] = "zip_code";
		attributes[7] = "pay_card_num";
		attributes[8] = "pay_card_cvv";
		attributes[9] = "pay_card_exp_date";
		attributeCount = 10;
	}
	if (tableName == "account"){
		attributes[0] = "account_num";
		attributes[1] = "invoice_num1";
		attributes[2] = "invoice_num2";
		attributes[3] = "invoice_num3";
		attributes[4] = "invoice_num4";
		attributes[5] = "invoice_num5";	
		attributeCount = 6;
	}
	if (tableName == "material"){
		attributes[0] = "catalogue_num";
		attributes[1] = "title";
		attributes[2] = "author";
		attributes[3] = "publisher";
		attributes[4] = "publish_date";
		attributes[5] = "type";
		attributes[6] = "invoice_num";
		attributeCount = 7;
	}
	if (tableName == "material_invoice"){
		attributes[0] = "invoice_num";
		attributes[1] = "account_num";
		attributes[2] = "catalogue_num";
		attributes[3] = "checkout_date";
		attributeCount = 4;
	}
	
	/*now we select what to change*/
	int changeChoice;
	cout << "Enter 1 to make new record\nEnter 2 to change record\n";
	cin >> changeChoice;
	if(!cin || (changeChoice != 1 && changeChoice != 2)){
		if (!cin){
			cin.clear();
			cin.ignore(1000,'/n');
		}
		cout << "That is not a valid choice." << endl;
		cin >> changeChoice;
	}
	
	//new record
	if (changeChoice == 1){
		query = "INSERT INTO " + tableName + " VALUES (";
		//get entered values
		for (int i=0; i<attributeCount; i++){
			cout << "Enter " << attributes[i] << endl;
			getline(cin>>ws, attributeValues[i]);
			query += attributeValues[i];
			if (i<attributeCount)
				query += ", "
		}
		query += ");";
		//create record with values entered
		int rc = sqlite3_exec(db, query.c_str(), callback, 0, &err_msg);
	    if (rc != SQLITE_OK) {
	        cerr << "there was an error: " << err_msg << endl;
	        sqlite3_free(err_msg);
	    }
	}
	if (changeChoice == 2){
		cout << "Enter " << attributes[0] << " of record to change: \n";
		getline(cin>>ws, attributeValues[0]);
		query = "SELECT * FROM " + tableName + " WHERE " + attributes[0] + " = '" + attributeValues[0] + "';";
		
		
	}
}


void updateData(sqlite3 *db){
	sqlite3_stmt *pRes;
	string m_strLastError;
	string tableName = chooseTable();
	int attributeCount = 0;
	string query;
	
	string attributes[10];	//this array holds strings for neede attributes
	string attribbuteValues[10];	//holds values to be added
	
	if (tableName == "member"){
		attributes[0] = "card_num";
		attributes[1] = "account_num";
		attributes[2] = "name";
		attributes[3] = "street_address";
		attributes[4] = "city";
		attributes[5] = "state";
		attributes[6] = "zip_code";
		attributes[7] = "pay_card_num";
		attributes[8] = "pay_card_cvv";
		attributes[9] = "pay_card_exp_date";
		attributeCount = 10;
	}
	if (tableName == "account"){
		attributes[0] = "account_num";
		attributes[1] = "invoice_num1";
		attributes[2] = "invoice_num2";
		attributes[3] = "invoice_num3";
		attributes[4] = "invoice_num4";
		attributes[5] = "invoice_num5";	
		attributeCount = 6;
	}
	if (tableName == "material"){
		attributes[0] = "catalogue_num";
		attributes[1] = "title";
		attributes[2] = "author";
		attributes[3] = "publisher";
		attributes[4] = "publish_date";
		attributes[5] = "type";
		attributes[6] = "invoice_num";
		attributeCount = 7;
	}
	if (tableName == "material_invoice"){
		attributes[0] = "invoice_num";
		attributes[1] = "account_num";
		attributes[2] = "catalogue_num";
		attributes[3] = "checkout_date";
		attributeCount = 4;
	}
	
	
	cout << "Enter " << attributes[0] << " of record to change: \n";
	getline(cin>>ws, attributeValues[0]);
	query = "SELECT * FROM " + tableName + " WHERE " + attributes[0] + " = '" + attributeValues[0] + "';";
	
	int rc = sqlite3_exec(db, query.c_str(), callback, 0, &err_msg);
	    if (rc != SQLITE_OK) {
	        cerr << "there was an error: " << err_msg << endl;
	        sqlite3_free(err_msg);
	    }
	
	//loop to save all attributes from SQL call above
	for (int i=1; i<attributeCount; i++){
		attributes[i] = reinterpret_cast<const char*>(sqlite3_column_text(pRes,i));
	}
	sqlite3_finalize(pRes);
}


void removeData(sqlite3 *db){
	int choice = chooseTable();
	
	
}

string chooseTable(){
	int choice;
	
	cout 	<< "Select number of chosen table:\n"
			<< "1. Member    2. Account    3. Material    4. material invoice\n";
	
	
	cin >> choice;
	while (!cin || choice < 1 || choice > 4){		
		if (!cin){
			cin.clear();
			cin.ignore(1000,'/n');
		}
		cout << "That is not a valid choice. Try again: ";
		cin >> choice;
		cout << endl;
	} 
	
	string tableName;	
	/*here we use choice from chooseTable() to grab table name for SQL query*/
	switch (choice){
		case 1://member
			tableName = "member";
			break;
		case 2://account
			tableName = "account";
			break;
		case 3://material
			tableName = "material";
			break;
		case 4://material invoice
			tableName = "material_invoice";
			break; 
		default: cout << "That is not a valid choice." << endl;
	}
	
	/*use table name to pull table*/	
	return tableName;	
}



