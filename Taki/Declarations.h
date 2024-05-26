#pragma once


#define _CRT_SECURE_NO_WARNINGS

#define LEN_OF_NAME 20
#define START_SIZE_OF_HAND 4
#define WIDTH_OF_CARD 9
#define CHANGE_COLOR_CARD 1
#define NUMBER_CARD 2
#define TAKI_CARD 3
#define CHANGE_DIRACTION_CARD 4
#define STOP_CARD 5
#define PLUS_CARD 6
#define TAKI 5
#define COLOR 6
#define STOP 5
#define TAKE_CARD 0
#define LEN_OF_TYPE 7

enum eCOLORS
{
    YELLOW = 1,
    RED,
    BLUE,
    GREEN
};


#include <stdio.h>
#include <stdlib.h> // in order to use the "rand" and "srand" functions
#include <time.h>	// in order to use "time" function
#include <stdbool.h>
#include <string.h>

typedef struct Cards {
    int typeNum;
    char typeName[LEN_OF_TYPE];
    char color;
}CARDS;

typedef struct playerData {
    char firstName[LEN_OF_NAME];
    CARDS* cards;
    int counterOfCards;
}PLAYERS;

void run();//the function start the game.

void freeAllocatedMemory(PLAYERS* playersData, int numOfPlayers);//the function free all allocated memory.

void doTurn(PLAYERS* playersData, int* i, int playerChoice, int* physSizeHand, int* lastColorPlayed, bool* gameOver, bool* direction);//the function runs the turn based of player choices and rules of taki.

bool checkIfGameOver(PLAYERS* playersData, int i, int playerChoice);//the function return true if the player has no more cards.

void doTurnBasedOfSelectedCard(PLAYERS* playersData, int i, int* playerChoice, int* lastColorPlayed, bool* plus, bool* stop, bool* direction);//the function does the action needed based of selected card.

PLAYERS* initDecksAndPlayersData(int* numOfPlayers);//the function allocates the decks and initialize them.

void randomCardType(PLAYERS* playersData, int playerInd, int cardInd);//the function generates random cards.

char printStartCard();//the function prints the start card for the game. it can only be a number card.

void printNumLine(int num);//the function prints a line of stars but with the num/name of card in the middle.

char randomColor();//the function generates a random color for the card.

void takeCard(PLAYERS* playersData, int playerInd, int numOfCards);//the function is uses when the player take a new card.

void printLine();//the function prints a line of stars.

void printMiddleLine();//the function prints a line of spaces with stars in the edges.

void printTypeLine(char* type);//the function prints a line of stars but with the num/name of card in the middle.

void printColorLine(char color);//the function prints a line of stars but with the color of the card in the middle.

void getPlayersName(PLAYERS* playersData, int size);//the function get the players names and put it in the array.

void printCard(PLAYERS* playersData, int playerInd, int indOfCards);//the function is printing the card inserted to it by the index.

int playerInput(PLAYERS* playersData, int i);//the function take an answer from the player of the action he want to take.

int playerTakeCard(PLAYERS* playersData, int i, int size);//the function is used to take a new card from the deck. when the logical size of the array is equal to the physical size it uses realloc to make a new array twice the size.

void swapBetweenLastCardAndUsedCard(PLAYERS* playersData, int indPlayer, int indCard, int indLast);// the function swaps the cards in the array

int checkRightColor(PLAYERS* PlayersData, int i, int playerChoice, char checkRightColor);// the function check if the input is valid.

void printPlayerCards(PLAYERS* playersData, int i);//the function prints all the player cards that are in his hands

void takiCard(PLAYERS* playersData, int i, char lastColorPlayed, int* playerChoice);//the function is used to activate the spacial traits of the taki card.

int playerInputForTakiCard(PLAYERS* playersData, int i); //the function take an answer from the player of the action he want to take when a taki card was played.

int checkIfSameColorToContinue(PLAYERS* playersData, int i, int playerChoice, char lastColorPlayed);// the function check if the input is valid when a taki card is open.