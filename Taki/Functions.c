#include "declarations.h"


void run()
{
    PLAYERS* playersData = NULL;
    int i, numOfPlayers, playerChoice;
    int physSizeHand = START_SIZE_OF_HAND;
    char lastColorPlayed;
    bool gameOver = false;
    bool right = true;
    bool left = false;
    bool direction = true;  // True for right, false for left

    srand(time(NULL));

    printf("********** Welcome to the TAKI game ***********\n\n");
    printf("please enter the number of players:\n");
    scanf("%d", &numOfPlayers);

    if (numOfPlayers <= 0)
    {
        printf("Invalid input.");
        exit(1);
    }

    playersData = initDecksAndPlayersData(&numOfPlayers);

    printf("\nUpper card:\n");

    lastColorPlayed = printStartCard();

    while (gameOver == false)
    {
        for (i = (direction ? 0 : numOfPlayers - 1);
            (direction ? i < numOfPlayers : i >= 0);
            (direction ? i++ : i--))
        {
            printf("\n%s's turn\n\n", playersData[i].firstName);

            printPlayerCards(playersData, i);

            playerChoice = playerInput(playersData, i);

            playerChoice = checkRightColor(playersData, i, playerChoice, lastColorPlayed);

            doTurn(playersData, &i, playerChoice, &physSizeHand, &lastColorPlayed, &gameOver, &direction);
        }
    }

    freeAllocatedMemory(playersData, numOfPlayers);
}

void freeAllocatedMemory(PLAYERS* playersData, int numOfPlayers)
{
    int i;

    for (i = 0; i < numOfPlayers; i++)
    {
        free(playersData[i].cards);
    }

    free(playersData);
}

void doTurn(PLAYERS* playersData, int* i, int playerChoice, int* physSizeHand, char* lastColorPlayed, bool* gameOver, bool* direction)
{
    bool plus = false;
    bool stop = false;

    if (playerChoice == TAKE_CARD)
    {
        *physSizeHand = playerTakeCard(playersData, *i, *physSizeHand);
        printf("\nLast color played: %c\n", *lastColorPlayed);
    }
    else
    {
        printf("\n");

        printCard(playersData, *i, playerChoice - 1);

        doTurnBasedOfSelectedCard(playersData, *i, &playerChoice, lastColorPlayed, &plus, &stop, direction);

        playersData[*i].counterOfCards--;

        *gameOver = checkIfGameOver(playersData, *i, playerChoice);
    }

    if (plus == true)
    {
        direction ? (*i)-- : (*i)++;
    }
    if (stop == true)
    {
        direction ? (*i)++ : (*i)--;
    }
}

bool checkIfGameOver(PLAYERS* playersData, int i, int playerChoice)
{
    if (playersData[i].counterOfCards <= 0)
    {
        printf("The winner is... p%d! congratulations!", i + 1);
        return true;
    }
    else if (playerChoice - 1 != playersData[i].counterOfCards)
        swapBetweenLastCardAndUsedCard(playersData, i, playerChoice - 1, playersData[i].counterOfCards);

    return false;
}

PLAYERS* initDecksAndPlayersData(int* numOfPlayers)
{
    int i;
    PLAYERS* playersData = (PLAYERS*)malloc(*numOfPlayers * sizeof(PLAYERS));

    if (playersData == NULL)
    {
        printf("\nERROR! Memory allocation failed.");
        exit(1);
    }

    getPlayersName(playersData, *numOfPlayers);

    for (i = 0; i < *numOfPlayers; i++)
    {
        playersData[i].counterOfCards = 0;

        playersData[i].cards = NULL;

        playersData[i].cards = (CARDS*)malloc(4 * sizeof(CARDS));

        if (playersData[i].cards == NULL)
        {
            printf("\nERROR! Memory allocation failed.");
            exit(1);
        }
        else
            takeCard(playersData, i, 4);
    }

    return playersData;
}

