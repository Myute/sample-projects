#include <vector>
#include <iostream>

using namespace std;

class Towers {

    private:
        vector<int> towers[3];
        int numMoves;

    public:
        // Constructors
        Towers();       // Default Constructor for a three disk tower
        ~Towers();      // Deconstructor
        // Function Prototypes
        bool move(int, int);        // moves the top disk from the first specified tower to the second specified tower
        void display();             // prints towers to console
        vector<int> * getTowers();  // returns the towers
};