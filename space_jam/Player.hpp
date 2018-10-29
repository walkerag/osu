/****************************************************************
** Author: Adam Walker
** Date: June 10 2018
** Description: Header file for Player class.
Essentially used to track player's progression in game,
particularly which coach, players, and fans have been added to the roster
****************************************************************/

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Space.hpp"

class Player
{

private:

   // Hold player's current location
   Space* currentSpace;

   // Vectors to hold team roster components
   std::vector<Space*> coach;
   std::vector<Space*> player;
   std::vector<Space*> fan;

public:

   // Print the full roster
   void teamPrint();

   // Mutators
   void setCurrentSpace(Space* currentSpaceInput);
   void addCoach(Space* coachInput);
   void addPlayer(Space* playerInput);
   void addFan(Space* fanInput);

   // Accessors
   Space* getCurrentSpace();
   std::vector<Space*> getCoach();
   std::vector<Space*> getPlayer();
   std::vector<Space*> getFan();
   
   // Returns true is team has at least one coach and five players
   bool getRosterSet();

};

#endif
