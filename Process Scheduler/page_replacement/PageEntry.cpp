/**
 * Name: Kevyn Higbee, Preston Roberts
 * 
 * Date: May 9, 2019
 * Assignment: Program 5
 * File: PageEntry.cpp
 **/

#include "PageEntry.h"

// default construcor
PageEntry::PageEntry()
{
    lastUsed = 0;
    address = 0;
    dirty = false;
    valid = false;
}

// overloaded constructor
PageEntry::PageEntry(int last, int addr, bool dirt, bool val)
{
    lastUsed = last;
    address = addr;
    dirty = dirt;
    valid = val;
}