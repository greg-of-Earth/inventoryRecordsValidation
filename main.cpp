// Calderon, Gregory

/******** Instructor's Feedback Do NOT DELETE

check feedback.md

*********/
#include <algorithm>
#include <cstring>
#include <ctype.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
using namespace std;

struct Car {
	string ID;
	string model;
	int quantity;
	double price;
};

const string ERRORS[] = {
	" Invalid ID format.",
	" Invalid Id Length.",
	" Invalid Model Format.",
	" Invalid Quantity.",
	" Invalid Price.",
};

enum MenuOption {
	VALID = 1,
	INVALID = 2,
	GET_RECORDS = 3,
	QUIT = 4,
};

enum SearchOption {
	SEARCH_ID_MODEL = 1,
	SEARCH_PRICE = 2,
	MENU_MAIN = 3,
};

const int MAX = 100;

// function declarations
void GetData(Car inventoryArray[], int &invCount, int &errCount);
bool isDataValid(Car temp, string &errMess);
void ShowMenu(Car CarInventory[], int, int);
void PrintRecords(Car inventoryArray[], int, string);
void PrintErrors(int);
bool isAllowedAlpha(char);
bool isValidID(Car, string &);
bool isIdLength(Car, string &);
bool isValidModel(Car, string &);
bool isValidQuantity(Car, string &);
bool isValidPrice(Car, string &);
void GetRecordByString(Car inventoryArray[], int);
void GetRecordByPrice(Car inventoryArray[], int);
void SearchMenu(Car CarInventory[], int);
void TransformToUpper(string &, Car CarInventory[], int);
double GetSearchPrice();
double GetSearchRange();

int main() {
	Car CarInventory[MAX];
	int inventoryCount = 0;
	int errorCount = 0;
	int option = -1;

	// validate data and store valids, output invalids
	GetData(CarInventory, inventoryCount, errorCount);
	// show main menu to user
	ShowMenu(CarInventory, inventoryCount, errorCount);

	return 0;
}

// GetData() reads data from the file
void GetData(Car CarInventory[], int &inventoryCount, int &errorCount) {
	ifstream inData;
	ofstream errorData;

	// open files and check if open and check if it opened
	inData.open("carInventory.txt");
	errorData.open("errorOutput.txt");

	if (!inData.is_open()) {
		cout << "ERROR. COULD NOT OPEN INPUT FILE." << endl;
		exit(EXIT_FAILURE);
	}

	if (!errorData.is_open()) {
		cout << "ERROR. COULD NOT OPEN ERROR FILE." << endl;
		inData.close();
		exit(EXIT_FAILURE);
	}

	// format error file
	errorData << fixed << showpoint << setprecision(2);

	string errMess;
	Car temp;

	// peek to see if file is empty
	if (inData.peek() == EOF) {
		cout << "ERROR. FILE INPUT FILE IS EMPTY." << endl;
		exit(EXIT_FAILURE);
	}

	// if not end of file or out-of-bounds then process record
	while (!inData.eof() && inventoryCount < MAX) {
		inData >> temp.ID >> temp.model >> temp.quantity >> temp.price;

		bool validData = true;
		validData = isDataValid(temp, errMess);

		// if the data is valid save it into our inventory else save it to error
		if (validData) {
			CarInventory[inventoryCount] = temp;
			inventoryCount++;
		} else {
			errorData << temp.ID << " " << temp.model << " " << temp.quantity
					  << " " << temp.price << " " << errMess << "\n"
					  << endl;

			errMess = "";
			errorCount++;
		}
	}
	if (inventoryCount >= MAX) {
		cout << "\n***WARNING: STORAGE CAPACITY REACHED. SOME RECORDS WERE NOT "
				"PROCESSED.***\n"
			 << endl;
	} else if (!inData.eof()) {
		cout << "\n***WARNING: UNPROCESSED RECORDS REMAIN.\n" << endl;
	}
	inData.close();
	errorData.close();
}

// validate the data BEFORE passing it to CarInventory[]
bool isDataValid(Car temp, string &errMess) {
	isValidID(temp, errMess);
	isValidModel(temp, errMess);
	isValidQuantity(temp, errMess);
	isValidPrice(temp, errMess);

	return errMess == "";
}

