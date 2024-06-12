#include <iostream>
#include <vector>
#include <queue>
#include <deque>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <fstream>
#include "Scheduler.h"
using namespace std;

// g++ -o run main.cpp Scheduler.cpp FCFS.cpp SJF.cpp Priority.cpp RR.cpp PriorityRR.cpp   
// ./run


// Read the input processes from data.txt file
void readTestData(Scheduler *scheduler)
{
	unsigned int arrivalTime, burstTime, Priority;
	string processName, line;

	// Open the file containing the processes data
	string fileName = "data.txt";
	fstream fileStream;
	fileStream.open(fileName, ios::in);

	if (!fileStream.is_open())
	{
		cerr << "Error opening the file " << fileName << endl;
		exit(1);
	}

	// Parse the file line by line, create each process
	while (getline(fileStream, line))
	{
		if(line.empty())
			break;
		stringstream lineStream(line);
		lineStream >> processName >> arrivalTime >> burstTime >> Priority;

		scheduler->createProcess(arrivalTime, burstTime, Priority);
	}
}

// User interface
int main()
{
	while (1)
	{
		// OS initialize
		Scheduler scheduler;
		readTestData(&scheduler);

		cout << "\nChoose scheduling algorithm" << endl;
		cout << "[FCFS], [SJF], [PRIORITY], [RR], [PRIORITY_RR], [EXIT]" << endl;

		string sLine;
		getline(cin, sLine);

		if (sLine == "FCFS")
			scheduler.setSchedulingAlgorithm(FCFS);

		else if (sLine == "SJF")
			scheduler.setSchedulingAlgorithm(SJF);

		else if (sLine == "PRIORITY")
			scheduler.setSchedulingAlgorithm(PRIORITY);

		else if (sLine == "RR")
			scheduler.setSchedulingAlgorithm(RR);

		else if (sLine == "PRIORITY_RR")
			scheduler.setSchedulingAlgorithm(PRIORITY_RR);

		else if (sLine == "EXIT")
			exit(0);

		else
			cout << "Undefined command, try again" << endl;

		// OS run
		scheduler.run();

		// Print results
		cout << "The input processes ... " << endl;
		scheduler.printProcesses();
		cout << endl;
		scheduler.printExecutionHistory();
		cout << endl;
		scheduler.printStatistics();
	}

	return 0;
}