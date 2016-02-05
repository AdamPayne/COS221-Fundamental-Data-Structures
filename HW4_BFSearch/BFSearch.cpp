#include <iostream>
#include <iomanip>
#include <cstring>
#include <sstream>
#include <math.h>
#include <vector>
#include <stack>
#include <queue>
#include <algorithm>
using namespace std;

/* Utility functions: */
int add(int a, int b) {
	return a + b;
}
int subtract(int a, int b) {
	return a - b;
}
char& straightAccess(char temp[][7], int a, int b) {
	return temp[a][b];
}
char& flippedAccess(char temp[][7], int a, int b) {
	return temp[b][a];
}
//compare if two lots are identical (returns false in that case, similar to strcmp):
bool compare(char[][7], char[][7]);
//copy the contents of the first lot into the second one:
void copy(char[][7], char[][7]);
//print a grid of the parking lot:
void print(char[][7]);
//fill an empty parking lot (0 signifies an empty spot):
void setEmptyPL(char[][7]);

/* Functions to use preset parking lots: */
//ask which preset parking lot to use
void preset();
//actually fill the 'pl' variable with the preset parking lot
//(I've left the definition of this function at the very end because it takes a lot of space):
void setPresetPL(int);

/* Functions to set a custom parking lot: */
//Creates a custom parking lot
//Sets the X car first and then lets the user put as many other vehicles:
void setCustomPL();
//Asks the user to enter the size, disposition and coordinate of a vehicle 
//and returns false in case the vehicle would overlap with an already existing vehicle:
bool inputVehicle(char[][7], char);
//Asks for either the X or Y coordinate and validates it based on the size and disposition of the vehicle
//to ensure the vehicle won't go outside the parking lot
//it returns a valid coordinate:
int validateCoordinate(char, int, char, char, char);
//Validates the vehicle in terms of the already existing vehicles
bool validateVehicle(char[][7], char, int, int, int, char& (*)(char[][7], int, int));

/* Functions to implement Breadth-First Search: */
//Creates a queue with all the unique states, for which the subsequent possible states weren't explored (not all subsequent states, only those that move one vehicle)
//immediately after the subsequent states are explored, pop the state from the queue and explore the next state
//Immediately after a winning state is found, the wining moves get printed
//If a winning scenario can't be achieved, the algorithm exhaust all possibilities and then notifies the user of that fact:
void BFsearch(char[][7]);
//A function to computer all the possbile moves for a given vehicle:
bool computeStates(int, int, queue<char(*)[7]>&, vector<char(*)[7]>&, vector<int>&, vector<char*>&, char& (*)(char[][7], int, int));
//Moves the vehicle only by one 'square':
void moveVehicle(char[][7], int, int, int(*)(int, int), char& (*)(char[][7], int, int));
//Checks if the states achived after a movement has not been already found
//Returns false for an already explored state:
bool searchState(vector<char(*)[7]>&, char[][7]);
//Checks if a 'square' (a 'square' that can me moved to) next to the vehicle is free (returns true in that case)
//If it is free it moves the vehicle using moveVehicle and checks if the new state is new using searchState
//	If it is a new state it adds it the queue and to the vector visitedStates
bool validateState(char[][7], int, int, queue<char(*)[7]>&, vector<char(*)[7]>&, vector<int>&, vector<char*>&, int(*)(int, int), char& (*)(char[][7], int, int));

int main() {
	char vehicle = 't';
	int option = -1;
	string input = "";
	do {
		option = -1;
		cout << "Do you want to choose from the preset parking lots or create your own?\n";
		cout << "1. Choose from preset.\n";
		cout << "2. Create your own.\n";
		cout << "Input: ";
		getline(cin, input);
		stringstream(input) >> option;
		switch (option) {
		case 1:
			preset();
			break;
		case 2:
			setCustomPL();
			break;
		default:
			cout << "Wrong Input!\nTry again.\n\n";

		}
	} while (option != 1 && option!=2);
	system("pause");
	return 0;
}

bool compare(char pl1[][7], char pl2[][7]) {
	bool fl = false;
	for (int i = 0; i < 6 && !fl; i++)
		fl = fl || strcmp(pl1[i], pl2[i]);
	return fl;
}

void copy(char pl1[][7], char pl2[][7]) {
	for (int i = 0; i < 6; i++)
		strcpy_s(pl2[i], pl1[i]);
}

