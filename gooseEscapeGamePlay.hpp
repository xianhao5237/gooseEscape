//Start of gooseEscapeGamePlay.hpp

#ifndef GOOSE_ESCAPE_GAMEPLAY
#define GOOSE_ESCAPE_GAMEPLAY
#include "gooseEscapeUtil.hpp"
#include "gooseEscapeActors.hpp"
#include "gooseEscapeConsole.hpp"

/*This file is all about the game world. Modify gooseGamePlay.cpp to
	actually add functionality.
*/

/*
    Declare constants to indicate various game world features in the board
    array.  Modify them to fit what you would like to do in the game.  You can
    change the type if you choose to store your game board as something other
    than intengers.
*/	
// Going further:  Learn how to use an enum for these values
const int EMPTY = 0;
const int SHALL_NOT_PASS = 1;
const int WINNER = 2;
const int TELEPORT = 3;
const int SHIELD_ITEM = 4;

// player initial location
const int PLAYER_X = 45;
const int PLAYER_Y = 19;

// goose initial location
const int MONSTER_X = 1;
const int MONSTER_Y = 1;

// winner/home location
const int WINNER_X = 30;
const int WINNER_Y = 1;

// teleport location
const int TELEPORT_X = 77;
const int TELEPORT_Y = 2;
const int TELEPORT_EXIT_X = 2;
const int TELEPORT_EXIT_Y = 18;

// shield location
const int SHIELD_X = 12;
const int SHIELD_Y = 19;

/*
    A few examples of characters both for actors and for the game board
    itself are shown.
*/	
//display characters
const int PLAYER_CHAR = int('@');
const int MONSTER_CHAR = int('G');
const int WALL_CHAR = int('o');
const int WIN_CHAR = int('H'); 
const int EMPTY_SPACE = int(' ');
const int TELEPORT_CHAR = int('T');
const int PLAYER_SHIELDED_CHAR = int('#'); 


// print the game board function protype
void printBoard (int board[NUM_BOARD_X][NUM_BOARD_Y], 
					Actor player, Actor monster);

/*
    Do something when the goose captures the player
    
    If you want to attack or something else, this is the function you 
    need to change.  For example, maybe the two touch each other and
    then fight.  You could add a health to the Actor class that is
    updated.  Run, use weapons, it's up to you!
*/
// lose if goose is on the player
bool captured(Actor & player, Actor & monster);

//checks if player has won by reaching the home
bool winner(Actor const & player, int winnerX, int winnerY);

//checks if player activacte the portal
bool isTeleporting(Actor const & player);

//teleporting the player
void teleportPlayer(Actor & player);

//gives a player a shield (additional life)
bool getShield(Actor & player, int board[NUM_BOARD_X][NUM_BOARD_Y]);


/*
    Move the player to a new location based on the user input.  You may want
    to modify this if there are extra controls you want to add.
    
    All key presses start with "TK_" then the character.  So "TK_A" is the a
    key being pressed.
    
    A look-up table might be useful.
    You could decide to learn about switch statements and use them here.
*/
// player input and movement
void movePlayer(int key, Actor & player, int board[NUM_BOARD_X][NUM_BOARD_Y]);

/*
    What other functions do you need to make the game work?  What can you
    add to the basic functionality to make it more fun to play?
*/

// goose movement function
void moveGoose(Actor & player, Actor & monster, 
				int board[NUM_BOARD_X][NUM_BOARD_Y]);

//wall creator 
void walls(int board[NUM_BOARD_X][NUM_BOARD_Y]);

//use walls to create a maze
void maze(ifstream & fin, int board[NUM_BOARD_X][NUM_BOARD_Y]);

#endif