// check if ID is valid format
bool isValidID(Car temp, string &errMess) {
	bool isValid = isIdLength(temp, errMess);
	int errorMessIndex = 0;
	string firstTwo;
	string middleFour;
	string lastThree;
	firstTwo = (temp.ID.substr(0, 2));
	middleFour = (temp.ID.substr(2, 4));
	lastThree = (temp.ID.substr(6, 3));

	if (isValid) {
		// capitalize for uniformity
		for (int i = 0; i < middleFour.length(); i++) {
			middleFour[i] = toupper(middleFour[i]);
		}

		// check first two elements of ID
		if (!isdigit(firstTwo[0]) || !isdigit(firstTwo[1])) {
			isValid = false;
			// check middle four elements of ID
		} else if (
			!isAllowedAlpha(middleFour[0]) || !isAllowedAlpha(middleFour[1]) ||
			!isAllowedAlpha(middleFour[2]) || !isAllowedAlpha(middleFour[3])) {
			isValid = false;
			// check last three elements of ID
		} else if (
			!isalnum(lastThree[0]) || !isalnum(lastThree[1]) ||
			!isalnum(lastThree[2])) {
			isValid = false;
		}
	}
	if (isValid == false) {
		errMess += ERRORS[errorMessIndex];
	}

	return errMess == "";
}

// check that id is of accepted length
bool isIdLength(Car temp, string &errMess) {
	const int LENGTH = 9;
	int errorMessIndex = 1;
	if (temp.ID.length() != LENGTH) {
		errMess += ERRORS[errorMessIndex];
	}
	return errMess == "";
}

// function that checks ID has valid alpha characters
bool isAllowedAlpha(char a) {
	bool passedAlpha = true;
	if (a > 'S' || a == 'O' || a < 'A') {
		passedAlpha = false;
	}

	return passedAlpha;
}

// function that checks car model is correct format
bool isValidModel(Car temp, string &errMess) {
	bool modelValid = true;
	int errorMessIndex = 2;
	string firstLetter;
	firstLetter = (temp.model.substr(0, 1));
	char stringChars[temp.model.length()];

	if (!isalpha(firstLetter[0])) {
		modelValid = false;
	}

	for (int i = 0; i < temp.model.length(); i++) {
		stringChars[i] = temp.model[i];
	}
	for (int i = 0; i < temp.model.length() && modelValid; i++) {
		if (!isalnum(stringChars[i]) && stringChars[i] != '_') {
			modelValid = false;
		}
	}
	if (!modelValid) {
		errMess += ERRORS[errorMessIndex];
	}

	return errMess == "";
}

// Check if quantity is greater than 0
bool isValidQuantity(Car temp, string &errMess) {
	int errorMessIndex = 3;
	if (temp.quantity <= 0) {
		errMess += ERRORS[errorMessIndex];
	}

	return errMess == "";
}

// Check if price is greater than 0
bool isValidPrice(Car temp, string &errMess) {
	bool priceValid = true;
	int errorMessIndex = 4;
	if (temp.price <= 0) {
		priceValid = false;
		errMess += ERRORS[errorMessIndex];
	}
	return errMess == "";
}

// retrieve record by ID or model. allows partial term search
void GetRecordByString(Car CarInventory[], int inventoryCount) {
	string userSrchTerm;
	Car foundRecords[inventoryCount];
	int foundCounter = 0;
	cout << "Enter car ID or Model: ";

	cin >> userSrchTerm;
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	// transform all data to uppercase
	TransformToUpper(userSrchTerm, CarInventory, inventoryCount);

	// check user search term using partial search against toupper(car.ID)
	for (int i = 0; i < inventoryCount; i++) {
		int posID = CarInventory[i].ID.find(userSrchTerm);
		int posModel = CarInventory[i].model.find(userSrchTerm);

		// if search term found, save to found results
		if (posID != string::npos || posModel != string::npos) {
			foundRecords[foundCounter] = CarInventory[i];
			foundCounter++;
		}
	}
	if (foundCounter > 0) {
		string header = "RECORDS FOUND";
		PrintRecords(foundRecords, foundCounter, header);
	} else {
		cout << "\nRECORD NOT FOUND. PLEASE TRY AGAIN.";
	}
}

void TransformToUpper(
	string &userSrchTerm,
	Car CarInventory[],
	int inventoryCount) {
	transform(
		userSrchTerm.begin(),
		userSrchTerm.end(),
		userSrchTerm.begin(),
		::toupper);
	for (int i = 0; i < inventoryCount; i++) {
		transform(
			CarInventory[i].ID.begin(),
			CarInventory[i].ID.end(),
			CarInventory[i].ID.begin(),
			::toupper);
		transform(
			CarInventory[i].model.begin(),
			CarInventory[i].model.end(),
			CarInventory[i].model.begin(),
			::toupper);
	}
}

