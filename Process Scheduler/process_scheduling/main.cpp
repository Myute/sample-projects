/**
 * Name: Kevyn Higbee, Preston Roberts
 * 
 * Date: March 29, 2019
 * Assignment: Program 3
 * File: main.cpp
 **/

#include <iostream>
#include <string>
#include "Simulation.h"

using namespace std;

// function prototypes
void parseArguments(Simulation &sim, int argc, char * argv[]);
void prompt(Simulation &sim, int argc, char * argv[]);

// Main Function - implements cpu schedulers using simulator
// @param: argc - number of arguments
// @param: argv[] - arguments
int main(int argc, char * argv[])
{
	Simulation sim;
	parseArguments(sim, argc, argv);
	sim.setup();					

	switch (sim.selection) 
	{
	case 1: // FCFS Scheduler
		cout << "Running FCFS" << endl << endl;
		sim.runFCFS();
		sim.report();
		break;

	case 2: // SJF Scheduler
		cout << "Running SJF" << endl << endl;
		sim.runSJF();
		sim.report();
		break;

	default: // do nothing if unexpected selection
		break;
	}

	return 0;
}

// parseArguments: prompts user for arguments, handles arguments
// @param: &sim = sim Simulation
// @param: argc = number of arguments
// @param: argv[] = arguments passed
void parseArguments(Simulation &sim, int argc, char * argv[])
{
	if (argc == 3) 	// if 2 arguments are passed
	{
		sim.amount = atoi(argv[1]);
		sim.selection = atoi(argv[2]);

		if (sim.amount < 1 || sim.selection < 1 || sim.selection > 2) 
		{
			cout << "[ERROR::MAIN::PARSE_ARGUMENTS:INAVLID_ARGUMENTS]" << endl;
			prompt(sim, argc, argv);
		}
	}
	else // if 2 arguments are not passed
	{
		if (argc > 3 || argc == 2) 
			cout << "[ERROR::MAIN::PARSE_ARGUMENTS:INAVLID_ARGUMENTS]" << endl;

		prompt(sim, argc, argv);
	}
}


// prompt: prompts user input for selection of scheduelr and how many jobs
// @param: &sim = sim simulation
// @param: argc = number of arguments
// @param: argv[] = arguments passed
void prompt(Simulation &sim, int argc, char * argv[]) 
{
	cout << "Enter number of processes to add into Simulation: ";
	cin >> sim.amount;

	while (sim.amount < 1 || cin.fail()) 
	{
		cin.clear();
		cin.ignore(32767, '\n');
		cout << "[ERROR::MAIN::PROMPT:BAD_INPUT] Enter Again: ";
		cin >> sim.amount;
	}
	cout << endl;

	cout << "Select scheduler type" << endl;
	cout << "1: FCFS" << endl;
	cout << "2: SJF" << endl << endl;

	cout << ">> ";
	cin >> sim.selection;

	while (sim.selection < 1 || sim.selection > 2 || cin.fail()) 
	{
		cin.clear();
		cin.ignore(32767, '\n');
		cout << "[ERROR::MAIN::PROMPT:BAD_INPUT] Enter Again: ";
		cin >> sim.selection;
	}
	cout << endl;
}