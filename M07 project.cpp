/* Program name: M07 Programming project
*  Author: Ben Webb
*  Date last updated: 5/8/23
* Purpose: create a database for the NBA
*/


#include <iostream>
#include <string>
#include <iomanip>
#include "sqlite3.h"

using namespace std;

void printMainMenu();
void viewDivision(sqlite3 *);
void viewTeam(sqlite3 *);
void viewGame(sqlite3 *);
void addTeam(sqlite3 *);
void addPlayer(sqlite3 *);
void addGame(sqlite3 *);
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
	int rc = sqlite3_open("NBA.db", &database_connection); //sqlite3_open() returns 0 if open runs correctly
	char **errmsg = "oops";  /* Error msg written here */	
	if (rc){
		// If the connection doesn't open successfully print an error message from the database and end the program
		std::cerr << "Error opening database: " << sqlite3_errmsg(DB);
		sqlite3_close(database_connection);
		return (-1);
	}
		
	
	cout << "This is the NBA!!!" << endl;
	choice = mainMenu();
	while (true)
	{
		switch (choice) 
		{
			case 1: viewDivision(mydb);	break;
			case 2: viewTeam(mydb); 	break;
			case 3: viewGame(mydb);		break;
			case 4: addTeam(mydb);		break;
			case 5: addPlayer(mydb);	break;
			case 6: addGame(mydb);		break;
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
	cout << "1. View a division" << endl;
	cout << "2. View a team" << endl;
	cout << "3. View a game" << endl;
	cout << "4. Add new team" << endl;
	cout << "5. Add new player" << endl;
	cout << "6. Add new game" << endl;
	cout << "Enter Choice: ";
}

int mainMenu()
{
	int choice = 0;
	
	printMainMenu();
	cin >> choice;
	while ((!cin || choice < 1 || choice > 6) && choice  != -1)
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

void viewDivision(sqlite3 * db){
	std::string division_name, team_name, conference;
	//we show user list of divisions
	string getDivisionSQL =	"SELECT division_name \nFROM division; ";
	sqlite3_stmt *pRes;
	string m_strLastError;

	//process query
	if (sqlite3_prepare_v2(db, getDivisionSQL.c_str(), -1, &pRes, NULL) != SQLITE_OK){
		m_strLastError = sqlite3_errmsg(db);
		sqlite3_finalize(pRes);
		cout << "There was an error: " << m_strLastError << endl;
		return;
	}else{	//ask user which division and display result of query
		cout << "Please choose the division you want to see: " << endl;
		int columnCount = sqlite3_column_count(pRes);
		int i = 1, choice;
		sqlite3_stmt *pRes2;
		cout << left;
		//display result of query above
		while (sqlite3_step(pRes) == SQLITE_ROW){
			cout << i << ". " << sqlite3_column_text(pRes, 0);
			//cout << i << ". " << sqlite3_column_text(pRes, --i);
			cout << endl;
			i++;
		}
		
		do{
			if (!cin){
				cin.clear();
				cin.ignore(1000, '/n');
			}
			cin >> choice;
			if (!cin || choice < 1 || choice > i)
				cout << "That is not a valid choice! Try Again!" << endl;
		} while (!cin);

		sqlite3_reset(pRes);

		//now we pull information from the choice given above
		//loop interates to choice above and sets pRes to point to that record
		for (int i = 0; i < choice; i++)
			sqlite3_step(pRes);
		
		//save pulled data
		division_name = reinterpret_cast<const char*>(sqlite3_column_text(pRes,1));
		team_name = reinterpret_cast<const char*>(sqlite3_column_text(pRes,2));
		conference = reinterpret_cast<const char*>(sqlite3_column_text(pRes,3));
		sqlite3_finalize(pRes);

		//need to provide the query to select the chosen division from database
		string chosenDivisionSQL;
		chosenDivisionSQL =		"SELECT * \n";
		chosenDivisionSQL +=	"FROM  division; ";

		//use pRes2 to hold query above
		if (sqlite3_prepare_v2(db, chosenDivisionSQL.c_str(), -1, &pRes2, NULL) != SQLITE_OK){
			m_strLastError = sqlite3_errmsg(db);
			sqlite3_finalize(pRes2);
			cout << "There was an error: " << m_strLastError << endl;
			return;
		}else{
			cout << "Team: " << team_name << std::endl;
			cout << "Conference: " << conference << endl;
			columnCount = sqlite3_column_count(pRes2);
			cout << left;
			for (int i = 0; i < columnCount; i++){
				cout << "|" << setw(25) << sqlite3_column_name(pRes2, i);
			}
			cout << "|" << endl;
			
			while (sqlite3_step(pRes2) == SQLITE_ROW){
				for (int i = 0; i < columnCount; i++){
					if (sqlite3_column_type(pRes2, i) != SQLITE_NULL)
						cout << "|" << setw(25) << sqlite3_column_text(pRes2, i);
					else
						cout << "|" << setw(25) << " ";
				}
				cout << "|" << endl;
			}
			
			sqlite3_finalize(pRes2);
		}//end printing records
	}//end choosing division and printing
}



void viewTeam(sqlite3 * db){
	string team_name, division, mascot, city, state, home_arena, head_coach;
	//we show user list of teams
	string getTeamSQL =	"SELECT team_name \nFROM team; ";
	sqlite3_stmt *pRes;
	string m_strLastError;

	//process query
	if (sqlite3_prepare_v2(db, getTeamSQL.c_str(), -1, &pRes, NULL) != SQLITE_OK){
		m_strLastError = sqlite3_errmsg(db);
		sqlite3_finalize(pRes);
		cout << "There was an error: " << m_strLastError << endl;
		return;
	}else{	//ask user which division and display result of query
		cout << "Please choose the team you want to see: " << endl;
		int columnCount = sqlite3_column_count(pRes);
		int i = 1, choice;
		sqlite3_stmt *pRes2;
		cout << left;
		//display result of query above
		while (sqlite3_step(pRes) == SQLITE_ROW){
			cout << i << ". " << sqlite3_column_text(pRes, 0);
			//cout << i << ". " << sqlite3_column_text(pRes, --i);
			cout << endl;
			i++;
		}
		
		do{
			if (!cin){
				cin.clear();
				cin.ignore(1000, '/n');
			}
			cin >> choice;
			if (!cin || choice < 1 || choice > i)
				cout << "That is not a valid choice! Try Again!" << endl;
		} while (!cin);

		sqlite3_reset(pRes);

		//now we pull information from the choice given above
		//loop interates to choice above and sets pRes to point to that record
		for (int i = 0; i < choice; i++)
			sqlite3_step(pRes);
		
		//save pulled data
		//team_name, division, mascot, city, state, home_arena, head_coach
		team_name = reinterpret_cast<const char*>(sqlite3_column_text(pRes,1));
		division = reinterpret_cast<const char*>(sqlite3_column_text(pRes,2));
		mascot = reinterpret_cast<const char*>(sqlite3_column_text(pRes,3));
		city = reinterpret_cast<const char*>(sqlite3_column_text(pRes,4));
		state = reinterpret_cast<const char*>(sqlite3_column_text(pRes,5));
		home_arena = reinterpret_cast<const char*>(sqlite3_column_text(pRes,6));
		head_coach = reinterpret_cast<const char*>(sqlite3_column_text(pRes,7));
		sqlite3_finalize(pRes);

		//need to provide the query to select the chosen division from database
		string chosenTeamSQL;
		chosenTeamSQL =		"SELECT * \n";
		chosenTeamSQL +=	"FROM  team; ";

		//use pRes2 to hold query above
		if (sqlite3_prepare_v2(db, chosenTeamSQL.c_str(), -1, &pRes2, NULL) != SQLITE_OK){
			m_strLastError = sqlite3_errmsg(db);
			sqlite3_finalize(pRes2);
			cout << "There was an error: " << m_strLastError << endl;
			return;
		}else{
			cout << "Team: " << team_name << std::endl;
			cout << "Division: " << division << endl;
			cout << "Mascot: " << mascot << endl;
			cout << "City: " << city << endl;
			cout << "State: " << state << endl;
			cout << "Home_arena: " << home_arena << endl;
			cout << "head_coach: "  << head_coach << endl;
			columnCount = sqlite3_column_count(pRes2);
			cout << left;
			for (int i = 0; i < columnCount; i++){
				cout << "|" << setw(25) << sqlite3_column_name(pRes2, i);
			}
			cout << "|" << endl;
			
			while (sqlite3_step(pRes2) == SQLITE_ROW){
				for (int i = 0; i < columnCount; i++){
					if (sqlite3_column_type(pRes2, i) != SQLITE_NULL)
						cout << "|" << setw(25) << sqlite3_column_text(pRes2, i);
					else
						cout << "|" << setw(25) << " ";
				}
				cout << "|" << endl;
			}
			
			sqlite3_finalize(pRes2);
		}//end printing records
	}//end choosing division and printing
}

void viewGame(sqlite3 *){	//just displays all games
	string query = "SELECT * ";
	query += "FROM game; ";
	sqlite3_stmt *pRes;
	string m_strLastError;
	string query2;
	string date, home_team, away_team;
	int final_score;
	std::string customerID;
	if (sqlite3_prepare_v2(db, query.c_str(), -1, &pRes, NULL) != SQLITE_OK)
	{
		m_strLastError = sqlite3_errmsg(db);
		sqlite3_finalize(pRes);
		cout << "There was an error: " << m_strLastError << endl;
		return;
	}
	else
	{
		date = reinterpret_cast<const char*>(sqlite3_column_text(pRes,0));
		home_team = reinterpret_cast<const char*>(sqlite3_column_text(pRes,1));
		away_team = reinterpret_cast<const char*>(sqlite3_column_text(pRes,2));
		final_score = reinterpret_cast<const char*>(sqlite3_column_text(pRes,3));
		sqlite3_finalize(pRes);
		
		cout << "Customer: " << cus_fname << " " << cus_lname << endl;
		std::cout << "Customer ID: " << customerID << std::endl;
		columnCount = sqlite3_column_count(pRes2);
		cout << left;

		for (int i = 0; i < columnCount; i++)
			cout << "|" << setw(25) << sqlite3_column_name(pRes2, i);

		cout << "|" << endl;
		
		while (sqlite3_step(pRes2) == SQLITE_ROW){
			for (int i = 0; i < columnCount; i++){
				if (sqlite3_column_type(pRes2, i) != SQLITE_NULL)
					cout << "|" << setw(25) << sqlite3_column_text(pRes2, i);
				else
					cout << "|" << setw(25) << " ";
			}
			cout << "|" << endl;
		}
		
		sqlite3_finalize(pRes2);
	}
}



void addTeam(sqlite3 *){
	string team_name, division, mascot, city, state, home_arena, head_coach;
	//prompt user for needed information
	cout << "Enter team name: "
	getline(cin>>ws, team_name);
	cout << "\nEnter division: ";
	getline(cin>>ws, division);
	cout << "\nEnter mascot: ";
	getline(cin>>ws, mascot);
	cout << "\nEnter city: ";
	getline(cin>>ws, city);
	cout << "\nEnter state: ";
	getline(cin>>ws, state);
	cout << "\nEnter home arena: ";
	getline(cin>>ws, home_arena);
	cout << "\nEnter head coach: ";
	getline(cin>>ws, head_coach);

	string addTeamSQL = "INSERT INTO team 
	VALUES (" + team_name + ", " + division + ", " +  mascot + ", " + city + ", " + state + ", " + home_arena + ", " + head_coach");";

	int rc = sqlite3_exec(db, addTeamSQL.c_str(), callback, 0, &err_msg);
    if (rc != SQLITE_OK) {
        cerr << "there was an error: " << err_msg << endl;
        sqlite3_free(err_msg);
    }
}

void addPlayer(sqlite3 *){
	int number;
	string f_name, l_name, team_name;
	cout << "Enter number: ";
	getline(cin>>ws, number);
	cout << "\nEnter first name: ";
	getline(cin>>ws, f_name);
	cout << "\nEnter last name: ";
	getline(cin>>ws, l_name);
	cout << "\nEnter team: ";
	getline(cin>>ws, team_name);

	string addPlayerSQL = 	"INSERT INTO player 
						VALUES ("+ number + ", " + f_name + ", " + l_name + ", " + team_name + ");";

	int rc = sqlite3_exec(db, addPlayerSQL.c_str(), callback, 0, &err_msg);
    if (rc != SQLITE_OK) {
        cerr << "there was an error: " << err_msg << endl;
        sqlite3_free(err_msg);
    }
}

void addGame(sqlite3 *){
	int final_score;
	string date, home_team, away_team;
	
	cout << "Enter date: ";
	getline(cin>>ws, date);
	cout << "\nEnter home team: ";
	getline(cin>>ws, home_team);
	cout << "\nEnter away team: ";
	getline(cin>>ws, away_team);
	cout << "\nEnter final score: ";
	getline(cin>>ws, final_score);

	string addGameSQL = 	"INSERT INTO game 
						VALUES ("+ date + ", " + home_team + ", " + away_team + ", " + final_score + ");";

	int rc = sqlite3_exec(db, addGameSQL.c_str(), callback, 0, &err_msg);
    if (rc != SQLITE_OK) {
        cerr << "there was an error: " << err_msg << endl;
        sqlite3_free(err_msg);
    }
}
