/**
 * Name: Kevyn Higbee, Preston Roberts
 * 
 * Date: March 29, 2019
 * Assignment: Program 3
 * File: Event class header file
 **/
#ifndef EVENT_H
#define EVENT_H

// enums
enum event_t { ARRIVAL, BURST_COMPLETE, IO_COMPLETE, TIME_EXPIRE };

// implementation of an Event
class Event 
{
public:
	int ID;								// the id of the process that belongs to the event
	int time;							// time event takes place
	event_t type;						// type of event

	Event();									// default constructor
	Event(int pID, event_t etype, int etime);	// constructor to create Event
};

#endif