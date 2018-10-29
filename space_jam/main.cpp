/******************************************************************
** Author: Adam Walker
** Date: June 10 2018
** Description: Main function for final project
This project is a simple game called "Space Jam: C++ Edition"
The player must try to assemble a team of NBA players, coaches,
and superfans in order to take on an extra-terrestrial squad known as the "MJ-liens".
In order to add an NBA coach, player, or fan to the roster, the player
must correctly answer a trivia question about said person.
******************************************************************/

#include "Game.hpp"
#include "menu.hpp"

int main() {

	srand(time(NULL));
	Game spaceJam;
	spaceJam.playGame();

	return 0;
}
