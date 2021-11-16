#include<iostream>
#include<fstream>
#include<string>
using namespace std;

/* INSTRUCTION:  Complete all ** parts.
   You may use any method to connect this file to scanner.cpp
   that you had written.  
  e.g. You can copy scanner.cpp here by:
          cp ../ScannerFiles/scanner.cpp .  
       and then append the two files into one: 
          cat scanner.cpp parser.cpp > myparser.cpp
*/


void story();
void connector();
void subject();
void verb();
void tense();
void be();
void noun();
void afternoun();
void object();
void afterobject();
void destination();
void period();

/// globals ======================================
enum tokentype { 
  ERROR, EOFM, PERIOD, WORD1, WORD2, 
  VERB, VERBNEG, VERBPAST, VERBPASTNEG, 
  IS, WAS, PRONOUN, CONNECTOR, OBJECT, 
  SUBJECT, DESTINATION 
};
struct reservedWord {
  string word; // the word (string)
  tokentype type; // the type (tokentype)
  // constructor for struct that assigns word/type
  reservedWord(string w, tokentype t) {
    word = w;
    type = t;
  }
};

enum state{     
Q0,Q01,QY0,QC,QS,QSA,QT,QY,QERROR,
}; // no Q1 because there is no way to get to Q1

// ** For the display names of tokens - must be in the same order as the tokentype.
string tokenName[] = { 
  "ERROR", "EOFM", "PERIOD", "WORD1", "WORD2",
  "VERB", "VERBNEG", "VERBPAST", "VERBPASTNEG",
  "IS", "WAS", "PRONOUN", "CONNECTOR", "OBJECT",
  "SUBJECT", "DESTINATION"
}; 

// ** Need the reservedwords table to be set up here.
// we used an array of struct reservedWord rather than a string[][] because we wanted to easily access the enums from each reserved word
reservedWord reservedWords[] = {
  reservedWord("masu", VERB), // use reservedWord struct's constructor as array element
  reservedWord("masen", VERBNEG),
  reservedWord("mashita", VERBPAST),
  reservedWord("masendeshita", VERBPASTNEG),
  reservedWord("desu", IS),
  reservedWord("deshita", WAS),
  reservedWord("o", OBJECT),
  reservedWord("wa", SUBJECT),
  reservedWord("ni", DESTINATION),
  reservedWord("watashi", PRONOUN),
  reservedWord("anata", PRONOUN),
  reservedWord("kare", PRONOUN),
  reservedWord("kanojo", PRONOUN),
  reservedWord("sore", PRONOUN),
  reservedWord("mata", CONNECTOR),
  reservedWord("soshite", CONNECTOR),
  reservedWord("shikashi", CONNECTOR),
  reservedWord("dakara", CONNECTOR),
  reservedWord("eofm", EOFM)
};

tokentype saved_token;
string saved_lexme;
string filename;
bool token_available;

// end globals ==================================


//=====================================================
// File scanner.cpp written by: Group Number: 3 
//=====================================================

// --------- Two DFAs ---------------------------------

// WORD DFA 
// Done by: Group 3
// RE: (vowel | vowel n | consonant vowel | consonant vowel n | consonant-pair vowel | consonant-pair vowel n)^+
// enum for states in word DFA (could use int, but just for my sanity)

