/**
 * Name: Kevyn Higbee, Preston Roberts
 * 
 * Date: May 10, 2019
 * Assignment: Program 5
 * File: Simulation.h
 **/

#ifndef SIMULATION_H
#define SIMULATION_H

#include "PageEntry.h"
#include <vector>

using namespace std;

class Simulation
{
public:
    // variables
    vector<PageEntry> pageTable;    // page table
    double pgsize;                  // page size
    double memsize;                 // physical memory size
    int algorithm;                  // which algorithm selected, 0)random 1)fifo 2)lru
    int faults;                     // total number of page faults
    int references;                 // total number of references                   // total number of flushes
    int replacements;               // total number of replacements
    int flushes;                    // total number of flushes (replacement where dirty bit = 1)
    int numLines;                   // line counter for lru algorithm
    int line;                       // current line for fifo algorithm
    double totalTime;               // time elapsed during algorithm

    // constructor & functions
    Simulation();

    int search(int value);  // searches pagetable for page, returns -1 if value not found
    void print();           // prints data from simulation
    void init();            // fills pagetable with 'size' number of empty pageentries
    void run(PageEntry &p); // start selected simulation

    void random(PageEntry &p);  // random policy simulation
    void fifo(PageEntry &p);    // first in first out policy simulation
    void lru(PageEntry &p);     // least recently used policy simulation
};


#endif