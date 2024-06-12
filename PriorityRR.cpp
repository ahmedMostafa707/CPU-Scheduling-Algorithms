#include "Scheduler.h"

// This function is used for sorting the processes by arrival time in descending order 
static bool arrivalTimeComp(PCB P1, PCB P2)
{
	return P1.arrivalTime > P2.arrivalTime;
}

// This function is used for sorting the processes by priority and arrival time and execution counter order
static bool comp(PCB P1, PCB P2)
{
	if(P1.priority == P2.priority) 
	{
		if(P1.executionCounter == P2.executionCounter) 
			return P1.arrivalTime < P2.arrivalTime;
		else 
			return P1.executionCounter < P2.executionCounter;
	}
	else 
		return P1.priority > P2.priority;
}

// Priority round-robin algorithm
void Scheduler::runPriorityRR(vector<PCB> processes, unsigned int quanta)
{
	deque<PCB> readyQueue;
	PCB runningProcess;
	unsigned int currentTime = 0;

    // sorting the processes by arrival time in descending order
	sort(processes.begin(), processes.end(), arrivalTimeComp);
	
	// Service the processes one by one
	while (!readyQueue.empty() or !processes.empty())
	{
		while (!processes.empty())
		{
			if (processes.back().arrivalTime <= currentTime) 
            {
                processes.back().remainingTime = processes.back().burstTime;
				readyQueue.push_back(processes.back());
				processes.pop_back();
				continue;
			}

			// If no processes are ready and the ready queue is empty, increment time
			if(processes.back().arrivalTime > currentTime and readyQueue.empty()) 
            {
				currentTime++;
				continue;
			}

            break;
		}

        // If no more processes are ready to be added, sort the ready queue
        sort(readyQueue.begin(), readyQueue.end(), comp);	

        // Remove the process from the ready queue and schedule it for running
		runningProcess = readyQueue.front();
		readyQueue.pop_front();

        // case1 round-robin scheduling 
        if (!readyQueue.empty() and readyQueue.front().priority == runningProcess.priority)
        {
            // The process will run for the quanta time
			ExecutionSlice executionSlice; 
			executionSlice.PID = runningProcess.PID;
			executionSlice.length = min(quanta, runningProcess.remainingTime);
			executionSlice.startTime = currentTime;
			executionSlice.endTime = currentTime + min(quanta, runningProcess.remainingTime);
			processingHistory.push_back(executionSlice);

            runningProcess.executionCounter++;
			currentTime +=  min(quanta, runningProcess.remainingTime);
			totalRunningTime +=  min(quanta, runningProcess.remainingTime);
			runningProcess.remainingTime = (runningProcess.remainingTime <= quanta ? 0 : runningProcess.remainingTime - quanta);

            if(runningProcess.remainingTime != 0)
            {
                // Process is not complete, push it back to the ready queue
                readyQueue.push_back(runningProcess);
            }
        }
        else // case2 priority scheduler
        {
            // The process will run to completion as one slice
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
