/**
 * Name: Kevyn Higbee, Preston Roberts
 * 
 * Date: March 29, 2019
 * Assignment: Program 3
 * File: scheduler_fcfs header file
 **/
#ifndef SCHEDULER_FCFS_H
#define SCHEDULER_FCFS_H

#include <queue>
#include "Event.h"
#include "Process.h"
#include "Simulation.h"

class FCFS 
{
public:
	std::queue<Process*> ready_queue;				// ready queue for SJF scheduler								
	Simulation *DES;								// reference to DES
	void handle_event(Event &e);					// handle for Event
	FCFS();											// defeault constructor
	FCFS(Simulation *s);							// constructor with Simulation paramater

private:
	void handle_process_arrival(Event &e);			// handle for Process arrival
	void handle_cpuburst_completion(Event &e);		// handle for cpu_completion
	void handle_io_completion(Event &e);			// handle for io_completion
	void handle_timer_expiration(Event &e);			// handle for time_expiration
	void schedule();								// dispatch
};

#endif