void doTurnBasedOfSelectedCard(PLAYERS* playersData, int i, int* playerChoice, char* lastColorPlayed, bool* plus, bool* stop, bool* direction)
{
    int newColor;

    switch (playersData[i].cards[*playerChoice - 1].typeNum)
    {
    case CHANGE_COLOR_CARD:
        printf("\nPlease enter your color choice:\n");
        printf("1 - Yellow\n2 - Red\n3 - Blue\n4 - Green\n");
        scanf("%d", &newColor);

        while (newColor != 1 && newColor != 2 && newColor != 3 && newColor != 4)
        {
            printf("Invalid Choice! Try again.\n\n");
            printf("\nPlease enter your color choice:\n");
            printf("1 - Yellow\n2 - Red\n3 - Blue\n4 - Green\n");
            scanf("%d", &newColor);
        }

        switch (newColor)
        {
        case (int)YELLOW:
            *lastColorPlayed = 'Y';
            printf("\nThe new color is yellow!\n");
            break;
        case (int)RED:
            *lastColorPlayed = 'R';
            printf("\nThe new color is red!\n");
            break;
        case (int)BLUE:
            *lastColorPlayed = 'B';
            printf("\nThe new color is blue!\n");
            break;
        case (int)GREEN:
            *lastColorPlayed = 'G';
            printf("\nThe new color is green!\n");
            break;
        default:
            break;
        }

        break;

    case TAKI_CARD:
        takiCard(playersData, i, *lastColorPlayed, playerChoice);
        break;

    case CHANGE_DIRACTION_CARD:
         *direction = !*direction;
         break;

    case STOP_CARD:
        *stop = true;
        break;

    case PLUS_CARD:
        *plus = true;
        break;

    }
}

void takiCard(PLAYERS* playersData, int i, char lastColorPlayed, int* playerChoice)
{

    playersData[i].counterOfCards--;

    swapBetweenLastCardAndUsedCard(playersData, i, *playerChoice - 1, playersData[i].counterOfCards);

    printf("\n");

    printPlayerCards(playersData, i);

    *playerChoice = playerInputForTakiCard(playersData, i);

    *playerChoice = checkIfSameColorToContinue(playersData, i, *playerChoice, lastColorPlayed);

    while (*playerChoice != 0)
    {
    
        printf("\n");
        printCard(playersData, i, *playerChoice - 1);
        printf("\n");
        playersData[i].counterOfCards--;

        if (playersData[i].counterOfCards == 0)
        {
            break;
        }
        else if (*playerChoice - 1 != playersData[i].counterOfCards)
            swapBetweenLastCardAndUsedCard(playersData, i, *playerChoice - 1, playersData[i].counterOfCards);

        printPlayerCards(playersData, i);

        *playerChoice = playerInputForTakiCard(playersData, i);
        *playerChoice = checkIfSameColorToContinue(playersData, i, *playerChoice, lastColorPlayed);
    }
}

void printPlayerCards(PLAYERS* playersData, int i)
{
    int k;

    for (k = 0; k < playersData[i].counterOfCards; k++)
    {
        printf("Card #%d\n", k + 1);
        printCard(playersData, i, k);
        printf("\n");
    }
}

int checkRightColor(PLAYERS* playersData, int i, int playerChoice, char lastColorPlayed) {

    bool continueToPlay = false;

    if (playerChoice == 0 || playersData[i].cards[playerChoice - 1].typeNum == CHANGE_COLOR_CARD)
        continueToPlay = true;

    while ((playerChoice > playersData[i].counterOfCards || playersData[i].cards[playerChoice - 1].color != lastColorPlayed) && continueToPlay == false)
    {
        printf("Invalid Card! Try again.\n\n");

        playerChoice = playerInput(playersData, i);

        if (playerChoice == 0 || playersData[i].cards[playerChoice - 1].color == lastColorPlayed)
            continueToPlay = true;
    }

    return playerChoice;

}

