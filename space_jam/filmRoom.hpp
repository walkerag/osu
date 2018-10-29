/****************************************************************
** Author: Adam Walker
** Date: June 10 2018
** Description: Header file for filmRoom class.
****************************************************************/

#ifndef FILMROOM_HPP
#define FILMROOM_HPP

#include "Space.hpp"

// Child class of Space
class filmRoom : public Space{

public:
   // Constructor, takes strings for name, location, and team, as well as an int for year
   filmRoom(std::string nameInput, std::string locationInput, std::string teamInput, int yearInput); 
   // Function to introduce space when a player arrives
   void announceLocation();
   // Function to ask a question
   bool askQuestion();
   ~filmRoom() {};

};

#endif
