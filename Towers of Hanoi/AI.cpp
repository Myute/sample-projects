#include "AI.h"
#include <algorithm>
#include <iostream>

using namespace std;

AI::AI() { /* nothing to do here */ }
AI::~AI() { /* nothing to do here */ }

// compares the passed State to the goal state, returns true if matches, else false
bool AI::goal(State s) {
    
    return (s.towers[2][0] == 3 && s.towers[2][1] == 2 && towers[2][2] == 1);

}

// function for calculating the heuristic value for a given state
// equation: h = (#disks not on tower 3) + (2 * #disks stacked in non sequential order).
int AI::hFunc(State s) {

    int h = 0;          // h value defaulted to 0
    int last = -1  ;    // for checking if stacked values are sequential or if a disk is skipped
    for (int i = 0; i < 3; i++) {
        for ( int x = 0; x < s.towers[i].length(); x++ ) {
            if ( i < 2 ) h++;                                   // only count disks on towers 1 & 2
            if ( last != -1 && last-1 != s.towers[i][x]) h+=2;  // check if there are skipped disks, add 2 to h for each found
            last = s.towers[i][x];
        }
        last = -1;      // reset last for next iteration
    }
}

// returns the state in the frontier with the best f
State AI::bestInFrontier() {

    if ( !frontier.empty()) {       // as long as frontier is not empty
        State best = frontier[0];       // start with first value as default best
        for( State tmp : frontier ) {   // compare f of each state in frontier
            if ( tmp.f < best.f )       // switch best if a better f is found
                best = tmp;
        }
    }
    return best;
}

// overloaded equality operator for utility
bool AI::operator==(const State& lhs, const State& rhs) {

    // if any of the towers are a different length, states cannot be equal
    // and return false
    for ( int i = 0; i < 3; i++ ) {
        if(lhs.towers[i].length() != rhs.towers[i].length)
            return false;
        else {
            // for all non empty towers, compare values, return false
            // if any non matching values are found
            if ( !lhs.towers[i].empty() ) {                     // we only need to check left length because previous check
                for(int z = 0; z < lhs.towers[i].length(); z++) // ensured that each tower of a matching index is the same length
                    if ( lhs.towers[i][z] != rhs.towers[i][z] )
                        return false;
            }
        }
    }

    return true;    // nothing was flagged as wrong   
}

// utility function: returns index of a state with matching contents inside of the passed vector
int AI::getIndex(State s, vector<State> vec) {
    
    int i = 0;
    for( State tmp : vec) {
        if ( tmp == s ) return i;
        else i++;
    }

    return -1;
}

// returns true if a state in the frontier has the same contents as the passed state
// else false
bool AI::inFrontier(State s) {
    for (State tmp : frontier) {
        if( s == tmp )
            return true;
    }
    return false;
}

// returns true if a state in the expanded has the same contents as the passed state
// else false
bool AI::inExpanded(State s) {
    for (State tmp : expanded) {
        if( s == tmp )
            return true;
    }
    return false;
}

// checks if a state is already in the frontier, if not adds it
// if a state with matching contents is found, compares g/f/parent
// and upadates values if necessary
void AI::addToFrontier(State s) {

    if(!inFrontier(s))
        frontier.push_back(s);
    else {
        int i = getIndex(s,frontier);
        if(frontier[i].g > s.g) {
            frontier[i].parent = s.parent;
            frontier[i].g = s.g;
            frontier[i].f = s.f;
        }
    }
    
}

// erases the passed state from the frontier
void AI::removeFromFrontier(State s) {

    frontier.erase(frontier.begin() + getIndex(s, frontier));
}