// done by: Abdulla Azim, Kevyn Higbee, Preston Roberts
bool word (string s)
{
  int i = 0;
  state state = Q0;
  char cur = s[i]; // set 'cur' character to first character of word
  /* replace the following todo the word dfa  **/
  while (cur != '\0') 
  {
    //cout << "|_|" << state << "|_|" << endl; was for testing
    switch(state) // switch statement based on case (looked better than if statements)
    {
      case Q0: case QY0:
        switch(cur) // switch statement based on 'cur' = current character (looked better than if statements)
        {
          case 'n': case 'h': case 'b': case 'm': case 'k': case 'p': case 'r': case 'g':
            state = QY;
            break;
          case 'y': case 'd': case 'w': case 'z': case 'j':
            state = QSA;
            break;
          case 'a': case 'i': case 'u': case 'e': case 'o': case 'E': case 'I': // vowels
            state = Q01;
            break;
          case 's':
            state = QS;
            break;
          case 'c':
            state = QC;
            break;
          case 't':
            state = QT;
            break;
          default: // default to QERROR if char is outside of language 
            state = QERROR;
            break;
        }
        break;
      case Q01:
        switch(cur)
        {
          case 'n':
            state = QY0;
            break;
          case 'h': case 'b': case 'm': case 'k': case 'p': case 'r': case 'g':
            state = QY;
            break;
          case 'y': case 'd': case 'w': case 'z': case 'j':
            state = QSA;
            break;
          case 'a': case 'i': case 'u': case 'e': case 'o': case 'E': case 'I': // vowels
            state = Q01;
            break;
          case 's':
            state = QS;
            break;
          case 'c':
            state = QC;
            break;
          case 't':
            state = QT;
            break;
          default:
            state = QERROR;
            break;
        }
        break;
      case QC:
        switch(cur)
        {
          case 'h':
            state = QSA;
            break;
          default:
            state = QERROR;
            break;
        }
        break;
      case QS:
        switch(cur)
        {
          case 'u': case 'a': case 'i': case 'e': case 'o': case 'E': case 'I': // vowels
            state = Q01;
            break;
          case 'h':
            state = QSA;
            break;
          default:
            state = QERROR;
            break;
        }
        break;
      case QSA:
        switch(cur) {
          case 'u': case 'a': case 'i': case 'e': case 'o': case 'E': case 'I': // vowels
            state = Q01;
            break;
          default:
            state = QERROR;
            break;
        }
        break;
      case QT:
        switch(cur) {
          case 'u': case 'a': case 'i': case 'e': case 'o': case 'E': case 'I': // vowels
            state = Q01;
            break;
          case 's':
            state = QSA;
            break;
          default:
            state = QERROR;
            break;
        }
        break;
      case QY:
        switch(cur) {
          case 'u': case 'a': case 'i': case 'e': case 'o': case 'E': case 'I': // vowels
            state = Q01;
            break;
          case 'y':
            state = QSA;
            break;
          default:
            state = QERROR;
            break;
        }
        break;
      default:
        state = QERROR;
        break;
    }
    i++;
    cur = s[i];
  }//end of while
  // where did I end up????
  return (state == Q01 || state == QY0); // return boolean where true for if you landed on a final state
}

// PERIOD DFA 
// Done by: Abdulla Azim, Kevyn Higbee, Preston Roberts
// RE: \.{1}
bool period (string s)
{
  return (s == "."); // return true if you've hit a period
}

// ------ Three  Tables -------------------------------------

// TABLES Done by: Abdulla Azim, Kevyn Higbee, Preston Roberts

// ** Update the tokentype to be WORD1, WORD2, PERIOD, ERROR, EOFM, etc.

// struct to make the word-type pairs for reservedWords doable without other libraries




// ** Do not require any file input for this. Hard code the table.
// ** a.out should work without any additional files.


// ------------ Scanner and Driver ----------------------- 

ifstream fin;  // global stream for reading from the input file

// Scanner processes only one word each time it is called
// Gives back the token type and the word itself
// Done by: Abdulla Azim, Kevyn Higbee, Preston Roberts
void scanner(tokentype& tt, string& w)
{
  cout << "Scanner called using word: " << w << endl;
  // initizlie tt
  tt = ERROR;
  fin >> w;   // Grab the next word from the file via fin
  
  // 1. If it is eofm, return right now.   
  if(w == "eofm") {  // check if word is end of file marker
    tt = EOFM;
    return;
  }  
  
  // 2. Call the token functions (word and period) 
  // one after another (if-then-else).
  // Generate a lexical error message if both DFAs failed.
  // Let the tokentype be ERROR in that case.
  // 3. If it was a word,
  // check against the reservedwords list.
  // If not reserved, tokentype is WORD1 or WORD2
  // decided based on the last character.
  if(word(w)){                                  // check if word is valid in word DFA
    for( reservedWord word : reservedWords){    // compare w string with reserved words
      if(w == word.word){                       // if w is a reserved word, give appropriate type (check against struct word value)
        tt = word.type;                         // since the word is in the reserved words list then match the type (via struct type value)
        //cout << w << "|_|" << word.word << "|_|" << word.type << endl; // testing
        return;
      }
    }
    // check for either word1/word2
    char last = w.back();
    if(last == 'a' || last == 'e' || last == 'i' || last == 'o' || last == 'u' || last == 'n')  tt = WORD1;  // check WORD1
    else if (last == 'E' || last == 'I') tt = WORD2;                                          // check WORD2
    else tt = ERROR;                             // if it is a word, but not WORD1 or WORD2 or a reservedWord then, type is ERROR
    return;       // exit function
  }
  else if(period(w)) tt= PERIOD; // token type period
  else { tt = ERROR; cout << "LEXICAL ERROR: " << w << " is not a valid token" << endl; } // error output
  return;
  // 4. Return the token type & string  (pass by reference)      
} //the end of scanner


