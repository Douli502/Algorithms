#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "Boggle.h"
#include "bogglemain.h"
#include "strlib.h"
#include <ctype.h>
#include <algorithm>

bool isValidInput(const string& input);
string writeInput();
void humanTurn(Boggle& boggle);
void computerTurn(Boggle& boggle);

/*
 * Plays one game of Boggle using the given boggle game state object.
 */
void playOneGame(Boggle& boggle) {
    boggle.reset();
    // Build board
    yesOrNo("Do you want to generate a random board? ")?
    boggle.shuffleBoard() : boggle.createBoard(writeInput());
    cout << "It's your turn!" << endl;
    boggle.showBoard();

    // My turn
    humanTurn(boggle);

    // Computers turn
    computerTurn(boggle);
}

/*
 * Takes input from user that is 16 letters long.
 */
string writeInput() {
    string input;
    do {
        cout << "Type the 16 letters to appear on the board: ";
        getline(cin,input);
        if(!isValidInput(input)) {
            cout << "That is not a valid 16-letter board String. Try again." << endl;
        }
    }
    while(!isValidInput(input));
    cout << input << endl;
    return input;
}

/*
 * Checks if the inputs are alphabets.
 */
bool isValidInput(const string& input) {
    if(input.size() != 16) {
        return false;
    }
    for(char c : input) {
        if(!isalpha(c)) {
            return false;
        }
    }
    return true;
}

/*
 * Users turn to input words to the in the game.
 */
void humanTurn(Boggle& boggle) {
    bool isValidWord = false;
    string input;
    do {
        cout << "Your words (" << boggle.getUsedWords().size() << "): ";
        if(!boggle.getUsedWords().empty()) {
            boggle.showUsedWord();
        }
        if(boggle.getUsedWords().empty()) {
            cout << "{}" << endl;
        }
        cout << "Your score: " << boggle.getPlayerPoints() << endl;
        cout << "Type a word (or press Enter to end your turn): ";
        getline(cin, input);
        transform(input.begin(), input.end(), input.begin(), ::toupper);
        if(input == "") {
            cout << endl;
            break;
        } else if(boggle.usedWord(input)) {
            cout << "You have already guessed that word." << endl;
        } else if(!boggle.isInLexicon(input)) {
            cout << "That word is not in the dictionary." << endl;
        } else if(input.size()<4) {
            cout << "That word is not long enough." << endl;
        } else if(!boggle.isOnBoard(input)) {
            cout << "That word can't be formed on this board." << endl;
        } else {
            cout << "You found a new word! \""+input+"\"" << endl;
            boggle.insertUsedWord(input);
            boggle.addPoints(input.size()-3);
        }
        boggle.showBoard();
    } while(!isValidWord);
}

/*
 * Computers turn to find the rest of the words.
 */
void computerTurn(Boggle& boggle) {
    boggle.findRemainingWords();
    cout << "It's my turn!" << endl
       << "My words (" << boggle.getRemainingWords().size()
       << "): ";
    boggle.showRemainingwords();
    cout << "My score: " << boggle.getComputerPoints() << endl;
    if(boggle.getComputerPoints() > boggle.getPlayerPoints()) {
        cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!" << endl;
    } else {
        cout << "WOW, you defeated me! Congratulations!" << endl;
    }
}

void clearConsole() {
#if defined(_WIN32) || defined(_WIN64)
    std::system("CLS");
#else
    // assume POSIX
    std::system("clear");
#endif
}
