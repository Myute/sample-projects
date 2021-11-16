/**
 * Name: Kevyn Higbee, Preston Roberts
 * 
 * Date: March 29, 2019
 * Assignment: Program 3
 * File: scheduler_fcfs implemention file
 **/
#include <iostream>
#include "Random.h"
#include "Scheduler_FCFS.h"
#include "Simulation.h"

using namespace std;

// default constructor
FCFS::FCFS()
{
	DES = nullptr;
}

// overloaded constructor
FCFS::FCFS(Simulation * s)
{
	DES = s;
}

// dispatch a process
void FCFS::schedule()
{
	// dispatch Process
	if (DES->_CPU.CPU_STATE == IDLE && !ready_queue.empty()) 
	{
		// select Process to schedule from ready queue.
		Process * p = ready_queue.front();
		ready_queue.pop();

		//  assign Process to cpu.
		DES->_CPU.CPU_JOB = p;
		DES->_CPU.CPU_STATE = BUSY;
		p->procStatus = RUNNING;
		p->triggerTime = DES->currentTime;

		// create Event for the time when Process' cpu burst will complete.
		int time = DES->currentTime + p->nextCPUburstLength;
		Event * cpucompletion = new Event(p->procID, BURST_COMPLETE, time);

		// add Event to Event queue.
		DES->event_queue.push(cpucompletion);
	}
}

// handles event by passing it to correct fucntion
// @param: e - event
void FCFS::handle_event(Event & e)
{
	switch (e.type) 
	{
	case ARRIVAL:
		handle_process_arrival(e);
		break;

	case BURST_COMPLETE:
		handle_cpuburst_completion(e);
		break;

	case IO_COMPLETE:
		handle_io_completion(e);
		break;

	case TIME_EXPIRE:
		handle_timer_expiration(e);
		break;
	}
}

// handles process arrival events
// @param: e - proc arrival event
void FCFS::handle_process_arrival(Event & e)
{
	Process * p = &DES->process_table[e.ID];
	p->nextCPUburstLength = CPUBurstRandom(p->avgCPUburstLength);
	p->enterTime = DES->currentTime;
	p->procStatus = READY;
	ready_queue.push(p);

	schedule();
}

// handle cpu burst completion event
// @param: e - cpu burst completion event
void FCFS::handle_cpuburst_completion(Event & e)
{
	Process * p = &DES->process_table[e.ID];

	// update remaining cpu duration
	p->remainCPUduration -= p->nextCPUburstLength;

	// update info
	p->serviceTime += p->nextCPUburstLength;
	p->waitTime = p->waitTime + (p->triggerTime - p->enterTime);

	if (p->remainCPUduration <= 0) 
	{
		cout << "[clock: " << DES->currentTime / 1000 << "] - Process " << p->procID << " complete!" << endl;
		DES->completedJobs++;
		p->turnAroundTime = DES->currentTime - p->startTime;
		p->finishTime = DES->currentTime;
		p->procStatus = TERMINATED;
		DES->_CPU.CPU_JOB = nullptr;
	}
	else 
	{
		p->IOburstTime = ranInt(30, 100);
		int time = DES->currentTime + p->IOburstTime;
		Event * iocompletion = new Event(e.ID, IO_COMPLETE, time);
		DES->event_queue.push(iocompletion);
	}

	DES->_CPU.CPU_STATE = IDLE;
	schedule();
}

// handles io completion event
// @param: e - io completion evnet
void FCFS::handle_io_completion(Event & e)
{
	Process * p = &DES->process_table[e.ID];

	// generate the processes next cpu burst length
	p->nextCPUburstLength = CPUBurstRandom(p->avgCPUburstLength);
	p->totalIOburstTime += +p->IOburstTime;

	// move the Process to ready queue.
	p->procStatus = READY;
	p->enterTime = DES->currentTime;
	ready_queue.push(p);

	schedule();
}

// handles expiration event
// @param: e - timer expiraiton event
void FCFS::handle_timer_expiration(Event & e)
{
	Process * p = &DES->process_table[e.ID];
	p->procStatus = READY;
	ready_queue.push(p);
	p->nextCPUburstLength = CPUBurstRandom(p->avgCPUburstLength);
	DES->_CPU.CPU_STATE = IDLE;
	schedule();
}