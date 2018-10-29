/***************************************************************** 
** Author: Adam Walker
** Date: June 10 2018
** Description: Implementation file for Game class
******************************************************************/ 

#include "Game.hpp"

// 14 turns before championship game automatically starts
Game::Game() {

	cout << "Made a game" << endl;
	turns=14;

};


// Runs round of game
// Player visits a location, meets a famous NBA personality
// If the player answers a question correctly from the NBA personality,
// that personality agrees to join the Earth squad to face the MJ-liens
void Game::playGame() {

	// Intro script
	gameIntro();

	bool keepPlaying=menu("Do you accept this challenge?\n"
			      "1. Yes! Like Lance Stephenson, I was Born Ready.\n"
			      "2. Nah...not really feeling it.\n",'1','2'); 		

	if(keepPlaying) {
	
		// Setup the game's spaces
		setGame();

		clearScreen();

		// Initial map print
		printMap(true);
		cout << "The aliens have 'helpfully' deposited you and your teleporter in the middle of the country." << endl;
				
		int startingChoice = menu("Choose your first city. You never know who you might run into...\n"
					   "1. Go North to Chicago\n"
					   "2. Go East to Cleveland\n"
					   "3. Go South to Oklahoma City\n"
					   "4. Go West to Utah",1,4);
		
		// Go to player's first location
		if(startingChoice==1) ourHero.setCurrentSpace(basketballMap[0]);
		if(startingChoice==2) ourHero.setCurrentSpace(basketballMap[2]);
		if(startingChoice==3) ourHero.setCurrentSpace(basketballMap[14]);
		if(startingChoice==4) ourHero.setCurrentSpace(basketballMap[10]);

		// Play the turn
		playTurn();
		
    		clearScreen();	
		
		int menuChoice;
	
		// Keep running until out of turns or user decides to jump to final game
		bool showdown = false;
		while( turns>0 && !showdown ) {

		  printMap();
		  menuChoice=showMenu();

		  if( menuChoice == 1 ) {

			// Change location
			Space* newLocation = ourHero.getCurrentSpace()->locationOptions();
			ourHero.setCurrentSpace(newLocation);

			// Play the turn
			playTurn();

			turns--;

		  } else if ( menuChoice==2 ) {

			// Print roster
			clearScreen();
			ourHero.teamPrint();

		  } else {

		  	// Check roster is ready
		  	// Make sure there are at least 5 players
		  	// If so, go on to showdown
   			bool rosterCheck = ourHero.getRosterSet();

			if(rosterCheck) {

				showdown = true;
				cout << endl;
				cout << endl;
				cout << endl;
				cout << "You've traveled across North America (and time) to put together an exemplary squad.\n"
					"Let's see if they have what it takes to win!" << endl;

			} else {

				cout << endl;
  				clearScreen();
				cout << endl;
				cout << "WAIT! Wob and the rest of NBA Twitter do NOT think this roster is ready." << endl;
				cout << "Remember: the MJ-lien squad is tough, and your team is undermanned right now.\n"
					"Make sure you have a coach and at least 5 players before you challenge the MJ-liens..." << endl;
				cout << endl;
				cout << endl;
    				cout << "Hit the return key to continue" << endl;
    				system("read");
    				clearScreen();	

			}

		  }

		}

		// Message to announce championship game if player runs out of turns...
		if( turns==0 ) {

		  clearScreen();
		  cout << endl;
		  cout << "The teleporter is out of dark matter! Ready or not, it's time to take on the MJ-liens...." << endl;
		  cout << endl;
		  cout << endl;

		}

		// Plays out the final game
		playChampionship();


	} else {

		clearScreen();	
		cout << endl;
		cout << endl;
		cout << "Kevin Durant announces that his 'next chapter' will be to join the MJ-lien squad.\n" 
			"KD and his new extra-terrestrial teammates are utterly unstoppable,\n"
			"winning the INTERGALACTIC BASKETBALL CHAMPIONSHIP by over 50 points." << endl;
		cout << endl;
		cout << endl;
		cout << "GAME OVER" << endl;
		cout << endl;
		cout << endl;
	}


};