void print(char pl[][7]) {
	cout << setw(24) << "Parking Lot" << "\n\n";
	cout << '\t' << setfill('-') << setw(25) << "" << '\n';
	for (int i = 0; i < 6; i++) {
		cout << "\t| ";
		for (int j = 0; j < 6; j++) {
			cout << (pl[i][j] == '0' ? ' ' : pl[i][j]) << " | ";
		}
		cout << "\n\t" << setfill('-') << setw(25) << "" << '\n';
	}
	cout << setfill(' ');
}

void setEmptyPL(char pl[][7]) {
	for (int i = 0; i < 6; i++)
		strcpy_s(pl[i], "000000");
}

void preset() {
	string input = "";
	int option = 0;
	do {
		option = -1;
		cout << "\nChoose from 1 to 5 (1 is the easiest, 5 is the hardest).\n";
		cout << "0 to exit.\n";
		cout << "Input: ";
		getline(cin, input);
		stringstream(input) >> option;
		setPresetPL(option);
	} while (option < 1 && option>5);
}

void setCustomPL() {
	char pl[6][7];
	vector<char> vehicles;
	setEmptyPL(pl);
	print(pl);
	int x = -1;
	string input;
	do {
		cout << "Choose from 1 to 4 to set the the left end of the car that is to exit the parking lot (car X).\n";
		cout << "Input: ";
		getline(cin, input);
		stringstream(input) >> x;
		if (x<1 || x > 4)
			cout << "\nWrong Input! Try again.\n\n";
		else {
			pl[2][x - 1] = 'X';
			pl[2][x] = 'X';
		}
	} while (x<1 || x>4);
	vehicles.push_back('X');
	print(pl);
	char vehicle = '0';
	do {
		vehicle = '0';
		cout << "Enter the name of the vehicle you want to put next in the parking.\n";
		cout << "0 for exit.\n";
		cout << "Input: ";
		getline(cin, input);
		vehicle = input[0];
		if (vehicle != '0') {
			if (find(vehicles.begin(), vehicles.end(), vehicle) == vehicles.end()) {
				if (inputVehicle(pl, vehicle)) {
					vehicles.push_back(vehicle);
					cout << "Vehicle addedd successfuly!\n";
					print(pl);
				}
				else
					cout << "\nThe assignment of the vehicle failed because one or more of the spaces were already occupied.\n\n";
			}
			else {
				cout << "\nA vehicle with such a name is already present!\n\n";
			}
		}
	} while (vehicle != '0');
	BFsearch(pl);
}

bool inputVehicle(char pl[][7], char vehicle) {
	short size = -1;
	char position = 'v';
	short x = 0, y = 0;
	string input = "";
	do {
		cout << "Enter the size of the vehicle (2 or 3).\n";
		cout << "Input: ";
		getline(cin, input);
		stringstream(input) >> size;
		if (size<2 || size>3) cout << "\nWrong Input! Try again.\n\n";
	} while (size<2 || size>3);
	do {
		cout << "Is it a horizontal (h) or a vertical (v) vehicle?\n";
		cout << "Input: ";
		getline(cin, input);
		position = input[0];
		if (input != "v" && input != "h") cout << "\nWrong Input! Try again.\n\n";
	} while (input != "v" && input != "h");
	x = validateCoordinate('x', size, position, 'v', 'h');
	y = validateCoordinate('y', size, position, 'h', 'v');
	if (position == 'v') {
		return validateVehicle(pl, vehicle, y - 1, x - 1, size, straightAccess);
	}
	else {
		return validateVehicle(pl, vehicle, x - 1, y - 1, size, flippedAccess);
	}
}

int validateCoordinate(char coordinate, int size, char position, char checkPosition1, char checkPosition2) {
	int coord = 0;
	string input = "";
	do {
		coord = 0;
		cout << "Enter the " << (coordinate == 'x' ? "column" : "row") << " for the most upper left end of the vehicle.\n";
		cout << "Input: ";
		getline(cin, input);
		stringstream(input) >> coord;
		if (coord<1 || ((coord>6 && position == checkPosition1) || (coord>4 + (3 - size) && position == checkPosition2)))
			cout << "\nWrong Input! Try again.\n\n";
	} while (coord<1 || ((coord>6 && position == checkPosition1) || (coord>4 + (3 - size) && position == checkPosition2)));
	return coord;
}

bool validateVehicle(char pl[][7], char vehicle, int i, int j, int size, char& (*access)(char[][7], int, int)) {
	if (access(pl, i, j) != '0' || access(pl, i + 1, j) != '0' || access(pl, i + size - 1, j) != '0') {
		return false;
	}
	else {
		access(pl, i, j) = vehicle;
		access(pl, i + 1, j) = vehicle;
		access(pl, i + size - 1, j) = vehicle;
		return true;
	}
}