// ================================================================ SCANNER END ================================================================//


//=================================================
// File parser.cpp written by Group Number: 3
//=================================================

// ----- Four Utility Functions and Globals -----------------------------------

// ** Need syntaxerror1 and syntaxerror2 functions (each takes 2 args)
//    to display syntax error messages as specified by me.  

// Type of error: expected token
// Done by: Kevyn Higbee
void syntaxerror1(tokentype exp_token, string lexme) {
  cout << endl << "SYNTAX ERROR: expected " << tokenName[exp_token] << " but found " << lexme << " (" << tokenName[saved_token] << ")" << endl;
}

// Type of error: unexpected token
// Done by: Kevyn Higbee
void syntaxerror2(tokentype component, string lexme) {
  cout << endl << "SYNTAX ERROR: unexpected " << lexme << " in " << tokenName[component] << endl;
  exit(2);
}

// ** Need the updated match and next_token with 2 global vars
// saved_token and saved_lexeme

// Purpose: Obtainining the next token type
// Done by: Abdulla Azim
tokentype next_token() {

  if(!token_available) { //check to see if we should be consuming (depending on match)
    scanner(saved_token,saved_lexme); //save+consume next scanner input
    token_available = true; //need to check the token
  }

  return saved_token; //return with current token
}

// Purpose: Matching the expected token type
// Done by: Abdulla Azim
bool match(tokentype expected) {
  if (next_token() != expected) { // if you did not match
    // you have not matched the token, and will not consume more until matched
    return false; // no match
  } 
  else{ // you matched the token
    token_available = false; // you have consumed the token, so set available to false so scanner can consume again
    //cout << endl << "TOKEN: " << saved_lexme << " | " << tokenName[saved_token] << endl; // for testing
    return true; // matched!
  }

}

// ----- RDP functions - one per non-term -------------------

// ** Make each non-terminal into a function here
// ** Be sure to put the corresponding grammar rule above each function
// ** Be sure to put the name of the programmer above each function


// Grammar: <s> ::= [CONNECTOR] <noun> SUBJECT <after subject>
// Done by: Kevyn Higbee
void story() {
  cout << "Processing <s>\n";
  if(match(CONNECTOR))
  {
    connector();
  }
  else if(match(PRONOUN))
  {
    noun();
  }
  else if(match(WORD1))
  {
    noun();
  }
  else {
    syntaxerror1(CONNECTOR, saved_lexme);
  }
}

// Grammar: <connector> ::= <noun> {}
// Done by: Kevyn Higbee
void connector() {
  cout << "Processing <connector>\n";
  if(match(PRONOUN))
  {
    noun();
  }
  else if(match(WORD1))
  {
    noun();
  }
  else {
    syntaxerror1(WORD1, saved_lexme);
  }
}

// Grammar: <subject> ::= <verb> <tense> PERIOD | <noun> {} | <afternoun> {}
// Done by: Kevyn Higbee
void subject() {
  cout << "Processing <subject>\n";
  if(match(WORD1)) {
    afternoun();
  }
  else if(match(WORD2)) {
    verb();
  }
  else if(match(PRONOUN)) {
    afternoun();
  }
  else {
    syntaxerror1(WORD1, saved_lexme);
  }
}