// Plays out a single turn
void Game::playTurn() {
 
  // Retrieve player's current location
  Space* turnLocation = ourHero.getCurrentSpace();

  // Check if player has already visited location
  bool visitedStatus = turnLocation->getVisited();
  
  clearScreen();

  if ( visitedStatus ) {

    cout << endl;
    cout << endl;
    cout << "You've been here already! Let's go somewhere new " << endl;
    cout << endl;
    cout << endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));	

  } else {

    // Space introduction
    turnLocation->announceLocation();

    // Get type of location
    std::string type = turnLocation->getType();
  
    // Ask question
    bool addRoster = turnLocation->askQuestion();

    // Update visited status
    turnLocation->setVisited(true);

    // Add to roster if player answered question correctly
    // Can only have one coach
    if( type=="coach" && addRoster ) {
	 
	ourHero.addCoach(turnLocation);

    } else if ( type=="player" && addRoster ) {

	ourHero.addPlayer(turnLocation);

    } else if (type=="fan" && addRoster ) {

	ourHero.addFan(turnLocation);

    }

    cout << endl;
    cout << endl;
    cout << "Powering up teleporter for next trip..." << endl;
    cout << endl;
    cout << endl;
    // Source: https://stackoverflow.com/questions/1449324/how-to-simulate-press-any-key-to-continue
    cout << "Hit the return key to continue" << endl;
    system("read");
    clearScreen();	

  }  

}



// Print the NBA map
// Use o or X to mark cities depending on player's location
void Game::printMap(bool firstTurnInput) {

std::string currentLocation="None";
if (!firstTurnInput) currentLocation = ourHero.getCurrentSpace()->getLocation();

// Generated via http://sporkforge.com/imaging/ascii.php
cout << "                                                    WHERE TO NEXT?                                                 " << endl;
cout << "                                                                                                                   " << endl; 
cout << "    .P%.,J`''*ww,,_                                                                                     ,w=w       " << endl; 
cout << "     } :1[         `''*==ww,,,__                                                                        [   %      " << endl; 
if(currentLocation=="Toronto") cout << "     [ =QL                     ```""*****====wwww.,,,,ww=4''e__                            X Toronto    L    *w    " << endl; 
else cout << "     [ =QL                     ```""*****====wwww.,,,,ww=4''e__                            o Toronto    L    *w    " << endl; 
if(currentLocation=="Seattle") cout << "     @k   X Seattle                                           ''*==w..                                ,/.     ,F   " << endl;
else cout << "     @k   o Seattle                                           ''*==w..                                ,/.     ,F   " << endl;
cout << "    L                                                          ,zF  ,,   _,,                  ,,w==*`      ^       " << endl;
if(currentLocation=="Portland") cout << "   yF  X Portland                                              ''*`'*` ``'` ,JZL               y`         ,/       " << endl;
else cout << "   yF  o Portland                                              ''*`'*` ``'` ,JZL               y`         ,/       " << endl;
cout << "  #`                                                                    ,F ,/  *.            <L          F,,       " << endl;
if(currentLocation=="Boston") cout << " JL                                                                     PE F.   X,,       _,,#` Boston X '.Z]      " << endl;
else cout << " JL                                                                     PE F.   X,,       _,,#` Boston o '.Z]      " << endl;
if(currentLocation=="New York") cout << " /                                                                       LJL    *  L     $.   New York X ,/`       " << endl;
else cout << " /                                                                       LJL    *  L     $.   New York o ,/`       " << endl;
if(currentLocation=="Milwaukee") cout << "/                                                         Milwaukee X   JL'L      yL   ,/`           ,==f`         " << endl;
else cout << "/                                                         Milwaukee o   JL'L      yL   ,/`           ,==f`         " << endl;
if(currentLocation=="Chicago") cout << "L                                                             Chicago X JL.F     -L,w/`              Z5=^          " << endl;
else cout << "L                                                             Chicago o JL.F     -L,w/`              Z5=^          " << endl;
if(currentLocation=="Cleveland") cout << "{                                                                        ''F         X Cleveland      {            " << endl;
else cout << "{                                                                        ''F         o Cleveland      {            " << endl;
cout << " L                                                                                                 pwL             " << endl;
cout << " [                                                                                             JL }.               " << endl;
if(firstTurnInput) cout << " [                                                        X YOU ARE HERE                       JL }.               " << endl;
else cout << " [                                                                                               ''D$.             " << endl;
if(currentLocation=="Oakland") cout << " 4. X Oakland                                                                                     `4.,             " << endl;
else cout << " 4. o Oakland                                                                                     `4.,             " << endl;
if(currentLocation=="Utah") cout << "  JL                                X Utah                                                          y*             " << endl;
else cout << "  JL                                o Utah                                                          y*             " << endl;
cout << "   {.                                                                                             ;NL              " << endl;
if(currentLocation=="Oklahoma City") cout << "    `*%,                                                 X Oklahoma City                         y`                " << endl;
else cout << "    `*%,                                                 o Oklahoma City                         y`                " << endl;
if(currentLocation=="Los Angeles") cout << "        '. X Los Angeles                                                                        /                  " << endl;
else cout << "        '. o Los Angeles                                                                        /                  " << endl;
if(currentLocation=="Atlanta") cout << "          |                                                                  X Atlanta        yF                   " << endl;
else cout << "          |                                                                  o Atlanta        yF                   " << endl;
cout << "          *=+..,,                                                                           y*                     " << endl;
if(currentLocation=="Phoenix") cout << "                `'w,      X Phoenix                                                        /                       " << endl;
else cout << "                `'w,      o Phoenix                                                        /                       " << endl;
if(currentLocation=="Dallas") cout << "                    'w,       ,,,,                    X Dallas                             L                       " << endl;
else cout << "                    'w,       ,,,,                    o Dallas                             L                       " << endl;
if(currentLocation=="Houston") cout << "                       ````````  '%,                      X Houston                        L                       " << endl;
else cout << "                       ````````  '%,                      o Houston                        L                       " << endl;
cout << "                                   'L                                    ,=4%w===w,,,=<,    L                      " << endl;
if(currentLocation=="San Antonio") cout << "                                     %.   ,_      X San Antonio,,,_ ,   j2             '.    %                     " << endl;
else cout << "                                     %.   ,_      o San Antonio,,,_ ,   j2             '.    %                     " << endl;
if(currentLocation=="Orlando") cout << "                                      'k,#` `%.            ,/`       ``" "*               JL  X Orlando              " << endl;
else cout << "                                      'k,#` `%.            ,/`       ``" "*               JL  o Orlando              " << endl;
cout << "                                              JL        ,=^                              k.    l                   " << endl;
cout << "                                               'k     ,F                                  '%,   L                  " << endl;
if(currentLocation=="Miami") cout << "                                                 %.  -F                                      l X Miami              " << endl;
else cout << "                                                 %.  -F                                      l o Miami              " << endl;
cout << "                                                  '%.,3.                                      =*`                  " << endl;
cout << endl;
cout << endl;

}


