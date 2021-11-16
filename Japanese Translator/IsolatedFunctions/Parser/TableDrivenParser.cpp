#include<iostream>
#include<stdlib.h>
#include<fstream>
#include<vector>
#include<string>
#include<stack>

using namespace std;

vector<char> M[3][2];
stack<char> ruleStack;

int toRow(char c) {
  if(c == 'S') return 0;
  else if(c == 'A') return 1;
  else if(c == 'B') return 2;
  else return -1;
}
int toCol(char c) {
  return c - '0';
}
char toNonterm(int r) {
  if(r == 0) return 'S';
  else if(r == 1) return 'A';
  else if(r == 2) return 'B';
  else return -1;
}
void displayVector(vector<char> v)  {
  for(char c: v) {
    cout << c << " ";
  }
  cout << "\t";
}

void readrules() { 
  ifstream fin ("rules", ios::in);
  char tmp,left,last = ' ';
  vector<char> right;
  int col = 0;
  while(fin >> left) { 
    fin >> tmp;
    col = toCol(tmp);
    while(tmp != ';') {
      M[toRow(left)][col].push_back(tmp);
      fin >> tmp;
    } 
  }
  for(int x = 0; x < 3; x++) {
    cout << toNonterm(x) << ":   ";
    for(int y = 0; y < 2; y++) { displayVector(M[x][y]); }
    cout << endl;
  }
}

void addtostack(vector<char> v) {  
  cout << "Pushing rhs of a rule to the stack." << endl;
  for(char c : v) {
    ruleStack.push(c);
  }
}

int main() { 
  readrules();

  string ss;
  cout << "Enter a string made of 0's and/or 1's: ";
  cin >> ss;
  
  ruleStack.push('S');
  cout << "Stack: " << ruleStack.top() << endl;
  
  int i = 0;
  while (ss[i] != '\0' && !ruleStack.empty())  {
    cout << "current char is: " << ss[i] << endl;
    char rule = ruleStack.top();
    if(ss[i] == '0' || ss[i] == '1') {
      switch(rule) {
        case 'S':
          ruleStack.pop();
          addtostack(M[toRow(rule)][toCol(ss[i])]);
          i--;
          break;
        case 'A':
          ruleStack.pop();
          addtostack(M[toRow(rule)][toCol(ss[i])]);
          i--;
          break;
        case '0':
          if(ss[i] == rule) {
            cout << "Matched!" << endl;
            ruleStack.pop();
          }
          break;
        case '1':
          if(ss[i] == rule) {
            cout << "Matched!" << endl;
            ruleStack.pop();
          }
          break;
      }
    }
  else break;
  cout << endl << "Stack:" << endl;
  stack<char> tmp = ruleStack;
  while(!tmp.empty()) {
    cout << tmp.top() << endl;
    tmp.pop();
  }
  i++;
  } 

  if(ruleStack.empty() && i == (ss.length())) cout << endl << "Accept!" << endl; 
  else cout << endl << "Reject!" << endl; 
}
