/* 
 * File:   main.cpp
 * Author: Josue Carlito
 * Created on 4/20/2023 4:12PM
 * Purpose:  Mastermind Game STL Project
 * Version: 1, Use a map in the hints function
 */

//System Level Libraries
#include <cstdlib>
#include <iostream>   
#include <iomanip>    
#include <cmath>      
#include <ctime>      
#include <string>     
#include <algorithm>  
#include <list>
#include <map>
#include <random>
using namespace std;

//Function Prototypes
void intro(int &n, char &dup, int &gameLen); //Display intro
void colors(char guess); //Switch digits to colors
void hint(string code, string guess, int &rr, int &rw); //Evaluate Hints
void outro(string code, string guess, int n); //Display outro
void shuffle(list<char>& gen);
string set(int n, char dup); //Set random code
//Display Game board
void displayBoard(char** board, int gC, int n, int* rr, int *rw);
int* generateHintArray(int gameLen);
void deleteHintArrays(int* rr, int* rw);
char** generateGameBoard(int nColors, int gameLen);

void deleteGameboard(char** gameBoard, int gameLen);

//Execution Starts Here

int main(int argc, char** argv) {
    //Set Random Number Seed Here
    srand(static_cast<unsigned int> (time(0)));

    //Declare Variables - Known and Unknown, units, range, description

    int nColors; //Number of colors the user wants to guess
    int gameLen; //Length of the game the user chooses
    int gameCounter; //Counter for the guess the user is on
    int rrTemp; //Right color right place temporary variable
    int rwTemp; //Right color wrong place temporary variable
    int* rr; //Right color right place array
    int* rw; //Right color wrong place array
    char** board; //Game board 2D array
    char dup; //Whether the user wants to allow duplicates or not
    string guess; //User guess
    string code; //Generated code

    //Initialize Variables
    gameCounter = 0;

    //Display the outputs and process inputs

    //Display game intro
    intro(nColors, dup, gameLen);
    board = generateGameBoard(nColors, gameLen);
    rr = generateHintArray(gameLen);
    rw = generateHintArray(gameLen);

    //Set the randomly generated code
    code = set(nColors, dup);
    cout << "Code = " << code << endl;
    //Store user's guess
    cin >> guess;

    //Input Validation
    while (guess.size() != nColors) {
        cout << "Your guess isn't long enough. Try again: ";
        cin >> guess;
    }

    //Game Loop and check user's guess with the correct code
    while (gameCounter < (gameLen - 1) && guess != code) {

        //Evaluate user's guess for hints
        hint(code, guess, rrTemp, rwTemp);

        //Store Hints
        rr[gameCounter] = rrTemp;
        rw[gameCounter] = rwTemp;
        cout << endl;

        //Fill board
        for (int j = 0; j < gameLen; j++) {
            board[gameCounter][j] = guess[j];
        }

        //Display board
        displayBoard(board, gameCounter, nColors, rr, rw);

        //Store User's guess
        cout << endl;
        cout << "Enter your next guess: ";
        cin >> guess;

        //Input validation
        while (guess.size() != nColors) {
            cout << "Your guess isn't long enough. Try again: ";
            cin >> guess;
        }
        gameCounter++;
    }

    //Evaluate user's guess for hints
    hint(code, guess, rrTemp, rwTemp);

    //Store Hints
    rr[gameCounter] = rrTemp;
    rw[gameCounter] = rwTemp;

    //Fill board
    for (int j = 0; j < gameLen; j++) {
        board[gameCounter][j] = guess[j];
    }
    //Display board
    displayBoard(board, gameCounter, nColors, rr, rw);

    //Display outro message
    outro(code, guess, nColors);


    // Free memory
    deleteGameboard(board, gameLen);
    deleteHintArrays(rr, rw);

    //Exit Stage Right!
    return 0;
}

//Function Implementations

//Display intro

void intro(int &n, char &dup, int &gameLen) {
    //Display Prompt
    cout << "Welcome to Mastermind:" << endl;
    cout << "There are 8 colors to choose from." << endl;
    cout << "The colors are Red(0), Green(1), Blue(2), Yellow(3), Brown(4), ";
    cout << "Orange(5), Black(6)";
    cout << " and White(7)." << endl;
    cout << "The Board displays the colors as:" << endl;
    cout << "Red = Rd, Green = Gr, Blue = Bl, Yellow = Ye, Brown = Br";
    cout << ", Orange = Or, Black = Bk, and White = Wh" << endl;
    cout << "The board also displays hints:" << endl;
    cout << "RR = Right color right place and RW = Right color wrong place";
    cout << endl << endl;
    cout << "Let the game begin!" << endl;
    cout << "Enter 4, 6, or 8 colors based off how many you want to guess: ";
    cin >> n;

    //Input Validation
    while (n != 4 && n != 6 && n != 8) {
        cout << "Oops you input an invalid number. Try again: ";
        cin >> n;
    }

    cout << "Choose whether you'd like to allow duplicates (y/n): ";
    cin >> dup;

    //Input Validation
    while (dup != 'y' && dup != 'Y' && dup != 'n' && dup != 'N') {
        cout << "Oops you input an invalid answer. Try again: ";
        cin >> dup;
    }

    cout << "Finally, choose whether you'd like to give yourself 8, 10, or 12";
    cout << " guesses: ";
    cin >> gameLen;

    //Input Validation
    while (gameLen != 8 && gameLen != 10 && gameLen != 12) {
        cout << "Oops you input an invalid number. Try again: ";
        cin >> gameLen;
    }
    cout << n << " colors will be generated and you have to pick ";
    cout << "the right colors ";
    cout << "in the right order." << endl << endl;
    cout << "Enter your " << n << " numbers that correspond to "
            "your color guess ";
    cout << "(i.e 0123 = Red Green Blue Yellow)" << endl;
}

