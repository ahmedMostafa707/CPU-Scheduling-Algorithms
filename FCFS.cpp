#include "Scheduler.h"

// This function is used for sorting the processes by arrival time in ascending order
static bool arrivalTimeComp(PCB P1, PCB P2)
{
	return P1.arrivalTime < P2.arrivalTime;
}

// First-come first-serve algorithm
void Scheduler::runFCFS(vector<PCB> processes)
{
	queue<PCB> readyQueue;
	PCB runningProcess;
	unsigned int currentTime = 0;

	// sort based on arrival time
	sort(processes.begin(), processes.end(), arrivalTimeComp);

	// Add all processes sorted by arrival time to the ready queue
	for (auto p : processes) readyQueue.push(p);

	// Service the processes one by one
	while (!readyQueue.empty())
	{
		// Remove the first-arrived process from the ready queue and schedule it for running
		runningProcess = readyQueue.front();
		readyQueue.pop();

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