// Grammar: <verb> ::= WORD2
// Done by: Kevyn Higbee
void verb() {
  cout << "Processing <verb>\n";
  if(match(VERBPAST)){
    tense();
  }
  else if(match(VERBPASTNEG)) {
    tense();
  }
  else if(match(VERBNEG)) {
    tense();
  }
  else if(match(VERB)) {
    tense();
  }
  else {
    syntaxerror1(VERB, saved_lexme);
  }
}

// Grammar: <tense> ::= VERBPAST | VERBPASTNEG | VERB | VERBNEG
// Done by: Kevyn Higbee
void tense() {
  cout << "Processing <tense>\n";
  if(match(PERIOD))
  {
    period();
  }
  else {
    syntaxerror1(PERIOD, saved_lexme);
  }
}

// Grammar: <be> ::= IS | WAS
// Done by: Kevyn Higbee
void be() {
  cout << "Processing <be>\n";
  if(match(PERIOD))
  {
    period();
  }
  else {
    syntaxerror1(PERIOD,saved_lexme);
  }
}

// Grammar: <noun> ::= WORD1 | PRONOUN
// Done by: Preston Roberts
void noun() {
  cout << "Processing <noun>\n";
  if(match(SUBJECT)) {
    subject();
  }
  else {
    syntaxerror1(SUBJECT, saved_lexme);
  }
}

// Grammar: <after noun> ::= <be>PERIOD| DESTINATION<verb> <tense>| OBJECT<after object>
// Done by: Preston Roberts
void afternoun() {
  cout << "Processing <afternoun>\n";
  if(match(IS)) {
    be();
  }
  else if(match(WAS)) {
    be();
  }
  else if(match(DESTINATION)) {
    destination();
  }
  else if(match(OBJECT)) {
    object();
  }
  else {
    syntaxerror1(OBJECT,saved_lexme);
  }
}

// Grammar: OBJECT<verb><tense> PERIOD | OBJECT<noun>DESTINATION<verb><tense> PERIOD
// Done by: Preston Roberts
void object() {
  cout << "Processing <object>\n";
  if(match(WORD1)) {
    afterobject();
  }
  else if(match(WORD2)) {
    verb();
  }
  else if(match(PRONOUN)) {
    afterobject();
  }
  else {
    syntaxerror1(WORD1, saved_lexme);
  }
}

// Grammar: <after object> ::= <verb> <tense> PERIOD | <noun> DESTINATION <verb> <tense> PERIOD
// Done by: Preston Roberts
void afterobject() {
  cout << "Processing <afterobject>\n";
  if(match(DESTINATION)) {
    destination();
  }
  else {
    syntaxerror1(DESTINATION, saved_lexme);
  }
}

// Grammar: <destination> ::= <verb> {}
// Done by: Preston Roberts
void destination() {
  cout << "Processing <destination>\n";
  if(match(WORD2)) {
    verb();
  }
  else {
    syntaxerror1(WORD2,saved_lexme);
  }
}

// Grammar: <verb><tense> PEROD EOF
// Done by: Preston Roberts
void period() {
  cout << "Processing <period>\n";
  if(match(CONNECTOR))
  {
    connector();
  }
  else if(match(PRONOUN))
  {
    noun();
  }
  else if(match(WORD1))
  {
    noun();
  }
  else if(match(EOFM)) {
    // done
    cout <<"done";
  }
  else
  {
    syntaxerror2(saved_token,saved_lexme);
  }
    
}

//----------- Driver ---------------------------

// The new test driver to start the parser
// Done by:  Preston Roberts
int main() {
  tokentype thetype;
  string theword; 

  cout << "Enter the input file name: ";
  cin >> filename;
  fin.open(filename.c_str());

  // read file and store as token vector?
  cout << "Processing story\n\n";
  //** calls the <story> to start parsing
  story();


  //** closes the input file 
  fin.close();
}// end
//** require no other input files!
//** syntax error EC requires producing errors.txt of error messages
//** tracing On/Off EC requires sending a flag to trace message output functions