void BFsearch(char pl[][7]) {
	//states to explore
	queue<char(*)[7]> states;
	//a vector of visited states to avoid looping
	vector<char(*)[7]> visitedStates;
	//a vector to hold the sequential number of the mother state, i.e.
	//preState.at(5)==2 means that state number 5 in visitedStates originates from state number 2, again in visitedStates
	vector<int> preState;
	//holds the string would the move made where:
	//move.at(n)[0] holds the name of the moved vehicle
	//move.at(n)[1] holds the direction of movement
	//move.at(n)[2] holds the number of squares moved
	vector<char*> move;
	//do some preliminary initialization
	visitedStates.push_back(new char[6][7]);
	copy(pl, visitedStates.back());
	states.push(visitedStates.back());
	preState.push_back(-1);
	move.push_back(new char);
	move.back()[0] = 'b';
	move.back()[1] = 'e';
	move.back()[2] = 'g';
	bool fl = true;
	// loop until a winning state is achieved or exhaust all the possibilities
	do {
		for (int i = 0; i < 6 && fl; i++) {
			for (int j = 0; j < 6 && fl; j++) {
				if (states.front()[i][j] != '0' && (i == 0 || states.front()[i - 1][j] != states.front()[i][j]) && (j == 0 || states.front()[i][j - 1] != states.front()[i][j])) {
					//A vehicle is identified
					if (states.front()[i][j] == states.front()[i + 1][j]) {
						//The vehicle is a verticle one
						//put i (it is the one to change) before j  and use straightAccess
						fl = computeStates(i, j, states, visitedStates, preState, move, straightAccess);
					}
					else {
						//The vehicle is a horizontal one
						//put j (it is the one to change) before i and use flippedAccess
						fl = computeStates(j, i, states, visitedStates, preState, move, flippedAccess);
					}
				}
			}
		}
		states.pop();
	} while (!states.empty() && fl);
	if (fl) {
		cout << "There are no solutions to this scenario!\n";
	}
	else {
		// constrcut a stack with the winning moves so that the moves can be shown chronologically
		stack<int> winMoves;
		winMoves.push(visitedStates.size() - 1);
		while (preState.at(winMoves.top()) != -1) {
			winMoves.push(preState.at(winMoves.top()));
		}
		int count = 0;
		string input = "";
		cout << "\nDo you want a detailed report, printing the parking lot after each move was made?\n";
		cout << "(y/n): ";
		getline(cin, input);
		do {
			cout << "\nMove No: " << count << '\t' << move.at(winMoves.top())[0] << ' ' << move.at(winMoves.top())[1] << ' ' << move.at(winMoves.top())[2] << ' ' << '\n';
			if (input == "y") {
				print(visitedStates.at(winMoves.top()));
				if (count % 10 == 9){
					string nxtLine;
					cout << "\nPress enter to view next 10 moves.";
					getline(cin, nxtLine);
				}
			}
			count++;
			winMoves.pop();
		} while (!winMoves.empty());
	}
	while (!states.empty()){
		states.pop();
	}
	while (!visitedStates.empty()){
		delete[] visitedStates.back();
		visitedStates.pop_back();
	}
	preState.clear();
	while (!move.empty()){
		delete [] move.back();
		move.pop_back();
	}
}

bool computeStates(int i, int j, queue<char(*)[7]>& states, vector<char(*)[7]>& visitedStates, vector<int>& preState, vector<char*>& move, char& (*access)(char[][7], int, int)) {
	//temp is a parking lot that stays the same until all the possible moves in one direction are made
	char temp[6][7];
	copy(states.front(), temp);
	//moves the vehicle up if access==straightAccess else it moves it to the left
	//each successful iteration means there is i-k number of free squares up/left
	//the loop is stopped when the end of the parking lot is reached or another vehicle was found as an obstacle
	for (int k = i - 1; k > -1 && validateState(temp, k, j, states, visitedStates, preState, move, add, access); k--) {
		// If a new state was added, change the number of squares moved to the actual value
		if (move.back()[2] == '0') move.back()[2] += abs(i - k);
		//Check if a winning state was reached
		if (temp[2][4] == 'X' && temp[2][5] == 'X') {
			move.back()[2] += 2;
			return false;
		}
	}
	copy(states.front(), temp);
	//moves the vehicle down if access==straightAccess else it moves it to the right
	//each successful iteration means there is abs(i+1+(access(states.front(),i+2,j)==access(states.front(),i+1,j))-k) number of free squares down/right
	for (int k = i + 2 + (access(temp, i + 2, j) == access(temp, i + 1, j)); k < 6 && validateState(temp, k, j, states, visitedStates, preState, move, subtract, access); k++) {
		if (move.back()[2] == '0') move.back()[2] += abs(i + 1 + (access(states.front(), i + 2, j) == access(states.front(), i + 1, j)) - k);
		if (temp[2][4] == 'X' && temp[2][5] == 'X') {
			move.back()[2] += 2;
			return false;
		}
	}
	return true;
}

