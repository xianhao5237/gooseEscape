//Start of gooseEscapeGamePlay.cpp

#include <iostream>
#include <cmath>
#include <fstream>
#include <stdlib.h>
#include <time.h>
using namespace std;

#include <BearLibTerminal.h>
#include "gooseEscapeUtil.hpp"
#include "gooseEscapeActors.hpp"
#include "gooseEscapeConsole.hpp"
#include "gooseEscapeGamePlay.hpp"

extern Console out;
const int RANDOM_NUM_RANGE = 3; // range of random numbers

/*
    Print the game world
    
    The functions should draw characters to present features of the game
    board, e.g. win location, obstacles, power ups
*/

// print the game board 
void printBoard (int board[NUM_BOARD_X][NUM_BOARD_Y], 
					Actor player, Actor monster)
{
	//loop through board array
	for(int index1 = 0; index1 < NUM_BOARD_X; index1++) 
    {
    	for(int index2 = 0; index2 < NUM_BOARD_Y; index2++)
    	{
			if(board[index1][index2] == SHALL_NOT_PASS) //output walls
				terminal_put(index1,index2,WALL_CHAR);
				
			if(board[index1][index2] == WINNER)	//output home
				terminal_put(index1,index2,WIN_CHAR);
			
			if(board[index1][index2] == TELEPORT)	//output teleport
				terminal_put(index1,index2,TELEPORT_CHAR);	
				
			if(board[index1][index2] == SHIELD_ITEM) //output powerups
                terminal_put(index1,index2,PLAYER_SHIELDED_CHAR);	
    	}
	}
}

/*
    Do something when the goose captures the player
    
    If you want to attack or something else, this is the function you 
    need to change.  For example, maybe the two touch each other and
    then fight.  You could add a health to the Actor class that is
    updated.  Run, use weapons, it's up to you!
*/


bool captured(Actor & player, Actor & monster)
{
    bool captured  = (player.get_x() == monster.get_x() 
         && player.get_y() == monster.get_y());
         
	// if the player has a shield when captured, the goose respawn to 
	//its orginal position and the player does not lose the game.
    if(captured)
    {
        if(player.isShield())
        {
            captured = false;
            player.change_char(PLAYER_CHAR);
            player.changeShield(false);
            monster.set_location(MONSTER_X, MONSTER_Y);
            player.put_actor();
        }
    }

    return captured;
}

bool winner(Actor const & player, int winnerX, int winnerY)
{
	bool winner = false;
	//if player is on top of home
   	if (player.get_x() == winnerX && player.get_y() == winnerY)
   	{
   		out.writeLine("YOU WIN!!!");
   		winner = true;
	}
    return winner;
}

// give a shield to the player
bool getShield(Actor & player, int board[NUM_BOARD_X][NUM_BOARD_Y])
{
    if(board[player.get_x()][player.get_y()] == SHIELD_ITEM)
    {
        board[player.get_x()][player.get_y()] = EMPTY;
        player.changeShield(true);
        return true;
    }
    return false;
}

// check if the player on the teleport tile
bool isTeleporting(Actor const & player)
{
	bool teleport = false;
	//if player is on top of teleprot
   	if (player.get_x() == TELEPORT_X && player.get_y() == TELEPORT_Y)
   	{
   		teleport = true;
	}
    return teleport;
}

//teleports player to a new location
void teleportPlayer(Actor & player) 
{
	player.set_location(TELEPORT_EXIT_X,TELEPORT_EXIT_Y);
}
/*
    Move the player to a new location based on the user input.  You may want
    to modify this if there are extra controls you want to add.
    
    All key presses start with "TK_" then the character.  So "TK_A" is the a
    key being pressed.
    
    A look-up table might be useful.
    You could decide to learn about switch statements and use them here.
*/

void movePlayer(int key, Actor & player, int board[NUM_BOARD_X][NUM_BOARD_Y])
{
    int yMove = 0, xMove = 0;
    if (key == TK_UP) //move player up
        yMove = -1;
    else if (key == TK_DOWN) //move player down
        yMove = 1;
    else if (key == TK_LEFT) //move player left
        xMove = -1;
    else if (key == TK_RIGHT) //move player right
        xMove = 1;
        
        //checks if move is valid
    if (board[player.get_x() + xMove][player.get_y() + yMove] 
		!= SHALL_NOT_PASS)
        player.update_location(xMove, yMove);
}

/*
    What other functions do you need to make the game work?  What can you
    add to the basic functionality to make it more fun to play?
*/
void moveGoose(Actor & player, Actor & monster, 
                int board[NUM_BOARD_X][NUM_BOARD_Y])
{
    int xMove = 0, yMove = 0;

    //finds direction for goose to get closer to player
    if(player.get_x() < monster.get_x()) 
        xMove = -1;
    if(player.get_x() > monster.get_x())
        xMove = 1;
    if(player.get_y() < monster.get_y())
        yMove = -1;
    if(player.get_y() > monster.get_y())
        yMove = 1;

     //if will hit wall
    if (board[monster.get_x() + xMove][monster.get_y() + yMove] 
        == SHALL_NOT_PASS)
    {
        srand (time(NULL));
        int randomNum = rand() % RANDOM_NUM_RANGE;

        if(randomNum == 0) //33% chance to jump over wall
        {
        	xMove *= 2;
            yMove *= 2;
        }
        else // if not jumping over wall, could move horzional or vertical
        {
            if(randomNum == 1)
                xMove = 0;
            else
                yMove = 0;
        }
    }

    //if goose only moves once in one direction he can move 2 times
    if(xMove == 0 && abs(yMove) == 1)
        yMove *= 2;
    if(yMove == 0 && abs(xMove) == 1)
        xMove *= 2;

    // goose cannot land on a wall
    if (board[monster.get_x() + xMove][monster.get_y() + yMove] 
            != SHALL_NOT_PASS 
            && board[monster.get_x() + xMove][monster.get_y() + yMove] 
            != WINNER)
        monster.update_location(xMove, yMove);
}


void walls(int board[NUM_BOARD_X][NUM_BOARD_Y])
{
	//loop through board array
	for(int indexX = 0; indexX < NUM_BOARD_X; indexX++)
    {
    	for(int indexY = 0; indexY < NUM_BOARD_Y; indexY++)
    	{
    		//creates horizontal borders
    		if(indexX == 0 || indexX == MAX_BOARD_X)
    		{
    			board[indexX][indexY] = SHALL_NOT_PASS;
			}
			//creates vertical borders
			if (indexY == 0 || indexY == MAX_BOARD_Y)
			{
				board[indexX][indexY] = SHALL_NOT_PASS;
			}
			
		}
	}
}

void maze(ifstream & fin, int board[NUM_BOARD_X][NUM_BOARD_Y])
{
    int length = 0;
    int xLocation = 0;
    int yLocation = 0;
    //reads wall locations from a file
    while(fin >> xLocation >> yLocation >> length)
    {
        for(int indexX = 0; indexX < NUM_BOARD_X; indexX++)
        {
            for(int indexY = 0; indexY < NUM_BOARD_Y; indexY++)
            {
                //create walls:
                if((indexX >= xLocation && indexX < (xLocation+length)) 
				&& indexY == (yLocation-1))
                    board[indexX][indexY] = SHALL_NOT_PASS;
            }
        }
    }
}