// Menu function
int Game::showMenu() {

	cout << "---------------------------------------------" << endl;
        cout << "You are currently in " << (ourHero.getCurrentSpace())->getLocation() << endl;
	cout << "Total hops left: " << turns << endl;
	cout << "1. Change location" << endl;
	cout << "2. View roster" << endl;
	cout << "3. Play the INTERGALACTIC BASKETBALL CHAMPIONSHIP" << endl;
	cout << "---------------------------------------------" << endl;

	int choice=menu("Make your selection:",1,3);

	return choice;

}


// Create the spaces to be used
void Game::setGame() {

	// Each NBA city featured (I did 19, not quite the whole league) is one
	// of three spaces: teamGym, filmRoom, courtsideSeats
	// Team gyms contain an NBA player, film rooms a coach, and courtside seats a famous fan
	basketballMap.push_back(new teamGym("Michael Jordan","Chicago","Bulls",1997)); // 0 
	basketballMap.push_back(new filmRoom("Coach Red Auerbach","Boston","Celtics",1965)); // 1
	basketballMap.push_back(new teamGym("LeBron James","Cleveland","Cavs",2016)); // 2
	basketballMap.push_back(new teamGym("Giannis Antetokounmpo","Milwaukee","Bucks",2018)); // 3
	basketballMap.push_back(new courtsideSeats("Spike Lee","New York","Knicks",1994)); // 4
	basketballMap.push_back(new courtsideSeats("Drake","Toronto","Raptors",2017)); // 5
	basketballMap.push_back(new teamGym("Shawn Kemp","Seattle","Sonics",1992)); // 6
	basketballMap.push_back(new teamGym("Bill Walton","Portland","Blazers",1977)); // 7
	basketballMap.push_back(new teamGym("Steph Curry","Oakland","Warriors",2015)); // 8
	basketballMap.push_back(new teamGym("Magic Johnson","Los Angeles","Lakers",1989)); // 9
	basketballMap.push_back(new filmRoom("Coach Jerry Sloan","Utah","Jazz",1997)); // 10
	basketballMap.push_back(new teamGym("Charles Barkley","Phoenix","Suns",1992)); // 11
	basketballMap.push_back(new filmRoom("Coach Rick Carlisle","Dallas","Mavericks",2012)); // 12
	basketballMap.push_back(new teamGym("James Harden","Houston","Rockets",2018)); // 13
	basketballMap.push_back(new teamGym("Russell Westbrook","Oklahoma City","Thunder",2017)); // 14
	basketballMap.push_back(new courtsideSeats("2 Chainz","Atlanta","Hawks",2016)); // 15
	basketballMap.push_back(new teamGym("Penny Hardaway","Orlando","Magic",1994)); // 16
	basketballMap.push_back(new filmRoom("Coach Pat Riley","Miami","Heat",2006)); // 17
	basketballMap.push_back(new teamGym("Tim Duncan","San Antonio","Spurs",2008)); // 18

	// Set the location relationships	
	// Not all relationships are reciprocal in order to add some variance
	basketballMap[0]->setNorth(basketballMap[3]);
	basketballMap[0]->setEast(basketballMap[2]);
	basketballMap[0]->setSouth(basketballMap[14]);
	
	basketballMap[1]->setNorth(basketballMap[5]);
	basketballMap[1]->setSouth(basketballMap[4]);
	basketballMap[1]->setWest(basketballMap[3]);

	basketballMap[2]->setNorth(basketballMap[5]);
	basketballMap[2]->setEast(basketballMap[4]);
	basketballMap[2]->setSouth(basketballMap[15]);
	basketballMap[2]->setWest(basketballMap[0]);

	basketballMap[3]->setEast(basketballMap[2]);
	basketballMap[3]->setSouth(basketballMap[0]);
	basketballMap[3]->setWest(basketballMap[7]);

	basketballMap[4]->setNorth(basketballMap[1]);
	basketballMap[4]->setSouth(basketballMap[16]);
	basketballMap[4]->setWest(basketballMap[2]);
	
	basketballMap[5]->setSouth(basketballMap[1]);
	basketballMap[5]->setWest(basketballMap[6]);
	
	basketballMap[6]->setEast(basketballMap[5]);
	basketballMap[6]->setSouth(basketballMap[7]);

	basketballMap[7]->setNorth(basketballMap[6]);
	basketballMap[7]->setEast(basketballMap[3]);
	basketballMap[7]->setSouth(basketballMap[8]);

	basketballMap[8]->setNorth(basketballMap[7]);
	basketballMap[8]->setEast(basketballMap[10]);
	basketballMap[8]->setSouth(basketballMap[9]);
	
	basketballMap[9]->setNorth(basketballMap[8]);
	basketballMap[9]->setEast(basketballMap[14]);
	basketballMap[9]->setSouth(basketballMap[11]);
	
	basketballMap[10]->setNorth(basketballMap[0]);
	basketballMap[10]->setEast(basketballMap[14]);
	basketballMap[10]->setSouth(basketballMap[11]);
	basketballMap[10]->setWest(basketballMap[8]);

	basketballMap[11]->setNorth(basketballMap[10]);
	basketballMap[11]->setEast(basketballMap[12]);
	basketballMap[11]->setWest(basketballMap[9]);

	basketballMap[12]->setNorth(basketballMap[14]);
	basketballMap[12]->setEast(basketballMap[13]);
	basketballMap[12]->setSouth(basketballMap[18]);
	basketballMap[12]->setWest(basketballMap[11]);
	
	basketballMap[13]->setNorth(basketballMap[14]);
	basketballMap[13]->setEast(basketballMap[15]);
	basketballMap[13]->setSouth(basketballMap[18]);
	basketballMap[13]->setWest(basketballMap[12]);

	basketballMap[14]->setNorth(basketballMap[0]);
	basketballMap[14]->setEast(basketballMap[15]);
	basketballMap[14]->setSouth(basketballMap[12]);
	basketballMap[14]->setWest(basketballMap[10]);

	basketballMap[15]->setNorth(basketballMap[2]);
	basketballMap[15]->setSouth(basketballMap[16]);
	basketballMap[15]->setWest(basketballMap[14]);

	basketballMap[16]->setNorth(basketballMap[15]);
	basketballMap[16]->setSouth(basketballMap[17]);
	basketballMap[16]->setWest(basketballMap[18]);
	
	basketballMap[17]->setNorth(basketballMap[16]);
	basketballMap[17]->setWest(basketballMap[18]);

	basketballMap[18]->setNorth(basketballMap[12]);
	basketballMap[18]->setEast(basketballMap[16]);
	basketballMap[18]->setWest(basketballMap[11]);
	
	// Set question text and answers
	basketballMap[0]->setQuestionText("Here's a good one for you. What year did I win my first MVP?"
					  "\n 1. 1985 \n 2. 1988 \n 3. 1991 \n 4. 1994");
	basketballMap[0]->setQuestionAnswer(2);
	basketballMap[1]->setQuestionText("Who should already know this, but I'll ask you anyway. Who's our starting center?"
					  "\n 1. Jerry West \n 2. Bill Russell \n 3. Bob Cousy \n 4. Bill Sharman");
	basketballMap[1]->setQuestionAnswer(2);
	basketballMap[2]->setQuestionText("Who was drafted right after me in 2003? \n 1. Chris Bosh \n 2. Dwyane Wade \n 3. Andre Iguodala \n 4. Darko Milicic");
	basketballMap[2]->setQuestionAnswer(4);
	basketballMap[3]->setQuestionText("I live in Milwaukee now, but where am I from originally?"
					  "\n 1. France \n 2. Italy \n 3. Spain \n 4. Greece");
	basketballMap[3]->setQuestionAnswer(4);
	basketballMap[4]->setQuestionText("Which of my movies was nominated for best original screenplay at the Oscars?"
					  " \n 1. Malcolm X \n 2. Do The Right Thing \n 3. School Daze \n 4. She's Gotta Have it");
	basketballMap[4]->setQuestionAnswer(2);
	basketballMap[5]->setQuestionText("People think I'm embarrassed about my acting career. I'm not. What show was I on?"
				          " \n 1. Malcolm in the Middle \n 2. Degrassi: The Next Generation \n 3. Even Stevens \n 4. Hannah Montana");
	basketballMap[5]->setQuestionAnswer(2);
	basketballMap[6]->setQuestionText("My last dunk was ICONIC. Who was it over?"
				           "\n 1. Alton Lister \n 2. Charles Barkley \n 3. Shaquille O'Neal \n 4. Larry Bird");
	basketballMap[6]->setQuestionAnswer(1);
	basketballMap[7]->setQuestionText("Well, this is a delight! A time traveler from the future. Do you know where I went to college?"
					   "\n 1. USC \n 2. Georgetown \n 3. UCLA \n 4. Notre Dame ");
	basketballMap[7]->setQuestionAnswer(3);
	basketballMap[8]->setQuestionText("They call me and this guy the Splash Brothers. Who am I talking about?"
					  " \n 1. Draymond Green \n 2. Kevin Durant \n 3. Klay Thompson \n 4. Steve Kerr");
	basketballMap[8]->setQuestionAnswer(3);
	basketballMap[9]->setQuestionText("Easy question for you: what do they call our Lakers squads? "
					  "\n 1. Slowdown Lakers \n 2. Showtime Lakers \n 3. Snoozefest Lakers \n 4. Snailpace Lakers");
	basketballMap[9]->setQuestionAnswer(2);
	basketballMap[10]->setQuestionText("Here's a question for you: what play type am I most known for?"
					   "\n 1. Pindown \n 2. Flying V \n 3. Stagger screen \n 4. Pick and roll");
	basketballMap[10]->setQuestionAnswer(4);
	basketballMap[11]->setQuestionText("Listen man, you want me to play for your team? Answer me this: who coaches our team right now?"
					   " \n 1. Paul Westphal \n 2. Pat Riley \n 3. Steve Kerr \n 4. Phil Jackson");
	basketballMap[11]->setQuestionAnswer(1);
	basketballMap[12]->setQuestionText("I'm better known as a coach than a player, but which team did I win a title with in 1986?"
					   " \n 1. Boston Celtics \n 2. Houston Rockets \n 3. Los Angeles Lakers \n 4. Philadelphia 76ers");
	basketballMap[12]->setQuestionAnswer(1);
	basketballMap[13]->setQuestionText("Endorsements are everything in the NBA nowadays. What company did I sign a $200M contract with in 2015?"
					   " \n 1. Nike \n 2. Adidas \n 3. Puma \n 4. Big Baller Brand");
	basketballMap[13]->setQuestionAnswer(2);
	basketballMap[14]->setQuestionText("Everyone knows that I've averaged a triple double two seasons in a row. But who else has also done it for a full season?"
					   "\n 1. Michael Jordan \n 2. Oscar Robertson \n 3. Magic Johnson \n 4. Steve Nash");
	basketballMap[14]->setQuestionAnswer(2);
	basketballMap[15]->setQuestionText("I'm not the only successful rapper from Atlanta. In fact, there are quite a few. But which one of these artists is"
					   " NOT from the ATL? \n 1. OutKast \n 2. TI \n 3. Rae Sremmurd \n 4. Future");
	basketballMap[15]->setQuestionAnswer(3);
	basketballMap[16]->setQuestionText("Who was I traded for on draft night? \n 1. Shaquille O'Neal \n 2. Muggsy Bogues \n 3. Chris Webber  \n 4. Jason Kidd");
	basketballMap[16]->setQuestionAnswer(3);
	basketballMap[17]->setQuestionText("We won the title this year in Miami. How many championships have I now won as a head coach?"
					   "\n 1. 3 titles \n 2. 4 titles  \n 3. 5 titles \n 4. 6 titles");
	basketballMap[17]->setQuestionAnswer(3);
	basketballMap[18]->setQuestionText("Where did I play my college ball? \n 1. Duke \n 2. Clemson \n 3. North Carolina \n 4. Wake Forest");
	basketballMap[18]->setQuestionAnswer(4);

}



