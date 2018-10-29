/****************************************************************
** Author: Adam Walker
** Date: June 10 2018
** Description: Header file for Game class
****************************************************************/

#include "Player.hpp"
#include "filmRoom.hpp"
#include "teamGym.hpp"
#include "courtsideSeats.hpp"
#include "menu.hpp"

#include <chrono>
#include <ctime>
#include <thread>
#include <iomanip>

#ifndef GAME_HPP
#define GAME_HPP

class Game {

private:
  int turns;
  Player ourHero;
  std::vector<Space*> basketballMap;

public:

  // Constructor takes no input
  Game();

  // Runs the whole game
  void playGame();

  // Print the NBA map
  // There's an initial location only shown at the first turn
  void printMap(bool firstTurnInput=false);

  // Show turn menu
  int showMenu();
  
  // Intro script
  void gameIntro();

  //Set up game by creating all the spaces
  void setGame();

  // Play out a single turn of game, and the final showdown itself
  void playTurn();
  void playChampionship();

  // Utility function to clear screen
  void clearScreen();

  // Destructor to free dynamically allocated memory
  ~Game();

};

#endif
