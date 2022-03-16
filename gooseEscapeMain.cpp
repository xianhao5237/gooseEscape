//Start of gooseEscapeMain.cpp

#include <BearLibTerminal.h>
#include <cmath>
#include <iostream>
#include <fstream>
using namespace std;
#include "gooseEscapeUtil.hpp"
#include "gooseEscapeActors.hpp"
#include "gooseEscapeConsole.hpp"
#include "gooseEscapeGamePlay.hpp"

//set up the console.   Don't modify this line!
Console out;

int main() {
	//Set up the window.  Don't edit these two lines
    terminal_open();
    terminal_set(SETUP_MESSAGE);
  	
  	//check file
    ifstream fin("wall_location.txt");
    if(!fin) {
        cout << "File not found";
        return EXIT_FAILURE;
    }

    //make the player
    Actor player(PLAYER_CHAR, PLAYER_X,PLAYER_Y);
	
	//make the monster
    Actor monster(MONSTER_CHAR, MONSTER_X, MONSTER_Y);
	
    // Declare the array that will hold the game board "map"
    int gameBoard[NUM_BOARD_X][NUM_BOARD_Y] = {EMPTY};
  	
/*
    Initiallize locations in the game board to have game features.  What if you
    have man things to add to the game board?  Should you use a loop?  Does it
    make sense to store this information in a file?  Should this code be a
    function as well?
*/
    walls(gameBoard); //generate boarder
    maze(fin, gameBoard); // generate maze
    gameBoard[WINNER_X][WINNER_Y] = WINNER; //create win condition
    gameBoard[TELEPORT_X][TELEPORT_Y] = TELEPORT; // teleport tile
    gameBoard[SHIELD_X][SHIELD_Y] = SHIELD_ITEM; //create shield item

    // Call the function to print the game board
    printBoard(gameBoard, player, monster);	
  	
	// Printing the instructions
    out.writeLine("Escape the Goose! " + monster.get_location_string());
    out.writeLine("Use the arrow keys to move");
    out.writeLine("If the goose catches you, you lose! Get to H to win.");
    out.writeLine("Be careful! Sometimes the goose can jump through walls!");
    out.writeLine("'T' is a teleport tile. Step on it to teleport.");
    out.writeLine("'#' is a shield. Pick it up to defend yourself.");

/*
    This is the main game loop.  It continues to let the player give input
    as long as they do not press escape or close, they are not captured by
    the goose, and they didn't reach the win tile
*/
/*
    All key presses start with "TK_" then the character.  So "TK_A" is the "a"
    key being pressed.
*/
    int keyEntered = TK_A; // can be any valid value that is not ESCAPE or CLOSE
    
    //run while player not captured or won the game or closed the game
    while(keyEntered != TK_ESCAPE && keyEntered != TK_CLOSE && !captured(player,monster) && !winner(player, WINNER_X, WINNER_Y)) {
	    // get player key press
	 keyEntered = terminal_read();

        if (keyEntered != TK_ESCAPE && keyEntered != TK_CLOSE) {
            // move the player
    	    movePlayer(keyEntered, player, gameBoard);

            // call the goose's chase function
            moveGoose(player, monster, gameBoard);
            
            // check if player on teleport tile
		if(isTeleporting(player))
			teleportPlayer(player); //teleport player

        }
        //if get shield
        if(getShield(player, gameBoard))
        	player.change_char(PLAYER_SHIELDED_CHAR); //give player shield

  	}
   
    if (keyEntered != TK_CLOSE) {
      	//once we're out of the loop, the game is over
        out.writeLine("Game has ended.");
    
    	// Wait until user closes the window
        while (terminal_read() != TK_CLOSE);
    }

	//game is done, close it  
    terminal_close();
    fin.close();
}