// Opening script to introduce the game
void Game::gameIntro() {


	clearScreen();
	cout << "---------------------------------" << endl;	
	cout << "WELCOME TO SPACE JAM: C++ EDITION " << endl;
	cout << "---------------------------------" << endl;	
	cout << endl;
	cout << endl;
	cout << "Scene: Planet Earth, 2018" << endl;
	cout << endl;
	cout << endl;
	cout << "The Golden State Warriors have just won their third NBA title in four years.\n"
		"But as viewers across the globe were following Steph, KD and co. through the playoffs,\n"
		"it appears some extraterrestrial beings had also been paying attention..." << endl;
	cout << endl;
	cout << endl;
	cout << "A few light years away, these 8 foot tall aliens first learned of the amazing game \n"
		"humans call basketball through an intercepted transmission of the classic movie 'Space Jam'." << endl;
	cout << endl;
	cout << endl;
	cout << "The 'MJ-liens', as they have since come to be known, were absolutely embarrassed by the Monstars's agonizing loss.\n"
		"Determined to defend the honor of extra-terrestrials everywhere, they have now come to Earth to contest the first ever INTERGALACTIC BASKETBALL CHAMPIONSHIP." << endl;
	cout << endl;
	cout << endl;
	cout << "Your mission, should you choose to accept it, is to pull together a squad capable of representing the human race against a team literally light years ahead.\n"
		"We'll need a coach, at least 5 players, and maybe even some EXTREMELY LOUD superfans (...how else will we get the referees on our side?)" << endl;	
	cout << endl;
	cout << endl;
	cout <<	"One more thing: to give us a fighting chance, the MJ-liens have lent you a time traveling teleporter.\n"
		"Use it to travel through NBA history and convince legends of the game to sign-up.\n"
		"But be careful...the teleporter's dark matter fuel source will soon need to be replenished, and the MJ-liens only took two weeks vacation for this trip." << endl;
	cout << endl;
	cout << endl;
	cout << endl;
}