int checkIfSameColorToContinue(PLAYERS* playersData, int i, int playerChoice, char lastColorPlayed) {

    bool continueToPlay = false;

    if (playerChoice == 0)
        continueToPlay = true;

    while ((playerChoice > playersData[i].counterOfCards || playersData[i].cards[playerChoice - 1].color != lastColorPlayed) && continueToPlay == false)
    {
        printf("Invalid Card! Try again.\n\n");

        playerChoice = playerInputForTakiCard(playersData, i);

        if (playerChoice == 0 || playersData[i].cards[playerChoice - 1].color == lastColorPlayed)
            continueToPlay = true;
    }

    return playerChoice;

}

void swapBetweenLastCardAndUsedCard(PLAYERS* playersData, int indPlayer, int indCard, int indLast)
{

    int temp = playersData[indPlayer].cards[indCard].typeNum;
    char temp2 = playersData[indPlayer].cards[indCard].color;
    char temp3[LEN_OF_TYPE];

    strcpy(temp3, playersData[indPlayer].cards[indCard].typeName);

    playersData[indPlayer].cards[indCard].typeNum = playersData[indPlayer].cards[indLast].typeNum;
    playersData[indPlayer].cards[indCard].color = playersData[indPlayer].cards[indLast].color;
    strcpy(playersData[indPlayer].cards[indCard].typeName, playersData[indPlayer].cards[indLast].typeName);

    playersData[indPlayer].cards[indLast].typeNum = temp;
    playersData[indPlayer].cards[indLast].color = temp2;
    strcpy(playersData[indPlayer].cards[indLast].typeName, temp3);

}

int playerTakeCard(PLAYERS* playersData, int i, int size)
{
    int logicalSize = playersData[i].counterOfCards;
    int physicalSize = size;

    if (logicalSize == physicalSize)
    {
        physicalSize *= 2;
        playersData[i].cards = (CARDS*)realloc(playersData[i].cards, physicalSize * sizeof(CARDS));
    }

    takeCard(playersData, i, 1);

    return (physicalSize);
}

int playerInput(PLAYERS* playersData, int i)
{
    int playerChoice;

    if (playersData[i].counterOfCards > 1)
    {
        printf("Please enter 0 if you want to take a card from the deck or 1-%d if you want to put one of your cards in the middle:\n",
            playersData[i].counterOfCards);
        scanf("%d", &playerChoice);
    }
    else
    {
        printf("Please enter 0 if you want to take a card from the deck or 1 if you want to put your last card in the middle:\n", playersData[i].counterOfCards);
        scanf("%d", &playerChoice);
    }

    return playerChoice;

}

int playerInputForTakiCard(PLAYERS* playersData, int i)
{
    int playerChoice;

    if (playersData[i].counterOfCards > 1)
    {
        printf("Please enter 0 if you want to 'close' the taki card or 1-%d if you want to put one of your cards in the middle:\n",
            playersData[i].counterOfCards);
        scanf("%d", &playerChoice);
    }
    else
    {
        printf("Please enter 0 if you want to 'close' the taki card or 1 if you want to put your last card in the middle:\n", playersData[i].counterOfCards);
        scanf("%d", &playerChoice);
    }

    return playerChoice;

}

void getPlayersName(PLAYERS* playersData, int size)
{
    int i;

    for (i = 0; i < size; i++)
    {
        printf("Please enter the first name of player #%d:\n", i + 1);
        scanf("%s", playersData[i].firstName);
    }
}

char printStartCard()
{
    int randomNum = (rand() % 9 + 1);
    char color = randomColor();

    printLine();

    printMiddleLine();

    printNumLine(randomNum);

    printColorLine(color);

    printMiddleLine();

    printLine();

    return color;
}

void printNumLine(int num)
{
    int i;

    printf("*");

    for (i = 1; i < WIDTH_OF_CARD / 2; i++)
        printf(" ");

    printf("%d", num);

    for (i = 1; i < WIDTH_OF_CARD / 2; i++)
        printf(" ");

    printf("*\n");
}

void printTypeLine(char* type)
{
    int i;
    int len = strlen(type);

    printf("*");

    for (i = 1; i < (WIDTH_OF_CARD - len + 1) / 2; i++)
        printf(" ");

    i = 0;

    while (type[i] != '\0')
    {
        printf("%c", type[i]);
        i++;
    }

    for (i = 1; i < (WIDTH_OF_CARD - len) / 2; i++)
        printf(" ");

    printf("*\n");
}

