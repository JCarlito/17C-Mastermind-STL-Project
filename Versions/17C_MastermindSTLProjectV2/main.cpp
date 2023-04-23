/* 
 * File:   main.cpp
 * Author: Josue Carlito
 * Created on 4/20/2023 8:26PM
 * Purpose:  Mastermind Game STL Project
 * Version: 2, Use queues/stacks for the game board depending on the mode
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
#include <queue>
#include <utility>
#include <set>
#include <stack>
#include <random>
#include <vector>
using namespace std;

//Global Constants
//Science and Math, Conversions, Higher Dimensions const to follow

//Function Prototypes
void intro(int &n, char &dup, int &gameLen, char &modeFlag); //Display intro
void colors(char guess); //Switch digits to colors
void hint(string code, string guess, int &rr, int &rw); //Evaluate Hints
void outro(string code, string guess, int n); //Display outro
void shuffle(list<char>& gen);
string setCode(int n, char dup); //Set random code
//Display Game board
void displayBoard(queue<pair<int, int>>hints, queue<string> guesses,
        const set<char>& userColors);
void displayBoard(queue<pair<int, int>>hints, stack<string> prevGuesses,
        const set<char>& usedColors);

//Execution Starts Here

int main(int argc, char** argv) {
    //Set Random Number Seed Here
    srand(static_cast<unsigned int> (time(0)));

    //Declare Variables - Known and Unknown, units, range, description
    queue<string> guesses; // user's guesses in normal mode;
    stack<string> practiceGuesses; // User's guesses in practice mode;
    queue<pair<int, int>> hints; // pair of hints 
    set<char> usedColors; // The colors the player has already used
    int nColors; //Number of colors the user wants to guess
    int gameLen; //Length of the game the user chooses
    int rrTemp; //Right color right place temporary variable
    int rwTemp; //Right color wrong place temporary variable
    char dup; //Whether the user wants to allow duplicates or not
    char modeFlag; // Game mode flag
    string guess; //User guess
    string code; //Generated code

    //Display the outputs and process inputs

    //Display game intro
    intro(nColors, dup, gameLen, modeFlag);

    if (modeFlag == 'N' || modeFlag == 'n') {
        //Set the randomly generated code
        code = setCode(nColors, dup);
        cout << "Code = " << code << endl;
        //Store user's guess
        cin >> guess;

        //Input Validation
        while (guess.size() != nColors) {
            cout << "Your guess isn't long enough. Try again: ";
            cin >> guess;
        }

        // Add colors to the set
        for (string::iterator it = guess.begin(); it != guess.end(); it++) {
            char character = *it;
            usedColors.insert(character);
        }
        guesses.push(guess);

        //Game Loop and check user's guess with the correct code
        while (guesses.size() < gameLen && guess != code) {

            //Evaluate user's guess for hints
            hint(code, guess, rrTemp, rwTemp);

            //Store Hints
            hints.push(make_pair(rrTemp, rwTemp));
            cout << endl;

            //Display board
            displayBoard(hints, guesses);

            //Store User's guess
            cout << endl;
            cout << "Enter your next guess: ";
            cin >> guess;

            //Input validation
            while (guess.size() != nColors) {
                cout << "Your guess isn't long enough. Try again: ";
                cin >> guess;
            }
            // Add colors to the set
            for (string::iterator it = guess.begin(); it != guess.end(); it++) {
                char character = *it;
                usedColors.insert(character);
            }
            guesses.push(guess);
        }

        //Evaluate user's guess for hints
        hint(code, guess, rrTemp, rwTemp);

        //Store Hints
        hints.push(make_pair(rrTemp, rwTemp));

        //Display board
        displayBoard(hints, guesses);

        //Display outro message
        outro(code, guess, nColors);
    } else {
        cout << "To delete your previous guess, press (-1)" << endl;
        cout << endl;
        //Set the randomly generated code
        code = setCode(nColors, dup);
        cout << "Code = " << code << endl;
        //Store user's guess
        cin >> guess;

        //Input Validation
        while (guess.size() != nColors || guess == "-1") {
            cout << "Your guess isn't long enough or you tried deleting a ";
            cout << "guess before your first turn.\nTry again: ";
            cin >> guess;
        }

        // Add colors to the set
        for (string::iterator it = guess.begin(); it != guess.end(); it++) {
            char character = *it;
            usedColors.insert(character);
        }
        practiceGuesses.push(guess);

        //Game Loop and check user's guess with the correct code
        while (practiceGuesses.size() < gameLen && guess != code) {

            //Evaluate user's guess for hints
            if (guess != "-1")
                hint(code, guess, rrTemp, rwTemp);

            //Store Hints
            hints.push(make_pair(rrTemp, rwTemp));
            cout << endl;

            //Display board
            displayBoard(hints, practiceGuesses);

            //Store User's guess
            cout << endl;
            cout << "Enter your next guess: ";
            cin >> guess;

            //Input validation
            //Input validation
            while ((guess.size() != nColors && guess != "-1") || (practiceGuesses.empty() && guess == "-1")) {
                if (practiceGuesses.empty() && guess == "-1") {
                    cout << "The board is empty.\n Enter a guess: ";
                    cin >> guess;
                } else {
                    cout << "Your guess isn't long enough.\nTry again: ";
                    cin >> guess;
                }
            }

            if (guess == "-1") {
                practiceGuesses.pop();
                continue;
            }
            // Add colors to the set
            for (string::iterator it = guess.begin(); it != guess.end(); it++) {
                char character = *it;
                usedColors.insert(character);
            }
            practiceGuesses.push(guess);
        }

        //Evaluate user's guess for hints
        if (guess != "-1")
            hint(code, guess, rrTemp, rwTemp);

        //Store Hints
        hints.push(make_pair(rrTemp, rwTemp));

        //Display board
        displayBoard(hints, practiceGuesses);

        //Display outro message
        outro(code, guess, nColors);
    }

    //Exit Stage Right!
    return 0;
}

//Function Implementations

//Display intro

void intro(int &n, char &dup, int &gameLen, char &modeFlag) {
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
    cout << "You can either play the game in normal mode or practice mode." << endl;
    cout << "Normal mode simply lets you play through the game with the amount";
    cout << endl << "of colors you choose and guesses you allow yourself.";
    cout << endl << "Practice mode allows you to delete previous guesses. ";
    cout << "This enables you to play until you win\nand get the hang of how";
    cout << " the game works without failing." << endl;
    cout << "To play in normal mode, press (n)" << endl;
    cout << "To play in practice mode, press (p)" << endl;
    cout << "Enter mode input: ";
    cin >> modeFlag;

    //Input Validation
    while (modeFlag != 'n' && modeFlag != 'N' && modeFlag != 'p' && modeFlag != 'P') {
        cout << "Oops you input an invalid character choice. Try again: ";
        cin >> modeFlag;
    }

    cout << "Now, let the game begin!" << endl;
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

string setCode(int n, char dup) {
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

    for (map<char, int>::const_iterator kv = codeMap.begin(); kv != codeMap.end(); kv++) {
        map<char, int>::iterator it = guessMap.find(kv->first);
        if (it != guessMap.end()) {

            rw += min(kv->second, it->second);
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

//Display game board (normal mode with queue)

void displayBoard(queue<pair<int, int>>hints, queue<string> guesses,
        const set<char>& userColors) {
    int boardLength = guesses.size();
    //Display game board
    cout << "Game Board:" << endl;
    for (int i = 0; i < boardLength; i++) {
        //Display row
        if ((i + 1) >= 10) { //Display corrected formatting for double digits
            cout << "Row " << i + 1 << ":" << setw(2);
            cout << "|" << setw(4);
        } else { //Display normal formatting
            cout << "Row " << i + 1 << ":" << setw(3);
            cout << "|" << setw(4);
        }

        //Switch the numbers to the two letter color description
        string guess = guesses.front();
        string::iterator guess_it = guess.begin();
        while (guess_it != guess.end()) {
            colors(*guess_it);
            guess_it++;
        }

        //Display the hints
        cout << "| RR: " << hints.front().first << " RW: " << hints.front().second;
        cout << endl;
        guesses.pop();
        hints.pop();
    }
}

//Display game board (practice mode with stack)

void displayBoard(queue<pair<int, int>>hints, stack<string> prevGuesses,
        const set<char>& usedColors) {
    vector<string> prevGuessesVector; // For the game board to be printed
    // properly.
    while (!prevGuesses.empty()) {
        prevGuessesVector.push_back(prevGuesses.top());
        prevGuesses.pop();
    }
    reverse(prevGuessesVector.begin(), prevGuessesVector.end());
    //Display game board
    cout << "Game Board:" << endl;
    for (int i = 0; i < prevGuessesVector.size(); i++) {
        //Display row
        if ((i + 1) >= 10) { //Display corrected formatting for double digits
            cout << "Row " << i + 1 << ":" << setw(2);
            cout << "|" << setw(4);
        } else { //Display normal formatting
            cout << "Row " << i + 1 << ":" << setw(3);
            cout << "|" << setw(4);
        }

        //Switch the numbers to the two letter color description
        string guess = prevGuessesVector[i];
        string::iterator guess_it = guess.begin();
        while (guess_it != guess.end()) {
            colors(*guess_it);
            guess_it++;
        }

        //Display the hints
        cout << "| RR: " << hints.front().first << " RW: " << hints.front().second;
        cout << endl;
        hints.pop();
    }
}

