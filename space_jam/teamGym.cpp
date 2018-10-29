/****************************************************************
** Author: Adam Walker
** Date: June 10 2018
** Description: Implementation file for teamGym class.
****************************************************************/

#include "teamGym.hpp"

// Constructor
// Child class of Space
teamGym::teamGym(std::string nameInput, std::string locationInput, std::string teamInput, int yearInput) 
   : Space() {

	north=nullptr;
	south=nullptr;
	east=nullptr;
	west=nullptr;
	
	name=nameInput;
	type="player";
        visited=false;
	location=locationInput;
	team=teamInput;
	year=yearInput;
}

// Introduce space upon player's arrival
// Have 3 variants of introduction since this is most common space
void teamGym::announceLocation() {

  cout << endl;
  cout << endl;

  cout << "The city is " << location << endl;
  cout << "The year is " << year << endl;
  cout << "The location is the " << team << " practice facility" << endl;
  cout << endl;
  cout << endl;

  // Randomize the messages since players are most common characters in the game.
  int gymIntro = (rand() % 3);

  if( gymIntro==0 ) {

    cout << "Music is blaring as you enter the gym, and you quickly spot " << name << " dominating a practice game of 3 on 3." << endl;
    cout << endl;
    cout << "It takes a minute, but eventually you get him to give you the time of day. You explain the time travel scenario and the upcoming showdown against the MJ-liens\n"
	    "(actually not as hard as you expected), and ingratiate yourself with some 2018 stock picks. Finally, you volunteer to prove your worth by answering a question" << endl;
    cout << endl;
    cout << "After a quick brainstorming session with teammates, " << name << " asks you the following: " << endl;
    cout << endl;

  } else if ( gymIntro==1 ) {

    cout << "Finishing his workout with some jumpshots, " << name << " doesn't let a random time traveler break his concentration." << endl;
    cout << endl;
    cout << "Finally getting his attention, you explain your mission. Looking to prove yourself as a true basketball afficionado,\n"
	  "you ask for a question that will prove you aren't a fraud." << endl;
    cout << endl;
    cout << "After thinking for a second, " << name << " obliges: " << endl;
    cout << endl;


  } else if ( gymIntro==2 ) {

    cout << "Practice is over, and " << name << " is stretching as you sneak past some suspicious assistants into the gym." << endl;
    cout << endl;
    cout << "While " << name << " at first seems to think you're completely insane, the promise of intergalactic branding opportunities is alluring for any marketing savvy NBA players,\n"
	  "and he seems interested in joining the team. You put your best foot forward and ask for a question to prove that you are a true NBA fan." << endl;
    cout << endl;
    cout << "Pondering the offer, " << name << " poses you this: " << endl;
    cout << endl;


  }

};


// Have NBA player ask a question
// Player gets 2 tries to get it right. This is a balance between coach questions (1 try) and fan questions (3 tries)
bool teamGym::askQuestion() {
  
  // Get player's response
  int playerAnswer=menu(questionText,1,4);

  if( playerAnswer==questionAnswer) {

    cout << endl;
    cout << endl;
    cout << "CORRECT!!!" << endl;
    cout << name << " has convinced himself you know *something* about basketball. He grabs his gear and hops into the teleporter." << endl;

    return true;

  } else {

    cout << endl;
    cout << endl;
    cout << "Urgh! Wrong answer. You caught a break though - players are a little easier on mistakes than coaches, so " << name << " gives you a second chance to answer correctly:" << endl;
    cout << endl;
    playerAnswer=menu(questionText,1,4);
    
    if( playerAnswer==questionAnswer) {

	cout << endl;
	cout << endl;
	cout << "PHEW. Second time lucky! You've added a new player to the squad" << endl;
	return true;

    } else {

	cout << endl;
	cout << endl;
	cout << "Wow. Wrong again. " << name << " shakes his head and jogs off to call security on you. Time to leave!" << endl;
	return false;
    
    }

  }

  return true;

};
