/**
 * Name: Kevyn Higbee, Preston Roberts
 * 
 * Date: March 29, 2019
 * Assignment: Program 3
 * File: implementation File for Event class
 **/

#include "Event.h"

// default constructor
Event::Event()
{
	ID = 0;
	type = ARRIVAL;
	time = 0;
}

// overloaded constructor
// @param: pID - process id
// @param: etype - what type of event
// @param: etime - the current time
Event::Event(int pID, event_t etype, int eTime)
{
	ID = pID;
	type = etype;
	time = eTime;
}