//-------------------------------------------------------
// CS421 HW1
// Write a recognizer in C++ for L = {x | x is a binary number}.
// Your name: Kevyn Higbee
//-------------------------------------------------------

#include <iostream>
#include <string>

using namespace std;

bool recognizer(string);    // returns true when string is comprised of 0's and/or 1's, else false

//main: Given a string (from E) cined from the user, pass it to
//      the recognizer function.
//      Cout "YES IN L" or "NO NOT IN L" depending on what was returned.
int main() {
    // variable
    string inputStr = "\0";
    // get user input
    do {
        cout << "Enter a string consisting of numbers or 'Q' to Exit: ";
        cin >> inputStr;
        // print correct output
        if (inputStr != "Q")
            (recognizer(inputStr)) ? cout << "YES IN L" << endl : cout << "NO NOT IN L" << endl;
    }
    while(inputStr != "Q");

    return 0;
}

//  The recognizer function should return TRUE or FALSE
//  checking each character
//  to make sure it is 0 or 1. (leading 0's are allowed).
bool recognizer(string userStr) {
    int i = 0;
    while(i < userStr.length())                            // parse each character and check if value is a 0 or 1
       if ((userStr[i] == '1')||(userStr[i] == '0')) i++;  // return false if value is not allowed
       else return false;
    return true;
}



