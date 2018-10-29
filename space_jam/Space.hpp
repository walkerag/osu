/****************************************************************
** Author: Adam Walker
** Date: June 10 2018
** Description: Header file for Space class.
Abstract class, parent to teamGym, filmRoom, and courtsideSeats
****************************************************************/

#ifndef SPACE_HPP
#define SPACE_HPP

#include "menu.hpp"
#include <string>
#include <iostream>
#include <random>
#include <vector>

using std::string;
using std::cout;
using std::endl;

class Space
{

// Various attributes common to all spaces
protected:

   // Pointers indicating which spaces are nearby
   Space* north;
   Space* south;
   Space* east;
   Space* west;

   // Attributes like team name, city location etc.
   int year;
   std::string name;
   std::string location;
   std::string team;
   std::string type;
   std::string questionText;
   int questionAnswer;

   // Indicates whether player has already visited Space
   // Stops same question being asked many times
   bool visited;

public:

   // Virtual functions for introducing Space and posing question to player
   virtual void announceLocation() = 0;
   virtual bool askQuestion() = 0;

   // Lets player select a new Space from current location
   Space* locationOptions();

   // Mutators
   void setNorth(Space* northInput);
   void setSouth(Space* southInput);
   void setEast(Space* eastInput);
   void setWest(Space* westInput);
   void setVisited(bool visitedInput);
   void setQuestionText(std::string questionTextInput);
   void setQuestionAnswer(int questionAnswerInput);

   // Accessors
   std::string getLocation();
   std::string getName();
   std::string getType();
   std::string getTeam();
   bool getVisited();

   // Destructor
   virtual ~Space() {};

};

#endif