void printColorLine(char color)
{
    int i;

    printf("*");

    for (i = 1; i < WIDTH_OF_CARD / 2; i++)
        printf(" ");

    printf("%c", color);

    for (i = 1; i < WIDTH_OF_CARD / 2; i++)
        printf(" ");

    printf("*\n");
}

void printLine()
{
    int i;

    for (i = 1; i <= WIDTH_OF_CARD; i++)
        printf("*");
    printf("\n");
}

void printMiddleLine()
{
    int i;

    printf("*");

    for (i = 2; i < WIDTH_OF_CARD; i++)
        printf(" ");

    printf("*\n");
}

void takeCard(PLAYERS* playersData, int playerInd, int numOfCards) {

    int i;

    for (i = 0; i < numOfCards; i++)
    {
        randomCardType(playersData, playerInd, playersData[playerInd].counterOfCards);
        playersData[playerInd].counterOfCards++;
    }

}

void randomCardType(PLAYERS* playerData, int playerInd, int cardInd)
{
    char taki[TAKI] = { 'T', 'A', 'K', 'I', '\0' };
    char color[COLOR] = { 'C', 'O', 'L', 'O','R','\0' };
    char stop[STOP] = { 'S', 'T', 'O', 'P', '\0' };
    char change[CHANGE_DIRACTION_CARD] = { '<', '-', '>', '\0' };
    int randomNum = 1 + (rand() % 9);
    int typeNum = (rand() % 6) + 1;


    switch (typeNum)
    {
    case CHANGE_COLOR_CARD:
        strcpy(playerData[playerInd].cards[cardInd].typeName, color);
        playerData[playerInd].cards[cardInd].color = ' ';
        playerData[playerInd].cards[cardInd].typeNum = typeNum;
        break;
    case NUMBER_CARD:
        playerData[playerInd].cards[cardInd].typeName[0] = (char)('0' + randomNum);
        playerData[playerInd].cards[cardInd].typeName[1] = '\0';
        playerData[playerInd].cards[cardInd].color = randomColor();
        playerData[playerInd].cards[cardInd].typeNum = typeNum;
        break;
    case TAKI_CARD:
        strcpy(playerData[playerInd].cards[cardInd].typeName, taki);
        playerData[playerInd].cards[cardInd].color = randomColor();
        playerData[playerInd].cards[cardInd].typeNum = typeNum;
        break;
    case CHANGE_DIRACTION_CARD:
        strcpy(playerData[playerInd].cards[cardInd].typeName, change);
        playerData[playerInd].cards[cardInd].color = randomColor();
        playerData[playerInd].cards[cardInd].typeNum = typeNum;
        break;
    case STOP_CARD:
        strcpy(playerData[playerInd].cards[cardInd].typeName, stop);
        playerData[playerInd].cards[cardInd].color = randomColor();
        playerData[playerInd].cards[cardInd].typeNum = typeNum;
        break;
    case PLUS_CARD:
        playerData[playerInd].cards[cardInd].typeName[0] = '+';
        playerData[playerInd].cards[cardInd].typeName[1] = '\0';
        playerData[playerInd].cards[cardInd].color = randomColor();
        playerData[playerInd].cards[cardInd].typeNum = typeNum;
        break;
    default:
        break;

    }



}

char randomColor()
{
    char color;

    switch (1 + (rand() % 4))
    {
    case 1:
        color = 'R';
        break;
    case 2:
        color = 'B';
        break;
    case 3:
        color = 'Y';
        break;
    case 4:
        color = 'G';
        break;
    default:
        color = ' ';
        break;
    }
    return color;
}

void printCard(PLAYERS* playersData, int playerInd, int indOfCards)
{

    printLine();

    printMiddleLine();

    printTypeLine(playersData[playerInd].cards[indOfCards].typeName);

    printColorLine(playersData[playerInd].cards[indOfCards].color);

    printMiddleLine();

    printLine();
}