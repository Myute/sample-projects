#include<iostream>
#include<fstream>
#include<vector>
#include<string>
using namespace std;

//--------------------------------------
// CS421 File td.cpp for HW2B Table-Driven Scanner
// Your name: Kevyn Higbee
//--------------------------------------

// Complete this to fit the HW2B specification - look for **
// Must have the same types of tracing couts as my demo program.


// info on each DFA
struct info {
  string name;  // token name
  int startstate;                                                                                                         
  int finalstate;
};

info DFAs[4];     // store up to 4 dfas' start and final

int  TRS[10][4];  // store all trs's - states 0-9 and chars a b c d -- all dfas transitions are in here

// ----- utility functions -----------------------

int readTables() {                                                                                                                       
   ifstream fin ("trs.txt", ios::in);
   ifstream fin2 ("dfas.txt", ios::in);
   int i = 0;
   // ** Read in the files into TRS and DFAs
   while(fin && i < 10) {
    for(int x = 0; x < 4; x++) fin >> TRS[i][x];
    i++;
   }
   i = 0;
   while(fin2 && i < 4) {
     fin2 >> DFAs[i].name >> DFAs[i].startstate >> DFAs[i].finalstate;
     i++;
   }
   return i-1; // returns dfa's read
}

void displayTables(int numDFAs) {
  // ** display DFAs nicely labeled
  // ** display TRS nicely labeled
  cout << "         a b c d" << endl;
  for(int i = 0; i < 10; i++) {
    cout << "State " << i << ": ";
    for(int x = 0; x < 4; x++){
      if(TRS[i][x] >= 0) cout <<TRS[i][x] << " ";
      else cout << "  ";
    }
    cout << endl;
  }
  for(int z = 0; z < numDFAs; z++)
    cout << DFAs[z].name << ": " << DFAs[z].startstate 
         << " is start and ends in " << DFAs[z].finalstate << endl;
}

bool accept(info dfa, string word) {
  // ** Does the dfa accept the word?
  // Start with the start state of the DFA and
  // look up the next state in TRS for each char in word.
  // At the end of the word, make sure you are in the
  // final state of the DFA.
  // Use a formula to convert chars to TRS col numbers.
  int state = dfa.startstate;
  int pos =0;
  while(word[pos]!='\0') {
    state = TRS[state][word[pos]-97];
    cout << "state: " << state << " char: " << word[pos] << endl;
    pos++;
    if(state == -1)
      return false;
  }
  return (state == dfa.finalstate);
}


int main() {
  cout << "This is a table driven scanner. Needs trs.txt and dfas.txt." << endl;                                          
  cout << "States are 0 to 9 and chars are a to d" << endl;

  int numDFA = readTables(); // how many DFAs were read
  displayTables(numDFA);     // DISPLAY TABLES
  cout << ".....done reading tables...." << endl;
  string word;                                                                                                            
  while(true) {
      cout << "@@Enter a string: " ;
      cin >> word;
      int i = 0;
      while(i < numDFA) {
        if(accept(DFAs[i],word)) {
          cout << "found in " << DFAs[i].name << endl;
          i = numDFA +1;
        }
        else
          i++;
      }
      if(i==numDFA)
        cout << "Lexical Error!\n";
      // ** try the DFAs one by one and see
      //      if the word is accepted
      //      if so, display the word and the token name
      // ** if no DFA does, generate a lexical error message.
      cout << "do control-C to quit" << endl;                                                                               
    }

}//the end
