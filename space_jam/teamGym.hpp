/****************************************************************
** Author: Adam Walker
** Date: June 10 2018
** Description: Header file for teamGym class.
****************************************************************/

#ifndef TEAMGYM_HPP
#define TEAMGYM_HPP

#include "Space.hpp"

// Child class of Space
class teamGym : public Space{

public:
   // Constructor, takes strings for name, location, and team, as well as an int for year
   teamGym(std::string nameInput, std::string locationInput, std::string teamInput, int yearInput); 
   // Function to introduce space when a player arrives
   bool askQuestion();
   // Function to ask a question
   void announceLocation();
   ~teamGym() { };

};

#endif
