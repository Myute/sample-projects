//----------------------------------------------
// CS421 HW1
// Write a generator in C++ for L = {x | x is a binary number}.
// Your name: Kevyn Higee
//----------------------------------------------

#include <iostream>
#include <string>
#include <queue>

using namespace std;

bool recognizer(string);    // prototype for recongizer() function, copied from Recognizer.cpp

// main: It should create each string over E = {0,1,2} systematically
//   (short to long) and pass each string to the recognizer function
//   created in Part a).  Hint: use a queue to generate strings
//   Only those strings for which the recognizer returned TRUE
//   should be displayed.
//   Keeps on going until the queue overflows but the user can
//   terminate the program with control-C
//   after about 20 strings have been displayed.

int main() {
    int successful = 0;             // declare variables
    string str = "\0";
    queue<string> q;

    q.push("0");                    // initial queue contents
    q.push("1");
    q.push("2");
    
    while(true) {
        str = q.front();            // grab string from queue
        q.pop();            
        q.push(str + "0");          // add next strings to queue
        q.push(str + "1");
        q.push(str + "2");
                                    
        if(recognizer(str)) cout << str << " is binary " << endl;
    }
    return 0;
}

// recognizer() definition
bool recognizer(string userStr) {
    int i = 0;
    while(i < userStr.length())                               // parse eacch character and check if value is a 0 or 1
       if ((userStr[i] == '1')||(userStr[i] == '0')) i++;     // return false if value is not allowed
       else return false;
    return true;
}