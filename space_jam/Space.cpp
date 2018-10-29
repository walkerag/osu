/****************************************************************
** Author: Adam Walker
** Date: June 10 2018
** Description: Implementation file for Space class.
****************************************************************/

#include "Space.hpp"

// Virtual functions, defined in child classes
// bool Space::askQuestion() = 0;
// void Space::announceLocation() {};

// Prompts the user to choose next location based on nearby spaces
Space* Space::locationOptions() {

	cout << endl;
	cout << endl;
	cout << endl;

	bool locationMove=false;
	while( !locationMove ) {

	  cout << "Choose your new location: " << endl;

  	  if(north) cout << "1. Go North to " << north->getLocation() << endl;
	  else cout << "1. No available location North" << endl;

	  if(east) cout << "2. Go East to " << east->getLocation() << endl;
	  else cout << "2. No available location East" << endl;

	  if(south) cout << "3. Go South to " << south->getLocation() << endl;
	  else cout << "3. No available location South" << endl;

	  if(west) cout << "4. Go West to " << west->getLocation() << endl;
	  else cout << "4. No available location West" << endl;
	
	  int locationChoice = menu("",1,4);

	  if( locationChoice==1 && north ) return north;
  	  if( locationChoice==2 && east ) return east;
	  if( locationChoice==3 && south ) return south;
	  if( locationChoice==4 && west ) return west;
	
	  cout << "There's no NBA city in that direction!" << endl;
	  cout << endl;
	  cout << endl;

	}

      return nullptr;	

};


// Mutators
void Space::setNorth(Space* northInput) {

  north=northInput;

};

void Space::setSouth(Space* southInput) {

  south=southInput;

};

void Space::setEast(Space* eastInput) {

  east=eastInput;

}

void Space::setWest(Space* westInput) {

  west=westInput;

};

void Space::setVisited(bool visitedInput) {

  visited=visitedInput;

}

void Space::setQuestionText(std::string questionTextInput) {

  questionText=questionTextInput;

}

void Space::setQuestionAnswer(int questionAnswerInput) {

  questionAnswer=questionAnswerInput;

}


// Accessors
std::string Space::getLocation() {

  return location;

}

std::string Space::getName() {

  return name;

}

std::string Space::getType() {

  return type;

}

std::string Space::getTeam() {

  return team;

}

bool Space::getVisited() {

  return visited;

}
