#include "TOWERS.h"
#include <iostream>

using namespace std;

Towers::Towers() {
    // push disk 1 - 3 onto first tower
    towers[0].push_back(1);
    towers[0].push_back(2);
    towers[0].push_back(3);

    numMoves = 0;
}

Towers::~Towers() {

    // do nothing because memory should be automatically released
    // because we do not use any pointers or malloc

}

bool Towers::move(int from, int to) {

    // check if from tower is empty
    if (towers[from].empty()) return false;
    // if to tower is empty move top disk to it
    if (towers[to].empty()) {
        towers[to].push_back(towers[from].back());  // copy element
        towers[from].pop_back();                    // remove element from original tower
        numMoves++;
        return true;                                // move was successful
    }
    // if both towers have contents, check move is valid
    if (towers[to].back() > towers[from].back()) {
        towers[to].push_back(towers[from].back());
        towers[from].pop_back();  
        numMoves++;  
        return true;
    }

    return false;   // unable to perform move
}

void Towers::display() {

    cout << "displaying current puzzle content" << endl;

}

vector<int> * Towers::getTowers() { return towers; }