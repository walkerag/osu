/****************************************************************
** Author: Adam Walker
** Date: June 10 2018
** Description: Header file for courtsideSeats class.
****************************************************************/

#ifndef COURTSIDE_HPP
#define COURTSIDE_HPP

#include "Space.hpp"

// Child class of Space
class courtsideSeats : public Space{

public:
   // Constructor, takes strings for name, location, and team, as well as an int for year
   courtsideSeats(std::string nameInput, std::string locationInput, std::string teamInput, int yearInput); 
   // Function to introduce space when a player arrives
   void announceLocation();
   // Function to ask a question
   bool askQuestion();
   ~courtsideSeats() {};

};

#endif
