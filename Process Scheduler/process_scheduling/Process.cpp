/**
 * Name: Kevyn Higbee, Preston Roberts
 * 
 * Date: March 29, 2019
 * Assignment: Program 3
 * File: process class implementation file
 **/

#include <iomanip>
#include <iostream>

#include "Process.h"
#include "Random.h"

using namespace std;

// default constructor ( this should not actually be used )
// initializes everythign to zero, status to New
Process::Process()
{
	enterTime = 0;
	triggerTime = 0;
	finishTime = 0;
	waitTime = 0;
	turnAroundTime = 0;
	serviceTime = 0;
	procID = 0;
	startTime = 0;
	procStatus = NEW;
	priority = 0;
	totalCPUduration = 0; 
	remainCPUduration = 0;
	avgCPUburstLength = 0; 
	nextCPUburstLength = 0;
	totalIOburstTime = 0;
	IOburstTime = 0;
	
}

// overlaoded constructor ( this is the one that is used )
// @param: x - id of the process
Process::Process(int x)
{
	procID = x;
	startTime = ranInt(0, 300000); // generated between 1 millseconds - 5 minutes.
	procStatus = NEW;
	priority = 0;
	totalCPUduration = ranInt(1000, 60000); // generated between 1 seconds to 1 minute.
	remainCPUduration = totalCPUduration;
	avgCPUburstLength = ranInt(5, 100); // generated between 5 - 100 milliseconds
	nextCPUburstLength = 0;
	totalIOburstTime = 0;
	waitTime = 0;
	turnAroundTime = 0;
	serviceTime = 0;
	IOburstTime = 0;
	enterTime = 0;
	triggerTime = 0;
	finishTime = 0;
	
}

// PURPOSE: displays a Process' information
void Process::info()
{
	cout << left 
	<< setw(7) << procID 
	<< setw(7) << priority 
	<< setw(7) << procStatus 
	<< setw(7) << startTime 
	<< setw(7) << remainCPUduration 
	<< setw(7) << totalCPUduration 
	<< setw(7) << avgCPUburstLength 
	<< setw(7) << nextCPUburstLength 
	<< setw(7) << IOburstTime
	<< setw(7) << finishTime 
	<< setw(7) << turnAroundTime 
	<< setw(7) << waitTime << endl;
}