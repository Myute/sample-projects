/**
 * Name: Kevyn Higbee, Preston Roberts
 * 
 * Date: May 9, 2019
 * Assignment: Program 5
 * File: Main File Implementation
 **/

#include "Simulation.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <sys/time.h>
#include <time.h>

using namespace std;

void printHeader();
void handleArgs(int arc, char *argv[]);
void processFile(Simulation &sim);
int promptSelection();

void getTime(long& t_sec, long& t_usec) //A wrapper for the gettimeofday() linus function
{   //note above parameter variables are by value
    struct timeval pcTime; //some inate std::struct timeval

    gettimeofday(&pcTime,NULL); //get time of day
    t_sec = pcTime.tv_sec; //store the seconds
    t_usec = pcTime.tv_usec; //store the milliseconds
}

int main(int argc, char *argv[])
{
    Simulation sim;

    printHeader();
    handleArgs(argc, argv);

    // store argument inputs
    sim.pgsize = atoi(argv[1]);
    sim.memsize = atoi(argv[2]);
    sim.algorithm = promptSelection() - 1; // since cases are 0, 1, 2 and not 1, 2, 3
    //cout << sim.algorithm << endl; // test line
    sim.init();

    switch(sim.algorithm) // just some text output
    {
        case 0:
            cout << "Running Random Algorthm..." << endl;
            break;
        case 1:
            cout << "Running FIFO Algorithm..." << endl;
            break;
        case 2:
            cout << "Running LRU Algorithm..." << endl;
            break;
        default:
            break;
    }

    processFile(sim);

    return 0;
}

// program header for execution
void printHeader()
{
    cout << "Names: Kevyn Higbee & Preston Roberts" << endl
         << "Date Due: May 12, 2019" << endl
         << "Course: CS433" << endl
         << "Program: Simulation of Page Replacement Algorithms" << endl;
}

// error checking for arguments
void handleArgs(int argc, char* argv[])
{
    // check number of args
    if(argc != 3)
    {
        cout << "ERROR: Correct Usage is: ./assign5  <page size> <physical mem size>";
        exit(1);
    }
    int pgSize = atoi(argv[1]);
    int memSize = atoi(argv[2]);
    // check power of two
    if((pgSize & (pgSize -1)) != 0)
    {
        cout << "ERROR: <page size> must be a power of 2";
        exit(1);
    }
    if((memSize & (memSize - 1)) != 0)
    {
        cout << "ERROR: <physical memory size> must be a power of 2";
        exit(1);
    }

}

// selection parsing
int promptSelection()
{
    int choice;

    cout << "Select Algorithm..." << endl
         << "1) Random" << endl
         << "2) FIFO" << endl
         << "3) LRU" << endl
         << "Choice: ";
    
    cin >> choice;

    while( choice < 1 || choice > 3)
    {
        cout << "Invalid selection..." << endl
             << "Please enter a valid option: ";
        cin >> choice;
    }

    return choice;
}

// Main Simulation processing.
void processFile(Simulation &sim)
{
    // set initial variables
    ifstream fin("references.txt"); // get memory access requests from references.txt
    string line;
    int val = 0;
    int lastDig, index;
    bool bit;

    int emptyCt = sim.memsize/sim.pgsize; // total page count that is initialized as empty
    //cout << "empty: " << emptyCt << endl; //test line

    long ssec, susec, esec, eusec; //variables for the gettime
    /// TIME TEST START ///
    getTime(ssec, susec); //calls the gettime wrapper with values by address
    
    //int count = 0; // test line
    while(fin >> line) // while there are lines in references.txt
    {
        val = stoi(line); // store line as integer address
        lastDig = val % 10; // get the last digit to find read/write
        bit = (lastDig%2 == 1) ? true : false; // is last digit == even? (are we reading or writing)
        //cout << bit << endl;
        index = sim.search(val); // does the address reference already exist in page table

        if(emptyCt > 0) // the initial fill of the memory, no replacement needed until memory full
        {
            PageEntry page;
            page.address = val-bit;
            page.dirty = bit; // if the address last digit was even => determine read/write => determine dirty (true/false) 
            page.lastUsed = sim.line; // set the last used line prevous to current (most recent reference for that memory)
            page.valid = true; // page now valid
            
            emptyCt--; // there is one less full page for the initial fill
            sim.pageTable.at(emptyCt) = page; // send the created page date to that empty page in the pageTable.
            //count++; // test line
        }
        else if(index >= 0) // if you found the memory reference already in the PageTable, then you don't need to fetch again
        {
            sim.pageTable[index].dirty = bit;
            sim.pageTable[index].lastUsed = sim.line;
        }
        else // you did not find the memory reference in the pageTable... fetch data
        {
            PageEntry page;
            page.address = val-bit;
            page.dirty = bit;
            page.lastUsed = sim.line;
            page.valid = true;

            sim.faults++; // you did not find the reference in the page table, therefor memory fault
            sim.run(page); // run the replacement algorithm since the pageTable is full (emptyCt > 0) and you can't find the referenc currently in the pageTable
            // cout << "test" << endl; // test line
        }
        //cout << sim.pageTable.size() << endl; // test line
        sim.references++;
    }
    getTime(esec, eusec);
    /// TIME TEST STOP ///
    ssec = esec - ssec; //evaluate seconds elapsed
    susec = eusec - susec; //evaluate milliseconds elapsed

    /*for(int i = 0; i < 10; i++)
    {
        cout << sim.pageTable.at(i).address << "," << endl;
    }*/

    sim.totalTime = (ssec)*1000.00 + susec/1000.00; //send delta time to the Simulation for the print
    //cout << count << endl; // test line
    sim.print(); // print statistics.
}