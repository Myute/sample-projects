#pragma once

#include "TOWERS.h"
#include <vector>

using namespace std;

struct State {
    // contents of the towers for this state
    vector<int> towers[3];
    // cost variables
    int g;
    int h;
    int f;
    State *parent;   // previous state
};

class AI {

    private:
        vector<State> frontier;     // for generating new states
        vector<State> expanded;     // for expanded states
      
    public:
        // Constructors
        AI();
        ~AI();

        // Function Prototypes
        bool done(State);
        void aStar(Towers);
        int hFunc(State);
        void generate(State);
        State bestInFrontier();
        void addToFrontier(State);
        bool inFrontier(State);
        bool inExpanded(State);
        void removeFromFrontier(State);
        int getIndex(State, vector<State>);
        void tracepath(State s);
        void propogate(State s);
        bool operator==(const State& lhs, const State& rhs);
};