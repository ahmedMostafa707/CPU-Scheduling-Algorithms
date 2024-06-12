#include "Scheduler.h"

// This function is used for sorting the processes by arrival time and burst time  order
static bool comp(PCB P1, PCB P2)
{
	if(P1.arrivalTime == P2.arrivalTime)
		return P1.burstTime < P2.burstTime;
	else 
		return P1.arrivalTime < P2.arrivalTime;
}

// round-robin algorithm
void Scheduler::runRR(vector<PCB> processes, unsigned int quanta)
{
	queue<PCB> readyQueue;
	PCB runningProcess;
	unsigned int currentTime = 0;

	// sort based on arrival time
	sort(processes.begin(), processes.end(), comp);

	// Add all processes to the ready queue
	for (auto p : processes) 
    {
        p.remainingTime = p.burstTime; 
		readyQueue.push(p);
    }

	// Service the processes one by one
	while (!readyQueue.empty())
	{
		if(readyQueue.front().arrivalTime > currentTime) {
			readyQueue.push(readyQueue.front());
			readyQueue.pop();
			continue;
		}

		// Remove the process from the ready queue and schedule it for running
		runningProcess = readyQueue.front();
		readyQueue.pop();

		// There is only one process
		if(readyQueue.empty()) {
			// The process will run for the remaining time
			ExecutionSlice executionSlice; 
			executionSlice.PID = runningProcess.PID;
			executionSlice.length = runningProcess.remainingTime;
			executionSlice.startTime = currentTime;
			executionSlice.endTime = currentTime + runningProcess.remainingTime;
			processingHistory.push_back(executionSlice);

			currentTime += runningProcess.remainingTime;
			totalRunningTime += runningProcess.remainingTime;
			runningProcess.remainingTime = 0;
		}
		else {
			// The process will run for the quanta time
			ExecutionSlice executionSlice; 
			executionSlice.PID = runningProcess.PID;
			executionSlice.length = min(quanta, runningProcess.remainingTime);
			executionSlice.startTime = currentTime;
			executionSlice.endTime = currentTime + min(quanta, runningProcess.remainingTime);
			processingHistory.push_back(executionSlice);

			currentTime +=  min(quanta, runningProcess.remainingTime);
			totalRunningTime +=  min(quanta, runningProcess.remainingTime);
			runningProcess.remainingTime = (runningProcess.remainingTime <= quanta ? 0 : runningProcess.remainingTime - quanta);
		}

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
        else
		{
			// Process is not complete, push it back to the ready queue
			readyQueue.push(runningProcess);
        }
	}
}