void moveVehicle(char temp[][7], int k, int j, int(*op)(int, int), char& (*access)(char[][7], int, int)) {
	access(temp, k, j) = access(temp, op(k, 1), j);
	access(temp, op(k, 2), j) = (access(temp, op(k, 3), j) == access(temp, op(k, 1), j) ? access(temp, op(k, 1), j) : '0');
	access(temp, op(k, 3), j) = (access(temp, op(k, 2), j) != access(temp, op(k, 1), j) ? access(temp, op(k, 3), j) : '0');
}

bool searchState(vector<char(*)[7]>& visitedStates, char temp[][7]) {
	vector<char(*)[7]>::iterator it;
	for (it = visitedStates.begin(); it != visitedStates.end() && compare(*it, temp); ++it);
	return it == visitedStates.end();
}

bool validateState(char temp[][7], int k, int j, queue<char(*)[7]>& states, vector<char(*)[7]>& visitedStates, vector<int>& preState, vector<char*>& move, int(*op)(int, int), char& (*access)(char[][7], int, int)) {
	//check if the the square is free
	if (access(temp, k, j) == '0') {
		//move the vehicle one square
		moveVehicle(temp, k, j, op, access);
		//check if the new state isn't already explored or to be explored
		if (searchState(visitedStates, temp)) {
			visitedStates.push_back(new char[6][7]);
			copy(temp, visitedStates.back());
			states.push(visitedStates.back());
			preState.push_back(visitedStates.size() - states.size());
			move.push_back(new char);
			move.back()[0] = access(temp, op(k, 1), j);
			if (op == add) {
				if (access == straightAccess)
					move.back()[1] = 'U';
				else
					move.back()[1] = 'L';
			}
			else {
				if (access == straightAccess)
					move.back()[1] = 'D';
				else
					move.back()[1] = 'R';
			}
			move.back()[2] = '0';
		}
		return true;
	}
	else {
		return false;
	}
}

void setPresetPL(int option) {
	char pl[6][7];
	switch (option) {
		//1 in the manual
	case 1:
		strcpy_s(pl[0], "AA000O");
		strcpy_s(pl[1], "P00Q0O");
		strcpy_s(pl[2], "PXXQ0O");
		strcpy_s(pl[3], "P00Q00");
		strcpy_s(pl[4], "B000CC");
		strcpy_s(pl[5], "B0RRR0");
		break;
		//11 in the manual
	case 2:
		strcpy_s(pl[0], "OAAP00");
		strcpy_s(pl[1], "O00P00");
		strcpy_s(pl[2], "OXXP00");
		strcpy_s(pl[3], "00BQQQ");
		strcpy_s(pl[4], "00B00E");
		strcpy_s(pl[5], "00RRRE");
		break;
		//21 in the manual
	case 3:
		strcpy_s(pl[0], "AABO00");
		strcpy_s(pl[1], "P0BO00");
		strcpy_s(pl[2], "PXXO00");
		strcpy_s(pl[3], "PQQQ00");
		strcpy_s(pl[4], "000000");
		strcpy_s(pl[5], "000RRR");
		break;
		//31 in the manual 
	case 4:
		strcpy_s(pl[0], "AA0OOO");
		strcpy_s(pl[1], "000BCC");
		strcpy_s(pl[2], "DXXB0P");
		strcpy_s(pl[3], "D0QEEP");
		strcpy_s(pl[4], "FFQ00P");
		strcpy_s(pl[5], "00QRRR");
		break;
		//40 in the manual
	case 5:
		strcpy_s(pl[0], "OAA0B0");
		strcpy_s(pl[1], "OCD0BP");
		strcpy_s(pl[2], "OCDXXP");
		strcpy_s(pl[3], "QQQE0P");
		strcpy_s(pl[4], "00FEGG");
		strcpy_s(pl[5], "HHFII0");
		break;
	default:
		cout << "Wrong Input!\nTry again.\n";
	}
	if (option>0 && option<6) BFsearch(pl);
}