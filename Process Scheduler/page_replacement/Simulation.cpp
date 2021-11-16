/**
 * Name: Kevyn Higbee, Preston Roberts
 * 
 * Date: May 9, 2019
 * Assignment: Program 5
 * File: Simulation.cpp
 **/
//#include "Random.h"
#include "Simulation.h"
#include <iostream>
#include <iomanip>



// initialize everything to zero
Simulation::Simulation()
{
    pgsize = 0;
    memsize = 0;
    algorithm = 0;
    faults = 0;
    references = 0;
    flushes = 0;
    numLines = 0;
    line = 0;
    replacements = 0;
    totalTime = 0;
}

// prints resluts of a run of the simulation
void Simulation::print()
{
    cout << "Printing Algorithm Statistics..." << endl << endl
         << setw(25) << right << "Total References: " << setw(15) << right << references << endl
         << setw(25) << right << "Total Page Faults: " << setw(15) << right << faults << endl
         << setw(25) << right << "Page Replacements: " << setw(15) << right << replacements << endl
         << setw(25) << right << "Total Page Flushes: " << setw(15) << right << flushes << endl;
    cout << fixed << showpoint << setprecision(3); // need fixed 3-decimal precision for accurate time measurement
    cout << endl << "Time Elapsed: " << totalTime << " milliseconds, or about " << totalTime/1000 << " seconds" << endl;
}

// initalize the pageTable
void Simulation::init()
{
    int size = memsize/pgsize; // how many pages do we need for the memory
    cout << fixed << setprecision(1) << "Page Size: " << pgsize << endl; // test line
    cout << fixed << setprecision(1) <<"Memory Size: " << memsize << endl; // test line
    cout << "Number of Pages: "<< size << endl << endl; // test line
    // this wasn't asked for, but it helped for debugging

    for(int i = 0; i < size; i++) // initialize all the necessary empty pages.
    {
        PageEntry page;
        pageTable.push_back(page);
    }
    // this was necessary since we used a vector implementation, since we don't know the size at instantiation.
}

// search the pagetable for the given address
int Simulation::search(int value)
{
    for(int i = 0; i < pageTable.size(); i++)
    {
        if(value == pageTable[i].address)
            return i;
    }
    return -1;
}

// start the simulation and select the replacement algorithm
void Simulation::run(PageEntry &p)
{
    // cout << "running!" << endl;
    switch(algorithm)
    {
        case 0:
            // cout << "test random" << endl; // test line
            random(p);
            break;
        case 1:
            // cout << "test fifo" << endl; // test line
            fifo(p);
            break;
        case 2:
            // cout << "test lru" << endl; // test line
            lru(p);
            break;
        default:
            // cout << "test default" << endl; // test line
            break;
    }
}

// simple random page replagement alrogithm
void Simulation::random(PageEntry &p)
{ 
    /*int victim = 0;
    if(pageTable.size() != 0)
    {*/ // test lines
    int victim = rand() % pageTable.size(); // randomly select a page
    
    //int victim = ranInt(0,pageTable.size()); // test for other random function (provided by teacher for last assignment)

    if(pageTable[victim].dirty) // if the randomly selected page was dirty, then itreate the flush count.
    {
        flushes++;
        //cout << "flsh" << endl; // test line
    }
    //else cout << "not flsh" << endl; // test line
    
    //if(victim < 10) cout << endl << pageTable[victim].address << endl;

    pageTable[victim] = p; // set the new page in the table, but not until after the dirty check.
    
    /*if(victim < 10)
    {
        cout << victim << endl; //test line
        cout << pageTable[victim].address << endl << endl; 
    }*/

    replacements++; // iterate up replacements
    
    //}
    /*else
    {
        pageTable.push_back(p);
    }*/

    //cout << "working" << endl; // test line
}

// first in first out replacement implementation
void Simulation::fifo(PageEntry &p)
{
    // we chose a vector for pageTable implementation because we wanted the random access to be quicker, and also because we wanted
    // the page table to be contiguously allocated (less performance loss from haing to search array).
    int victim = line++; // iterate up a line so that you end up with the proper page
    line %= pageTable.size(); // make the vector (array) loop so that it acts like a circular array/stack
    
    if(pageTable[victim].dirty) // if dirty, then iterate flushes
    {
        flushes++;
    }

    pageTable[victim] = p; // set page only after dirty check
    replacements++; // iterate up replacements
}

// an LRU replacement algorithm which traces back corresponding previous lines for each memory addresses
void Simulation::lru(PageEntry &p)
{
    int victim, idx, min;

    idx = numLines;
    min = line;

    for(int i = 0; i < pageTable.size(); i++) // while inside the range of the page table
    {
        if(pageTable[i].lastUsed < min) // if the last used line of that page is less than the current minimum
        {
            idx = i; // the new least frequently used page (as far as algorithm knows) is now this one. Set to index
            min = pageTable[i].lastUsed; // set that line to the minimum
        }
    }
    victim = idx; // index up.
    
    if(pageTable[victim].dirty) // check dirty, per usual
    {
        flushes++; // if dirty, then iterate flushes
    }

    pageTable[victim] = p; // set page (again, only after the dirty check)
    replacements++; // iterate up replacements
}