//Switch digits two colors

void colors(char guess) {
    //Switch digits to the given two letter color code
    switch (guess) {
        case '0': cout << "Rd ";
            break;
        case '1': cout << "Gr ";
            break;
        case '2': cout << "Bl ";
            break;
        case '3': cout << "Yl ";
            break;
        case '4': cout << "Br ";
            break;
        case '5': cout << "Or ";
            break;
        case '6': cout << "Bk ";
            break;
        case '7': cout << "Wh ";
            break;
    }
}

void shuffle(list<char>& gen) {
    int length = gen.size();
    default_random_engine engine(time(0));
    uniform_int_distribution<int> dist(0, length - 1);
    for (int i = 0; i < length; i++) {
        list<char>::iterator it1 = next(gen.begin(), i);
        list<char>::iterator it2 = next(gen.begin(), dist(engine));
        swap(*it1, *it2);
    }
}

//Set random code

string set(int n, char dup) {
    //Initialize variables

    // List to be shuffled
    list<char> gen = {'0', '1', '2', '3', '4', '5', '6', '7'};
    shuffle(gen);
    string tempCode = "0"; // Code that's going to be filled

    //Resize the code string to the size the user specified
    tempCode.resize(n);
    if (dup == 'n') {//User does not allow duplicates
        list<char>::iterator it = gen.begin();
        for (int i = 0; i < n; i++) {
            tempCode[i] = *it;
            it++;
        }
    } else { //User allows duplicates
        for (int k = 0; k < n; k++) {
            tempCode[k] = (rand() % 8) + '0'; //Randomly fill code string
        }
    }
    //Return the generated code back to main
    return tempCode;
}

//Evaluate guess to give hints

void hint(string code, string guess, int& rr, int& rw) {
    map<char, int> codeMap;
    map<char, int> guessMap;
    rr = 0; // right color right place
    rw = 0; // right color wrong place
    for (int i = 0; i < code.length(); i++) {
        // if the colors match increment rr
        if (code[i] == guess[i]) {
            rr++;
        } else {
            // Store unmatched colors and how many times the occur
            codeMap[code[i]]++;
            guessMap[guess[i]]++;
        }
    }

    for (map<char, int>::const_iterator codeKV = codeMap.begin();
            codeKV != codeMap.end(); codeKV++) {
        map<char, int>::iterator guessIt = guessMap.find(codeKV->first);
        if (guessIt != guessMap.end()) {
            rw += min(codeKV->second, guessIt->second);
        }
    }
}

//Display outro

void outro(string code, string guess, int n) {
    //Display the outro
    cout << endl;
    //Display a congratulatory message when the user guesses correctly
    if (code == guess) {
        cout << "Congratulations you guessed correctly! The answer is \"";
        for (int a = 0; a < n; a++) {
            colors(code[a]);
        }
        cout << "\"" << endl;
    } else { //Display a failure message with the correct answer
        cout << "You failed :(";
        cout << " The correct answer is \"";
        for (int a = 0; a < n; a++) {
            colors(code[a]);
        }
        cout << "\"" << endl;
    }
}

//Display game board

void displayBoard(char** board, int gC, int n, int *rr, int *rw) {
    //Display game board
    cout << "Game Board:" << endl;
    for (int z = 0; z <= gC; z++) {
        //Display row
        if ((z + 1) >= 10) { //Display corrected formatting for double digits
            cout << "Row " << z + 1 << ":" << setw(2);
            cout << "|" << setw(4);
        } else { //Display normal formatting
            cout << "Row " << z + 1 << ":" << setw(3);
            cout << "|" << setw(4);
        }

        //Switch the numbers to the two letter color description
        for (int k = 0; k < n; k++) {

            colors(board[z][k]);
        }

        //Display the hints
        cout << "| RR: " << rr[z] << " RW: " << rw[z];
        cout << endl;
    }
}

char** generateGameBoard(int nColors, int gameLen) {
    // dynamically allocate the gameboard
    char** gameBoard = new char*[gameLen];
    for (int i = 0; i < gameLen; i++) {
        gameBoard[i] = new char[nColors];
    }
    return gameBoard;
}

void deleteGameboard(char** gameBoard, int gameLen) {
    for (int i = 0; i < gameLen; i++) {
        delete[] gameBoard[i];
    }
    delete[] gameBoard;
}

int* generateHintArray(int gameLen) {
    int* hintArray = new int[gameLen];

    return hintArray;
}

void deleteHintArrays(int* rr, int* rw) {
    delete[] rr;
    delete[] rw;
}