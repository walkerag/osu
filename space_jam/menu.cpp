/***************************************************************** 
** Author: Adam Walker
** Date: April 6 2018
** Description: Function implementation file for menu.
Overloaded function for prompting user input and validating it.
******************************************************************/ 
#include "menu.hpp"
#include <iostream>
#include <limits>

using std::cout;
using std::cin;
using std::endl;
using std::string;

// Menu to accept integer with min and max limits
int menu(std::string message, int min, int max) {

  int input;

  // Ask for input
  cout << message << endl;
  cin >> input;
  //cout << "Input: " << input << endl; 
 
  // Validate input
  while(std::cin.fail() | (input<min) | (input>max) ) {

  cout << "Incorrect input received. Please try again:" << endl;
  std::cin.clear();
  std::cin.ignore(256,'\n');	
  cin >> input;

  }

  // Clear buffer
  std::cin.clear();
  std::cin.ignore(256,'\n');	
  
  return input;

}


// Menu to accept integer with no limits
int menu(std::string message) {

  int input;

  // Ask for input
  cout << message << endl;
  cin >> input;
  
  // Validate input
  while(std::cin.fail()) {

  cout << "Incorrect input received. Please try again:" << endl;
  std::cin.clear();
  std::cin.ignore(256,'\n');	
  cin >> input;

  }

  std::cin.clear();
  std::cin.ignore(256,'\n');	
  
  return input;

}


// Menu to accept char, return boolean value
bool menu(std::string message, char t, char f) {

  char input;

  // Ask for input
  cout << message << endl;
  cin >> input;
  
  bool valid=false;
  while(!valid) {
    
   if(input==f) {
	
        std::cin.clear();
        std::cin.ignore(256,'\n');	
	return false;

   } else if (input==t) {
  	
        std::cin.clear();
        std::cin.ignore(256,'\n');	
	return true;
   
   } else {

        std::cin.clear();
  	std::cin.ignore(256,'\n');	
        cout << "Incorrect input received. Please try again:" << endl;
        cin >> input;
   }
  
  }

  return false;

}
