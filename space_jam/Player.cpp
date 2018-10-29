/****************************************************************
** Author: Adam Walker
** Date: June 10 2018
** Description: Implementation file for Player class.
****************************************************************/

#include "Player.hpp"

// Print the player's "inventory", aka the coaches, players, and fans on the roster
void Player::teamPrint() {

	cout << endl;
	cout << endl;
	cout << endl;

	cout << "-----------------------------------------" << endl;
	cout << "Your roster so far..." << endl;

	// Print coaches
	if( coach.size()==0 ) {

		cout << "No coach on the roster" << endl;

	} else {

		for( int i=0; i<coach.size(); i++ ) {

	  	  cout << "COACH:" << endl;
		  cout << coach[i]->getName() << endl;

		}
	}


	// Print players
	if( player.size()==0 ) {

		cout << "No players on the roster" << endl;

	} else {

	  	cout << "PLAYERS:" << endl;
		for( int i=0; i<player.size(); i++ ) {

		  cout << player[i]->getName() << endl;

		}
	}
	

	// Print fans
	if( fan.size()==0 ) {

		cout << "No superfans on the roster" << endl;

	} else {

	  	cout << "SUPERFANS:" << endl;
		for( int i=0; i<fan.size(); i++ ) {

		  cout << fan[i]->getName() << endl;

		}
	}

	cout << "-----------------------------------------" << endl;
	cout << endl;
	cout << endl;
    	cout << "Hit the return key to continue" << endl;
    	system("read");

};


void Player::setCurrentSpace(Space* currentSpaceInput) {

  currentSpace=currentSpaceInput;

}


// Prompt the player to choose if a coach is already in place
// Logic is team only has one coach
void Player::addCoach(Space* coachInput) {

  if( coach.size() < 1 ) {

    coach.push_back(coachInput);

  } else {

    std::string question = "Wait! You already have a coach. Want to change regime?\n 1. Keep " + coach[0]->getName() + "\n 2. Replace him with " + coachInput->getName() + "\n";
    int replaceCoach = menu(question,1,2);
    
    if( replaceCoach==1 ) {

	cout << "Sounds good: " << coach[0]->getName() << " is still running your team" << endl;

    } else {

	cout << "Great! Your team has a new coach!" << endl;
	coach[0] = coachInput;

    }
    
  }


};


// Add player to team
// No limits on player totals
void Player::addPlayer(Space* playerInput) {

    player.push_back(playerInput);

};


// Add fan to team
// No limits on fans
void Player::addFan(Space* fanInput) {

    fan.push_back(fanInput);

};



// Accessors
Space* Player::getCurrentSpace() {

  return currentSpace;

}

std::vector<Space*> Player::getCoach() {

  return coach;

}

std::vector<Space*> Player::getPlayer() {

  return player;

}

std::vector<Space*> Player::getFan() {

  return fan;

}


// Check if team has one coach and at least five players
bool Player::getRosterSet() {

  if( player.size()>=5 && coach.size()>0 ) return true;

  return false;

}