// compares state to expanded version and updates successors if necessary
void AI::propogate(State s) {

    int index = getIndex(s,expanded);
    if (expanded[index].g > s.g) {
        expanded[index].g = s.g;
        expanded[index].f = s.f;
        expanded[index].parent = s.parent;

        // dfs of all generated nodes starting with s
        // check nodes in frontier for any that trace back to this as a parent
        // for each that does, compare g, update g/f if necessary



    }

}
// generates any valid successors from passed state
// new successors will be inserted/updated into the frontier
// ensures an expanded node is not being readded
void AI::generate(State s) {

    State newState = s;
    // attempt adding new state from move 1 into frontier
    if ( !s.towers[0].empty() && s.towers[0].back() < s.towers[1].back() ) {
        newState.towers[1].push_back(s.towers[0].back());
        newState.towers[0].pop_back();
        newState.g++;
        newState.h = hFunc(newState);
        newState.f = newState.g + newState.h;
        newState.parent = s;
        
        if(!inExpanded(newState)) addToFrontier(newState);
        else propogate(newState);

        newState = s;
    }
    // attempt adding new state from move 2 into frontier
    if ( !s.towers[0].empty() && s.towers[0].back() < s.towers[2].back() ) {
        newState.towers[2].push_back(s.towers[0].back());
        newState.towers[0].pop_back();
        newState.g++;
        newState.h = hFunc(newState);
        newState.f = newState.g + newState.h;
        newState.parent = s;
        addToFrontier(newState);
        newState = s;
    }
    // attempt adding new state from move 3 into frontier
    if ( !s.towers[1].empty() && s.towers[1].back() < s.towers[0].back() ) {
        newState.towers[0].push_back(s.towers[1].back());
        newState.towers[1].pop_back();
        newState.g++;
        newState.h = hFunc(newState);
        newState.f = newState.g + newState.h;
        newState.parent = s;
        addToFrontier(newState);
        newState = s;
    }
    // attempt adding new state from move 4 into frontier
    if ( !s.towers[1].empty() && s.towers[1].back() < s.towers[2].back() ) {
        newState.towers[2].push_back(s.towers[1].back());
        newState.towers[1].pop_back();
        newState.g++;
        newState.h = hFunc(newState);
        newState.f = newState.g + newState.h;
        newState.parent = s;
        addToFrontier(newState);
        newState = s;
    }
    // attempt adding new state from move 5 into frontier
    if ( !s.towers[2].empty() && s.towers[2].back() < s.towers[0].back() ) {
        newState.towers[0].push_back(s.towers[0].back());
        newState.towers[2].pop_back();
        newState.g++;
        newState.h = hFunc(newState);
        newState.f = newState.g + newState.h;
        newState.parent = s;
        addToFrontier(newState);
        newState = s;
    }
    // attempt adding new state from move 6 into frontier
    if ( !s.towers[2].empty() && s.towers[2].back() < s.towers[1].back() ) {
        newState.towers[1].push_back(s.towers[2].back());
        newState.towers[2].pop_back();
        newState.g++;
        newState.h = hFunc(newState);
        newState.f = newState.g + newState.h;
        newState.parent = s;
        addToFrontier(newState);
        newState = s;
    }
}

// displays path
void AI::tracepath(State s) {
    
    cout << endl << "Tower 1: ";
    for(int x : s.towers[0]) cout << x << " ";
    cout << endl << "Tower 2: ";
    for(int x : s.towers[1]) cout << x << " ";
    cout << endl << "Tower 3: ";
    for(int x : s.towers[2]) cout << x << " ";

    cout << endl << "came from:";
    tracepath(s.parent);
}

// loop for a* algorithm
void AI::aStar(Towers t) {

    // create state from tower
    State initial;
    initial.parent=nullptr;
    initial.towers=t.getTowers();
    initial.g=0;
    initial.h=hFunc(initial);
    initial.f = initial.h;
    frontier.push_back(initial);

    State best = bestInFrontier;

     while ( !goal(best) ) {
          
        // expand current node
        generate(best);
        // add current node to expanded
        expanded.push_back(best);
        removeFromFrontier(best);
        // compare generated successors
        best = bestInFrontier();
    }
    cout << "Tracing Path: " << endl;
    tracepath(best);

}