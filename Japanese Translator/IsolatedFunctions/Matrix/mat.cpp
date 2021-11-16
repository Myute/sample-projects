#include<iostream>
#include<fstream>
#include<vector>
using namespace std;

//--------------------------------------
// CS421 File mat.cpp for HW2A Matrix Practice
// Your name: Kevyn Higbee
//--------------------------------------

// the following can be changed and the rest of the code should work
int const R = 3;  // number of rows
int const C = 4;  // number of columns
int const VM = 3; // table entry (vector) length

vector<char>  T[R][C];  // 3 rows and 4 columns table for now

// ------------------ Function Prototypes --------------------------------
int convert(char);
int readTable();
void showVector(vector<char>);
void displayTable();

// ------------------ Driver ---------------------------------------
int main() {
  cout << "Reading table from lines.txt..." << endl;
  readTable();
  displayTable();
}


// ------------------ Function Definitions --------------------------------

int convert(char x) {
    return x-97;    // returns offset from 'a' on ascii table
}
        
int readTable() {
   int row, col;   // row and col numbers
   char col_c;     // column indicator as read from file
   ifstream fin ("lines.txt", ios::in); // file input buffer

    // Read in the file into T
    while (fin >> row) {
        fin >> col_c; 
        col = convert(col_c);
        vector<char> v;  // a vector to fill
        char c;          // one char from the file

        for(int i = 0; i < 3; i++) {    // add characters into vector
            fin >> c;
            if(c == ' ') i--;   // don't read blankspace
            else v.push_back(c);
        }
        T[row][col] = v;    // add filled vector into matrix
    }
    return 0;
}

// Displays a vector or "---" if the vector is empty
void showVector(vector<char> v) { 
    cout << "| ";   // prefix for a nice looking table
    if (v.size() == 0) // on empty entry
        for(int i = 1; i <= VM; i++) cout << "- ";
    else // on filled entry
        for(int i = 1; i <= VM; i++) cout << v[i-1] << " ";
}

// Displays T as a table, separating entries with "|"
void displayTable() {
    for(int rows = 0; rows < R; rows++) {
        cout << rows;
        for(int cols = 0; cols < C; cols++) 
            showVector(T[rows][cols]);
        cout << endl;
    }
}