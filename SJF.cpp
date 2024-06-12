#include "Scheduler.h"

// This function is used for sorting the processes by arrival time in descending order 
static bool arrivalTimeComp(PCB P1, PCB P2)
{
	return P1.arrivalTime > P2.arrivalTime;
}

// This function is used for sorting the processes by burt time and arrival time order
static bool comp(PCB P1, PCB P2)
{
	if(P1.burstTime == P2.burstTime) 
		return P1.arrivalTime < P2.arrivalTime;
	else 
		return P1.burstTime < P2.burstTime;
}

// Short-Jop-First algorithm
void Scheduler::runSJF(vector<PCB> processes)
{
	deque<PCB> readyQueue;
	PCB runningProcess;
	unsigned int currentTime = 0;

	// sorting the processes by arrival time in descending order 
	sort(processes.begin(), processes.end(), arrivalTimeComp);

	// Service the processes one by one
	while (!readyQueue.empty() or !processes.empty())
	{
		
		// Add processes to the ready queue whose arrival time is <= current time
		while (!processes.empty())
		{
			if (processes.back().arrivalTime <= currentTime) {
				readyQueue.push_back(processes.back());
				processes.pop_back();
				continue;
			}

			// If no processes are ready and the ready queue is empty, increment time
			if(processes.back().arrivalTime > currentTime and readyQueue.empty()) {
				currentTime++;
				continue;
			}

			break;
		}

		// sort the ready queue
		sort(readyQueue.begin(), readyQueue.end(), comp);	

		// Remove the process from the ready queue and schedule it for running
		runningProcess = readyQueue.front();
		readyQueue.pop_front();

		// The process will run to completion as one slice
		ExecutionSlice executionSlice;
		executionSlice.PID = runningProcess.PID;
		executionSlice.length = runningProcess.burstTime;
		executionSlice.startTime = currentTime;
		executionSlice.endTime = currentTime + runningProcess.burstTime;
		processingHistory.push_back(executionSlice);
		runningProcess.remainingTime = 0;

		currentTime += runningProcess.burstTime;
		totalRunningTime += runningProcess.burstTime;

		// Process is complete
		if(runningProcess.remainingTime == 0)
		{
			runningProcess.endTime = currentTime;
			runningProcess.turnAroundTime = currentTime - runningProcess.arrivalTime;
			runningProcess.waitingTime = currentTime - runningProcess.arrivalTime - runningProcess.burstTime;

			totalWaitingTime += runningProcess.waitingTime;
			finishedProcessesCount++;
			terminatedProcesses.push_back(runningProcess);
		}
	}
}