// Plays the final game
// Two possible endings based on completeness of roster
void Game::playChampionship() {

	cout << endl;
	cout << endl;
	cout << endl;
        // https://stackoverflow.com/questions/4184468/sleep-for-milliseconds
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));	
	cout << "GET" << endl;	
	std::this_thread::sleep_for(std::chrono::milliseconds(300));	
	cout << "READY" << endl;	
	std::this_thread::sleep_for(std::chrono::milliseconds(300));	
	cout << "THE" << endl;	
	std::this_thread::sleep_for(std::chrono::milliseconds(300));	
	cout << "GAME" << endl;	
	std::this_thread::sleep_for(std::chrono::milliseconds(300));	
	cout << "IS" << endl;	
	std::this_thread::sleep_for(std::chrono::milliseconds(300));	
	cout << "ABOUT" << endl;	
	std::this_thread::sleep_for(std::chrono::milliseconds(300));	
	cout << "TO" << endl;	
	std::this_thread::sleep_for(std::chrono::milliseconds(300));	
	cout << "BEGIN" << endl;	
	std::this_thread::sleep_for(std::chrono::milliseconds(300));	

	cout << endl;
	cout << endl;
	cout << endl;
	
        std::vector<Space*> coach = ourHero.getCoach();
        std::vector<Space*> player = ourHero.getPlayer();
        std::vector<Space*> fan = ourHero.getFan();

	if( player.size()<5 ) {

	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	cout << "Oh no! How anti-climactic. Without 5 players even on the roster, Team Earth is absolutely destroyed by the MJ-liens, 164-22." << endl;  
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	cout << "GAME OVER" << endl;

	} else {

		std::this_thread::sleep_for(std::chrono::milliseconds(3700));
		cout << "It's a tight game early..." << player[4]->getName() << " is on fire, but the MJ-liens came to play" << endl;
		cout << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(3700));
		cout << "At halftime " << coach[0]->getName() << " makes some of the adjustments he was famous for with the " << coach[0]->getTeam() << endl;
		cout << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(4400));
		cout << "The game slows down in the second half. Defense tightens. Neither team really has an edge." << endl;
		cout << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		cout << "As the fourth quarter dwindles, team Earth needs someone to make a play..." << endl;
		cout << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(3500));
		cout << "but with timing winding down and Earth clinging to a lead, the MJ-liens drill a 3 to go ahead by 1!!! It doesn't look good." << endl;
		cout << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(6700));
		cout << "So here it is. Down 1, 2.4 seconds left. Earth has the ball." << endl;
		cout << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(3300));	
		if(coach.size()>0) cout << coach[0]->getName() << " draws up a play in the huddle. This is it. Now or never." << endl;
		else cout << "With no coach on the roster, who knows what play the team will draw up!" << endl;
		cout << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(3300));	
		if(fan.size()>0) cout << fan[0]->getName() << " yells some encouragement as the team takes the court...you can barely watch!" << endl;	
		cout << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(4000));
		cout << player[2]->getName() << " inbounds the ball. " << player[3]->getName() << " sets a pick, and " << player[0]->getName() << " gets free!" << endl;
		cout << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(4000));
		cout << "15 feet away from the basket, he turns, SHOOTS" << endl;
		cout << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(4000));
		cout << "The ball is in the air...time slows" << endl;	
		cout << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		cout << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		cout << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		cout << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		cout << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		cout << endl;

		if( coach.size()>0 && player.size()>=5 && fan.size()>0 ) {

		  cout << "BURIES IT" << endl;
		  cout << endl;
		  cout << endl;
		  std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		  cout << endl;
		  cout << "THE GAME IS OVER, EARTH WON! EARTH WON!" << endl;
		  std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		  cout << endl;
		  cout << endl;
		  cout << endl;
		  cout << "EARTH HAS WON THE INTERGALACTIC BASKETBALL CHAMPIONSHIP!!!!" << endl;
		  std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		  cout << endl;
		  cout << endl;
		  cout << endl;
		  cout << "Congratulations! You did it. The team you assembled had just enough to take down the MJ-liens!" << endl;

		} else {

		  cout << "CLANK! OH NO, IT DIDN'T DROP" << endl;
		  cout << endl;
		  cout << endl;
		  std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		  cout << endl;
		  cout << "The MJ-liens have won. Earth has lost. The MJ-liens celebrate on the court as they take the inaugual INTERGALACTIC BASKETBALL CHAMPIONSHIP" << endl;
		  std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		  cout << endl;
		  cout << endl;
		  cout << endl;
		  cout << "GAME OVER" << endl;
		  cout << endl;

		}

	}

};


// Clear the screen
void Game::clearScreen() {

	for(int i=0; i<60; i++) {

	  cout << endl;

	}

}


// Destructor to free dynamic memory
Game::~Game() {

	for(int i=0; i<basketballMap.size(); i++) {

	  delete basketballMap[i];

	}

};