// retrieve record by price
void GetRecordByPrice(Car CarInventory[], int inventoryCount) {
	Car foundRecords[inventoryCount];
	int foundCounter = 0;
	double searchPrice = -1;
	double range = -1;

	// get price and range from user
	searchPrice = GetSearchPrice();
	range = GetSearchRange();

	double inRangeLow = searchPrice - (searchPrice * (range / 100));
	double inRangeHigh = searchPrice + (searchPrice * (range / 100));

	for (int i = 0; i < inventoryCount; i++)
		if (CarInventory[i].price >= inRangeLow &&
			CarInventory[i].price <= inRangeHigh) {
			foundRecords[foundCounter] = CarInventory[i];
			foundCounter++;
		}
	if (foundCounter > 0) {
		string header = "RECORDS FOUND";
		PrintRecords(foundRecords, foundCounter, header);
	} else {
		cout << "\nRECORD NOT FOUND. PLEASE TRY AGAIN.";
	}
}

double GetSearchPrice() {
	double searchPrice = -1;
	do {
		cout << "Enter Price (larger than 0): ";
		cin >> searchPrice;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		if (searchPrice <= 0) {
			cout << "\nERROR: INVALID PRICE (MUST BE GREATER THAN 0)\n\n"
				 << endl;
		}
	} while (searchPrice <= 0);

	return searchPrice;
}

double GetSearchRange() {
	double range = -1;
	do {
		cout << "Enter price range to search (percent integer) Greater or "
				"Equal to 0: ";
		cin >> range;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		if (range < 0) {
			cout << "\nERROR: INVALID RANGE (MUST BE GREATER THAN OR EQUAL TO "
					"0)\n\n"
				 << endl;
		}
	} while (range < 0);

	return range;
}

// Print the carInventory array and total number of cars in inventory
void PrintRecords(Car CarInventory[], int inventoryCount, string header) {
	// format data so universal font output for all function calls
	TransformToUpper(header, CarInventory, inventoryCount);
	int i;
	cout << "\n***" << header << "***\n"
		 << setfill('-') << setw(52) << "-" << setfill(' ') << "\n"
		 << left << setw(11) << "ID"
		 << " |" << left << setw(10) << "Model"
		 << " | " << right << setw(13) << "Qty"
		 << "| " << right << setw(11) << "Price"
		 << "\n"
		 << setfill('-') << setw(52) << "-" << setfill(' ') << endl;

	for (i = 0; i < inventoryCount; i++) {
		cout << left << setw(11) << CarInventory[i].ID << " |" << left
			 << setw(10) << CarInventory[i].model << " | " << right << setw(13)
			 << CarInventory[i].quantity << "| " << right << setw(11) << fixed
			 << setprecision(2) << CarInventory[i].price << endl;
	}

	cout << setfill('-') << setw(52) << "-" << setfill(' ') << endl;
}

// Print the carInventory array and total number of cars in inventory
void PrintErrors(int errorCount) {
	int i;
	cout << "\n***Errors***\n"
		 << setfill('-') << setw(70) << "-" << setfill(' ') << endl;

	string errorString;
	ifstream errorInput("errorOutput.txt");
	if (!errorInput.is_open()) {
		cout << "ERROR. Could not open Error file." << endl;
		exit(EXIT_FAILURE);
	}
	if (errorInput.peek() == EOF) {
		cout << "ERROR. OUTPUT FILE IS EMPTY." << endl;
		exit(EXIT_FAILURE);
	}

	while (!errorInput.eof()) {
		getline(errorInput, errorString);
		cout << errorString << endl;
	}

	errorInput.close();
}

void ShowMenu(Car CarInventory[], int inventoryCount, int errorCount) {
	int option = -1;
	string header = "INVENTORY";
	// do-while loop to display menu until user quits
	do {
		cout << "\n\n***MENU***\n\n"
				"1. Print Inventory\n"
				"2. Print Errors\n"
				"3. Search Records\n"
				"4. Quit\n\n"
				"Enter Choice: ";

		cin >> option;
		switch (option) {
		case VALID:
			PrintRecords(CarInventory, inventoryCount, header);
			break;
		case INVALID:
			PrintErrors(errorCount);
			break;
		case GET_RECORDS:
			SearchMenu(CarInventory, inventoryCount);
			break;
		case QUIT:
			cout << "\nProgram Terminated" << endl;
			break;
		default:
			cout << "\nInvalid selection. Please choose again.\n" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			break;
		}
	} while (option != QUIT);
}

