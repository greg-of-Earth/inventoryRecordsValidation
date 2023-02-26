//Calderon, Gregory

/******** Instructor's Feedback Do NOT DELETE

check feedback.md

*********/
#include <iostream>
#include <fstream>
#include <iomanip> 
#include <string>
#include <cstring>
#include <stdio.h>
#include <ctype.h>
using namespace std;


struct Car
{
  string ID;
  string model;
  int quantity;
  double price;
};

const string ERRORS[] = 
{
  " Invalid ID format.",
  " Invalid Id Length.",
  " Invalid Model Format.",
  " Invalid Quantity.",
  " Invalid Price.",
};

enum MenuOption 
{
  VALID=1,
  INVALID=2,
  QUIT=3,
};
const int MAX=100;

//function declarations
void GetData(Car inventoryArray[], int &invCount, int &errCount); 
bool isDataValid(Car temp, string &errMess);
void PrintValid(Car inventoryArray[], int inventoryCount);
void PrintErrors(int);
bool isAllowedAlpha(char);
bool isValidID(Car, string&);
bool isIdLength(Car, string&);
bool isValidModel(Car, string&);
bool isValidQuantity(Car, string&);
bool isValidPrice(Car, string&);
int ShowMenu();


int main() {
  //variable and name declarations
  Car CarInventory[MAX];
  int inventoryCount = 0;
  int errorCount = 0;
  
  char userChoice;
  int option=-1;
  
  GetData(CarInventory, inventoryCount, errorCount);

  // do-while loop to display menu until user quits 
  do 
    {
     cout << "\n\n***MENU***\n\n"
              "1. Print Inventory\n"
              "2. Print Errors\n"
              "3. Quit\n\n"
              "Enter Choice: ";
    
      cin >> option;
      switch(option)
      {
        case VALID:
          PrintValid(CarInventory, inventoryCount);
          break;
        case INVALID:
          PrintErrors(errorCount);
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
  
  return 0;
}


//GetData() reads data from the file 
void GetData(Car CarInventory[], int& inventoryCount, int& errorCount)
{
  ifstream inData;
  ofstream errorData;

   // open files and check if open and check if it opened
  inData.open("carInventory.txt");
  errorData.open("errorOutput.txt");
  
  if(!inData.is_open()) 
  {
    cout << "ERROR. Could not open input file." << endl;
    exit(EXIT_FAILURE);
  }

  if(!errorData.is_open())
  {
    cout << "ERROR. Could not open error file." << endl;
    exit(EXIT_FAILURE);
  }

  errorData<<fixed<<showpoint<<setprecision(2);
  
  string errMess;
  Car temp;

  // peek to see if file is empty
   if(inData.peek() == EOF)
    {
        cout << "ERROR. FILE INPUT FILE IS EMPTY." << endl;
        exit(EXIT_FAILURE);
    }

 // if not end of file or out-of-bounds then process record
  while(!inData.eof() && inventoryCount < MAX)
    {
         inData >> temp.ID >> temp.model >> temp.quantity >> temp.price;
  
         bool validData = true;
         validData = isDataValid(temp, errMess);

         //if the data is valid save it into our inventory else save it to error
          if(validData)
         {
           CarInventory[inventoryCount] = temp;
           inventoryCount++;
          }
          else
         { 
           errorData << temp.ID << " " << temp.model << " " << temp.quantity << " " << temp.price << " " << errMess << "\n" << endl;
        
            errMess = "";
            errorCount++;
          }
        }
  if (inventoryCount >= MAX)
{
    cout << "\n***WARNING: STORAGE CAPACITY REACHED. SOME RECORDS WERE NOT PROCESSED.***\n" << endl;
}
else if (!inData.eof())
{
    cout << "\n***WARNING: UNPROCESSED RECORDS REMAIN.\n" << endl;
} 
  inData.close();
  errorData.close();
}

// validate the data BEFORE passing it to CarInventory[]
bool isDataValid(Car temp, string &errMess)
{  
 isValidID(temp,errMess);
 isValidModel(temp, errMess);
 isValidQuantity(temp, errMess);
 isValidPrice(temp, errMess);
  
 return errMess=="";
}

// check if ID is valid format
bool isValidID(Car temp, string &errMess)
{
  
  bool isValid = isIdLength(temp, errMess);    
  int errorMessIndex = 0;
  string firstTwo;
  string middleFour;
  string lastThree;
  firstTwo = (temp.ID.substr(0,2));
  middleFour = (temp.ID.substr(2,4));
  lastThree = (temp.ID.substr(6, 3));

 if(isValid)
 {
  //capitalize for uniformity
  for(int i = 0; i < middleFour.length(); i++)
    {
      middleFour[i] = toupper(middleFour[i]);
    }
    
  // check first two elements of ID 
  if (!isdigit(firstTwo[0]) || !isdigit(firstTwo[1]))
    {
      isValid = false;
    // check middle four elements of ID   
    }else if(!isAllowedAlpha(middleFour[0]) || !isAllowedAlpha(middleFour[1]) || !isAllowedAlpha(middleFour[2]) || !isAllowedAlpha(middleFour[3]))
    {
      isValid = false;
    // check last three elements of ID
    }else if(!isalnum(lastThree[0]) || !isalnum(lastThree[1])  || !isalnum(lastThree[2]))
    {
      isValid = false;
    }
      }
  if(isValid == false)
  {
    errMess += ERRORS[errorMessIndex];
  }
          
  return errMess == "";
}


//check that id is of accepted length 
bool isIdLength(Car temp, string &errMess)
{
  const int LENGTH=9;
  int errorMessIndex = 1;
  if(temp.ID.length() != LENGTH)
     {
       errMess+= ERRORS[errorMessIndex];
     }
  return errMess == "";
}



// function that checks ID has valid alpha characters 
bool isAllowedAlpha(char a)
{
 bool passedAlpha = true;
   if(a > 'S' || a == 'O' || a < 'A')
   {
     passedAlpha = false;  
   }
  
  return passedAlpha;
}



//function that checks car model is correct format
bool isValidModel(Car temp, string &errMess)
{
  bool modelValid = true;
  int errorMessIndex = 2;
  string firstLetter;
  firstLetter = (temp.model.substr(0,1));
  char stringChars[temp.model.length()];

  if(!isalpha(firstLetter[0]))
  {
    modelValid = false;
  }

  for(int i = 0; i < temp.model.length(); i++)
    {
      stringChars[i] = temp.model[i];
    }
  for(int i = 0; i < temp.model.length() && modelValid; i++)
    {
      if(!isalnum(stringChars[i]) && stringChars[i] != '_')
      {
        modelValid = false; 
      }
     
    }
   if(!modelValid)
      {
        errMess+= ERRORS[errorMessIndex];
      }
  
  return errMess == "";
}



// Check if quantity is greater than 0 
bool isValidQuantity(Car temp, string &errMess)
{
  bool quantityValid = true;
  int errorMessIndex = 3;
  if(temp.quantity <= 0)
  {
    quantityValid = false;
    errMess+= ERRORS[errorMessIndex];
  }
  
  return errMess == "";
}



// Check if price is greater than 0
bool isValidPrice(Car temp, string &errMess)
{
  bool priceValid = true;
  int errorMessIndex = 4;
  if(temp.price <= 0)
  {
    priceValid = false;
    errMess+= ERRORS[errorMessIndex];
  }
  return errMess == "";
}


// Print the carInventory array and total number of cars in inventory
void PrintValid(Car CarInventory[], int inventoryCount)
{
  int i;
  cout << "\n***INVENTORY***\n" 
       << setfill('-') << setw(52) << "-" << setfill(' ') << "\n"  
       << left << setw(11) << "ID" << " | " 
       << left << setw(10) << "Model" << " | " 
       << left << setw(13) << "Qty " << " | " 
       << right << setw(9) << "Price" << "\n"
       << setfill('-') << setw(52) << "-" << setfill(' ') << endl;

  for(i=0; i<inventoryCount; i++)
    {
      
      cout << left << setw(11) <<  CarInventory[i].ID << " | " 
           << left << setw(10) <<CarInventory[i].model << " | " 
           << left << setw(13) << CarInventory[i].quantity << " | "
           << right << setw(9) << fixed << setprecision(2) << CarInventory[i].price << endl; 
    }

  cout << setfill('-') << setw(52) << "-" << setfill(' ') << endl;
}


// Print the carInventory array and total number of cars in inventory
void PrintErrors(int errorCount)
{
  int i;  
  cout << "\n***Errors***\n" 
       << setfill('-') << setw(70) << "-" << setfill(' ') << endl;

  string errorString;
  ifstream errorInput("errorOutput.txt");
  if(!errorInput.is_open()) 
    {
      cout << "ERROR. Could not open Error file." << endl;
      exit(EXIT_FAILURE);
    }
 if(errorInput.peek() == EOF)
    {
        cout << "ERROR. OUTPUT FILE IS EMPTY." << endl;
        exit(EXIT_FAILURE);
    }

  while(!errorInput.eof())
    {
      getline(errorInput, errorString);
      cout << errorString << endl;
    }
  
  errorInput.close();   
}



/*test data
82hefn696 chEvy 30 66898.93
16abce345 chevy 711 39598.00
8979123 Ford 10 5000.00 
89ZZZ136 Toyota 14 54000.99  
AAzbcd123 Honda 12 10000.75  
A4AFAC123 Ford 12 1000.00  
12yFAB123 Ford 12 1000.00 
13hgcd456 Mercedes 12 55000.00  
-3abcd454 Mercedes 24 55000.00 
12AQ!D143 Ho_nda 2 12000.00
59AODE999 Hon!!_a 12 10000.00
98scGV483 1BMW  22  1000000.00
22aged456 Tesl_a -33 76000.00
13abde336 Bercedes 0 50000.00
49peer987 Porsche 45 -100000.00
179855454 Mazda 986 0.00
13iunD123 $$d_a 172 0000.00
97reed333 Cadillac 9 78999.98
55heep245 Ford 44 89324.22


***MENU***

1. Print Inventory
2. Print Errors
3. Quit

Enter Choice: 1

***INVENTORY***
----------------------------------------------------
ID          | Model      | Qty           |     Price
----------------------------------------------------
82hefn696   | chEvy      | 30            |  66898.93
16abce345   | chevy      | 711           |  39598.00
13hgcd456   | Mercedes   | 12            |  55000.00
97reed333   | Cadillac   | 9             |  78999.98
55heep245   | Ford       | 44            |  89324.22
----------------------------------------------------


***MENU***

1. Print Inventory
2. Print Errors
3. Quit

Enter Choice: 2

***Errors***
----------------------------------------------------
8979123 Ford 10 5000.00  Invalid Id Length. Invalid ID format.

89ZZZ136 Toyota 14 54000.99  Invalid Id Length. Invalid ID format.

AAzbcd123 Honda 12 10000.75  Invalid ID format.

A4AFAC123 Ford 12 1000.00  Invalid ID format.

12yFAB123 Ford 12 1000.00  Invalid ID format.

-3abcd454 Mercedes 24 55000.00  Invalid ID format.

12AQ!D143 Ho_nda 2 12000.00  Invalid ID format.

59AODE999 Hon!!_a 12 10000.00  Invalid ID format. Invalid Model Format.

98scGV483 1BMW 22 1000000.00  Invalid ID format. Invalid Model Format.

22aged456 Tesl_a -33 76000.00  Invalid Quantity.

13abde336 Bercedes 0 50000.00  Invalid Quantity.

49peer987 Porsche 45 -100000.00  Invalid Price.

179855454 Mazda 986 0.00  Invalid ID format. Invalid Price.

13iunD123 $$d_a 172 0.00  Invalid ID format. Invalid Model Format. Invalid Price.



***MENU***

1. Print Inventory
2. Print Errors
3. Quit

Enter Choice: a

Invalid selection. Please choose again.



***MENU***

1. Print Inventory
2. Print Errors
3. Quit

Enter Choice: afdfaf

Invalid selection. Please choose again.



***MENU***

1. Print Inventory
2. Print Errors
3. Quit

Enter Choice: 34343

Invalid selection. Please choose again.



***MENU***

1. Print Inventory
2. Print Errors
3. Quit

Enter Choice: 3

Program Terminated
  */