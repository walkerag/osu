/****************************************************************
** Author: Adam Walker
** Date: April 6 2018
** Description: Header file for menu functions.
****************************************************************/

#ifndef MENU_HPP
#define MENU_HPP

#include <string>
using std::string;

// Takes message printed to user, along with minimum and maximum values for user input
int menu(std::string message, int min, int max);
// Takes message printed to user. No integer limits.
int menu(std::string message);
// Takes message printed to user, as well as char variables indicating true or false values.
bool menu(std::string message, char t, char f);

#endif
