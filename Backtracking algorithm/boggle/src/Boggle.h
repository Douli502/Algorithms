/*
 * TDDD86 Boggle
 * This file contains implementation of boggle functions that operates on
 *  a grid of size 4x4.
 */

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include <grid.h>
#include <set>
#include <lexicon.h>
using namespace std;

class Boggle {
public:
    const string DICTIONARY_FILE = "EnglishWords.dat";
    const int MIN_WORD_LENGTH = 4;
    const int BOARD_SIZE = 4;

    /*
     * Shuffles a board
     */
    void shuffleBoard();

    /*
     * Shows the board
     */
    void showBoard();

    /*
     * Creates a random board or given board
     */
    void createBoard(string input);

    /*
     * Returns true if input exist in lexicon
     */
    bool isInLexicon(string& input) const;

    /*
     * Returns true if input has already been used
     */
    bool usedWord(string& input) const;

    /*
     * Returns true if word is in board
     */
    bool isOnBoard(string& word) const;

    /*
     * Insert input in set<string> usedWords
     */
    void insertUsedWord(string& input);

    /*
     * Returns true if word is found in the board
     */
    bool findWord(string& word, int curIndex, set<int>& visited) const;

    /*
     *  Get set<string> usedWords
     */
    set<string> getUsedWords() const;

    /*
     * Show  set<string> usedWords
     */
    void showUsedWord() const;

    /*
     * Show  set<string> remainingWords
     */
    void showRemainingwords() const;

    /*
     * Get player points
     */
    int getPlayerPoints() const;

    /*
     * Get computer points
     */
    int getComputerPoints() const;

    /*
     * Finds remaining words and insert to set<string> remaingingWords
     */
    void findRemainingWords();

    /*
     * Returns true if neighbour exist
     */
    bool isNeighbour(int curIndex, int i) const;

    /*
     * Get set<string> remainingWords
     */
    set<string> getRemainingWords() const;

    /*
     * Detecs any words it comes acorss in the board
     */
    void detectWords(string& word,int curIndex,set<int>& visited);

    /*
     * Add points to the player
     */
    void addPoints(int points);

    /*
     * Resets all the private variables
     */
    void reset();
private:
    Grid<char> board;
    Lexicon wordlist = Lexicon(DICTIONARY_FILE);
    set<string> usedWords;
    set<string> remainingWords;
    int playerPoints = 0;
    int computerPoints = 0;
};

#endif
