/**
 * Name: Kevyn Higbee, Preston Roberts
 * 
 * Date: April 23, 2019
 * Assignment: Program 4
 * File:  PBuffer Header File
 **/

#include <iostream>


using namespace std;

typedef int bItm;
#define BUFFER_SIZE 5


class PBuffer
{
 private:
   bItm buffer[BUFFER_SIZE];
   int front = 0;
   int rear = -1;
   int count = 0;

 public: 
   PBuffer();             // Constructor
   int insertItem(bItm);  // inserts element at rear of buffer
   int removeItem(bItm*); // removes front element of buffer
   void display();        // prints all the contents of the buffer
   bool isFull();         // returns true if count == BUFFER_SIZE
   bool isEmpty();        // returns true if count == 0
};