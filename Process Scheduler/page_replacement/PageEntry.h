/**
 * Name: Kevyn Higbee, Preston Roberts
 * 
 * Date: May 9, 2019
 * Assignment: Program 5
 * File: PageEntry.h
 **/

#ifndef PAGEENTRY_H
#define PAGEENTRY_H

using namespace std;

class PageEntry
{
public:
    int lastUsed; // for lru
    int address;  // corresponding address
    bool dirty;   // if data has been modified (if write)
    bool valid;   // valid bit.

    PageEntry(); 
    PageEntry(int last, int addr, bool dirt, bool val);
};

#endif