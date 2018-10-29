/****************************************************************
** Author: Adam Walker
** Date: June 10 2018
** Description: Implementation file for filmRoom class.
****************************************************************/

#include "filmRoom.hpp"


// Constructor for filmRoom, a child class of space
// This class represents a film room where coaches would watch video of opposing teams prior to a game.
filmRoom::filmRoom(std::string nameInput, std::string locationInput, std::string teamInput, int yearInput) 
   : Space() {

	north=nullptr;
	south=nullptr;
	east=nullptr;
	west=nullptr;
	
	name=nameInput;
	type="coach";
        visited=false;
	location=locationInput;
	team=teamInput;
	year=yearInput;
}


// Introduces the space to the player on arrival
void filmRoom::announceLocation() {

  cout << endl;
  cout << endl;
  cout << "The place is " << location << endl;
  cout << "The year is " << year << endl;
  cout << "In a dimly limit film room, " << name << " is watching game tape in preparation for the next opponent." << endl;
  cout << endl;
  cout << endl;

  cout << "Coach looks up at you angrily. Time traveler or not, he has a game to get ready for and you are a nuisance.\n"
          "Stammering slightly, you explain your predicament. You offer to show your knowledge of basketball by answering\n"
	  "one of Coach's questions. Sighing, he asks you this...." << endl;
  cout << endl;
  cout << endl;

};


// Coaches don't like mistakes, so player only gets one chance to get their question right
bool filmRoom::askQuestion() {
  
  // Get player's response
  int playerAnswer=menu(questionText,1,4);

  if( playerAnswer==questionAnswer) {

    cout << endl;
    cout << endl;
    cout << "CORRECT!!!" << endl;
    cout << "Coach is satisfied you know a little about basketball. He agrees to run your team." << endl;

    return true;

  } else {

    cout << endl;
    cout << endl;
    cout << "Oops! Wrong answer. And unfortunately coaches don't tolerate mistakes.\n"
            "You're going to have to look elsewhere for someone to run this team..." << endl;

    return false;

  }

  return true;

};
