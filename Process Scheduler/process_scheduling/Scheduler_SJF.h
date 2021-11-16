/**
 * Name: Kevyn Higbee, Preston Roberts
 * 
 * Date: March 29, 2019
 * Assignment: Program 3
 * File: scheduler_sjf header file
 **/
#ifndef SCHEDULER_SJF_H
#define SCHEDULER_SJF_H

#include <queue>
#include "Event.h"
#include "Process.h"
#include "Simulation.h"

// overrides priority queues comparison logic. comparsion based on next cpu burst
struct SJFCompare 
{
	bool operator () (Process* p1, Process* p2) const 
	{
		return p1->priority > p2->priority;	
	}
};

// implementation of SJF scheduler
class SJF 
{
public:
	std::priority_queue<Process*, std::vector<Process*>, SJFCompare> ready_queue; // ready queue for SJF scheduler
	Simulation * DES;								// reference to DES

	void handle_event(Event &e);					// handle for Event

	SJF();											// defualt constructor
	SJF(Simulation * s);							// constructor with Simulation paramater

private:
	void handle_process_arrival(Event &e);			// handle for Process arrival
	void handle_cpuburst_completion(Event &e);		// handle for cpu_completion
	void handle_io_completion(Event &e);			// handle for io_completion
	void handle_timer_expiration(Event &e);			// handle for time_expiration
	void schedule();								// dispatcher
};

#endif