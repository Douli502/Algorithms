#include <sstream>
#include "Boggle.h"
#include "random.h"
#include "shuffle.h"
#include "strlib.h"
#include <unordered_set>
#include <algorithm>
#include <string>

static const int NUM_CUBES = 16;   // the number of cubes in the game
static const int CUBE_SIDES = 6;   // the number of sides on each cube
static const set<int> neighbours = {1,3,4,5,-1,-3,-4,-5}; // the number for calculating neighbours
static string CUBES[NUM_CUBES] = {        // the letters on all 6 sides of every cube
   "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
   "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
   "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
   "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

/*
 * Shuffles the board using Stanford shuffle function.
 */

void Boggle::shuffleBoard() {
    for(int i = 0; i < NUM_CUBES; i++) {
        char c = CUBES[i].at(rand()%CUBE_SIDES);
        board[i/BOARD_SIZE][i%BOARD_SIZE] = c;
    }
    shuffle(board);
}

/*
 * Creates board after characters have been created.
 */
void Boggle::createBoard(string input) {
    for(int i = 0; i < NUM_CUBES; i++) {
        board[i/BOARD_SIZE][i%BOARD_SIZE] = toupper(input.at(i));
    }
}

/*
 * Resets the board completely.
 */
void Boggle::reset() {
    usedWords = {};
    remainingWords = {};
    computerPoints =0;
    playerPoints = 0;
    board = Grid<char>(BOARD_SIZE,BOARD_SIZE);
}

/*
 * Prints the board.
 */
void Boggle::showBoard() {
    for(int row = 0; row < BOARD_SIZE; row++) {
        for(int col = 0; col < BOARD_SIZE; col++) {
            cout << board[row][col];
        }
        cout << endl;
    }
    cout << endl;
}

bool Boggle::usedWord(string& input) const {
    return usedWords.count(input);
}

void Boggle::insertUsedWord(string& input) {
    usedWords.insert(input);
}

set<string> Boggle::getUsedWords() const{
    return usedWords;
}
set<string> Boggle::getRemainingWords() const{
    return remainingWords;
}

int Boggle::getPlayerPoints() const {
    return playerPoints;
}
int Boggle::getComputerPoints() const {
    return computerPoints;
}
void Boggle::addPoints(int points) {
    playerPoints+= points;
}

/*
 * Shows all the remaining words that can be found on the board.
 */
void Boggle::showRemainingwords() const {
    string output  = "{";
    if(!remainingWords.empty()) {
        for(auto word: remainingWords) {
            output.append("\"");
            output.append(word);
            output.append("\", ");
        }
        output.pop_back();
        output.pop_back();
        output += "}";
        cout << output << endl;
    }
    else{
        cout << "{}" << endl;
    }
}

/*
 * Checks if word exists in lexicon.
 */
bool Boggle::isInLexicon(string& input) const {
    return wordlist.contains(input);
}

/*
 * Prints out the words that have been found.
 */
void Boggle::showUsedWord() const{
    string output  ="{";
    for(auto word: usedWords) {
        output.append("\"");
        output.append(word);
        output.append("\", ");
    }
    output.pop_back();
    output.pop_back();
    output+="}";
    cout << output << endl;
}

/*
 * Checks if the word exists on the given board.
 */
bool Boggle::isOnBoard(string& word) const {
    string temp = word;
    temp.pop_back();
    for(int i = 0; i < NUM_CUBES;i++) {
        if(word.back() == board[i/BOARD_SIZE][i%BOARD_SIZE]) {
            set<int> visited = {i};
            return findWord(temp,i,visited);
        }
    }
    return false;
}

/*
 * Checks each letter of the word given if it can be
 * made into a word in the board.
 */
bool Boggle::findWord(string& word, int curIndex, set<int>& visited) const{
    if(word.empty()) {
        return true;
    }
    for(int i : neighbours) {
        int neighbor = curIndex+i;
        if(isNeighbour(curIndex,i)){
            if(!visited.count(neighbor)) {
                set<int> visitedCopy = visited;
                visitedCopy.insert(neighbor);
                string wordCopy = word;
                wordCopy.pop_back();
                if(findWord(wordCopy,neighbor,visitedCopy)) {
                    return true;
                }
            }
        }
    }
    return false;
}

/*
 * Finds all the remaining available words on the board.
 */
void Boggle::findRemainingWords() {
    for(int i = 0; i < NUM_CUBES; i++) {
        set<int> visited = {i};
        string firstLetter = "";
        firstLetter.push_back(board[i/BOARD_SIZE][i%BOARD_SIZE]);
        detectWords(firstLetter,i,visited);
    }
}

/*
 * Checks the neighours each word and excludes corners out of bounds.
 */
bool Boggle::isNeighbour(int curIndex, int i) const {
    int neighbor = curIndex + i;
    if(neighbor < 0 || neighbor > 15) {
        return false;
    } else if(curIndex%4 == 0 && (i == 3 || i == -5 || i == -1)) {
        return false;
    } else if(curIndex%4 == 3 && (i == -3 || i == 5 || i == 1)) {
        return false;
    }
    return true;
}

/*
 * Detects the words on the board, checking all neighbours of each letter.
 */
void Boggle::detectWords(string& word, int curIndex, set<int>& visited) {
    if(!wordlist.containsPrefix(word)) {
        return;
    }
    if(wordlist.contains(word) && word.size() > 3 && !remainingWords.count(word)
            && !usedWords.count(word)) {
        remainingWords.insert(word);
        computerPoints += word.size() - 3;
    }
    for(int i : neighbours) {
        int neighbour = curIndex + i;
        if(isNeighbour(curIndex, i)){
            if(!visited.count(neighbour)) {
                set<int> visitedCopy = visited;
                string wordCopy = word;
                visitedCopy.insert(neighbour);
                wordCopy += board[neighbour/BOARD_SIZE][neighbour%BOARD_SIZE];
                detectWords(wordCopy, neighbour, visitedCopy);
            }
        }
    }
}

