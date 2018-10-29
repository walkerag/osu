/****************************************************************
** Author: Adam Walker
** Date: June 10 2018
** Description: Implementation file for courtsideSeats class.
****************************************************************/

#include "courtsideSeats.hpp"

// Constructor
// Child of abstract Space class
courtsideSeats::courtsideSeats(std::string nameInput, std::string locationInput, std::string teamInput, int yearInput) 
   : Space() {

	north=nullptr;
	south=nullptr;
	east=nullptr;
	west=nullptr;
	
	name=nameInput;
	type="fan";
        visited=false;
	location=locationInput;
	team=teamInput;
	year=yearInput;

}


// Function to introduce the space when a player first arrives
void courtsideSeats::announceLocation() {

  cout << endl;
  cout << endl;
  cout << "The city is " << location << endl;
  cout << "The year is " << year << endl;
  cout << "The location is courtside during a " << team << " night game. This building is LOUD." << endl;
  cout << endl;
  cout << endl;

  cout << "As you sneak along the court, " <<  name << " is far too busy screaming at an official to notice you." << endl;
  cout << endl;
  cout << "Patiently waiting for a timeout, you eventually introduce yourself. Mixing in some flattery of " << name << "'s latest work, you manage to ingratiate yourself.\n"
	  "You explain your mission, and in particular the need for fervent fan support to help sway those hardnosed intergalactic refs.\n"
          << name << " looks intrigued, but first he wants you to prove you really are a true fan of his work..." << endl;
  cout << endl;
  cout << endl;

};


// Function where courtside fan asks the player a question
// Players get 3 chances to answer correctly
bool courtsideSeats::askQuestion() {
  
  // Get player's response
  int playerAnswer=menu(questionText,1,4);

  // Three chances to get it right
  if( playerAnswer==questionAnswer) {

    cout << endl;
    cout << endl;
    cout << "CORRECT!!!" << endl;
    cout << name << " knows you are a true fan. He might even give you a shoutout on social media. Either way, he follows you out of the arena and into the teleporter." << endl;

    return true;

  } else {

    cout << endl;
    cout << endl;
    cout << "Er...that was not the right answer! Fortunately the " << team << " are winning, so " << name << " gives you another shot:" << endl;
    cout << endl;
    playerAnswer=menu(questionText,1,4);
    
    if( playerAnswer==questionAnswer) {

	cout << endl;
	cout << endl;
	cout << "RIGHT ANSWER! You got it. More fan support has just been added." << endl;
	return true;

    } else {

    	cout << endl;
    	cout << endl;
	cout << "OK that was wrong too. Any self-respecting coach or player would be done with you by now, but for some reason " << name << " gives you a third and final chance:" << endl;
        cout << endl;
    	playerAnswer=menu(questionText,1,4);
    
    	if( playerAnswer==questionAnswer) {

		cout << endl;
		cout << endl;
		cout << "PHEW. Third time was the charm. " << name << " joins the squad." << endl;
		return true;

	} else {

		cout << endl;
		cout << endl;
		cout << "Yeah...that wasn't it. " << name << " has had enough. Better luck next time." << endl;
		return false;

	}
    
    }

  }

  return true;

};