// sub menu for searching records
void SearchMenu(Car CarInventory[], int inventoryCount) {
	int option = -1;
	do {
		cout << "\n\n***SEARCH MENU***\n\n"
				"1. Search by ID or Model\n"
				"2. Search by Price\n"
				"3. Main Menu\n\n"
				"Enter Choice: ";

		cin >> option;
		switch (option) {
		case SEARCH_ID_MODEL:
			GetRecordByString(CarInventory, inventoryCount);
			break;
		case SEARCH_PRICE:
			GetRecordByPrice(CarInventory, inventoryCount);
			break;
		case MENU_MAIN:
			break;
		default:
			cout << "\nINVALID SELECTION. PLEASE TRY AGAIN.\n" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			break;
		}
	} while (option != MENU_MAIN);
}

// test cases

// ***MENU***

// 1. Print Inventory
// 2. Print Errors
// 3. Search Records
// 4. Quit

// Enter Choice: 1

// ***INVENTORY***
// ----------------------------------------------------
// ID          |Model      |           Qty|       Price
// ----------------------------------------------------
// 82HEFN696   |CHEVY      |            30|    66898.93
// 16ABCE345   |CHEVY      |           711|    39598.00
// 13HGCD456   |MERCEDES   |            12|    55000.00
// 97REED333   |CADILLAC   |             9|    78999.98
// 55HEEP245   |FORD       |            44|    89324.22
// ----------------------------------------------------

// ***MENU***

// 1. Print Inventory
// 2. Print Errors
// 3. Search Records
// 4. Quit

// Enter Choice: 3

// ***SEARCH MENU***

// 1. Search by ID or Model
// 2. Search by Price
// 3. Main Menu

// Enter Choice: 1
// Enter car ID or Model: Hw

// RECORD NOT FOUND. PLEASE TRY AGAIN.

// ***SEARCH MENU***

// 1. Search by ID or Model
// 2. Search by Price
// 3. Main Menu

// Enter Choice: 1
// Enter car ID or Model: he

// ***RECORDS FOUND***
// ----------------------------------------------------
// ID          |Model      |           Qty|       Price
// ----------------------------------------------------
// 82HEFN696   |CHEVY      |            30|    66898.93
// 16ABCE345   |CHEVY      |           711|    39598.00
// 55HEEP245   |FORD       |            44|    89324.22
// ----------------------------------------------------

// ***SEARCH MENU***

// 1. Search by ID or Model
// 2. Search by Price
// 3. Main Menu

// Enter Choice: 1
// Enter car ID or Model: 96

// ***RECORDS FOUND***
// ----------------------------------------------------
// ID          |Model      |           Qty|       Price
// ----------------------------------------------------
// 82HEFN696   |CHEVY      |            30|    66898.93
// ----------------------------------------------------

// ***SEARCH MENU***

// 1. Search by ID or Model
// 2. Search by Price
// 3. Main Menu

// Enter Choice: 2
// Enter Price (larger than 0): 0

// ERROR: INVALID PRICE (MUST BE GREATER THAN 0)

// Enter Price (larger than 0): -98

// ERROR: INVALID PRICE (MUST BE GREATER THAN 0)

// Enter Price (larger than 0): 55000
// Enter price range to search (percent integer) Greater or Equal to 0: -9

// ERROR: INVALID RANGE (MUST BE GREATER THAN OR EQUAL TO 0)

// Enter price range to search (percent integer) Greater or Equal to 0: 55000

// ***RECORDS FOUND***
// ----------------------------------------------------
// ID          |Model      |           Qty|       Price
// ----------------------------------------------------
// 82HEFN696   |CHEVY      |            30|    66898.93
// 16ABCE345   |CHEVY      |           711|    39598.00
// 13HGCD456   |MERCEDES   |            12|    55000.00
// 97REED333   |CADILLAC   |             9|    78999.98
// 55HEEP245   |FORD       |            44|    89324.22
// ----------------------------------------------------

// ***SEARCH MENU***

// 1. Search by ID or Model
// 2. Search by Price
// 3. Main Menu

// Enter Choice: 2
// Enter Price (larger than 0): 55000
// Enter price range to search (percent integer) Greater or Equal to 0: 0

// ***RECORDS FOUND***
// ----------------------------------------------------
// ID          |Model      |           Qty|       Price
// ----------------------------------------------------
// 13HGCD456   |MERCEDES   |            12|    55000.00
// ----------------------------------------------------

// ***SEARCH MENU***

// 1. Search by ID or Model
// 2. Search by Price
// 3. Main Menu

// Enter Choice: 3

// ***MENU***

// 1. Print Inventory
// 2. Print Errors
// 3. Search Records
// 4. Quit

// Enter Choice: 4

// Program Terminated

// end test cases