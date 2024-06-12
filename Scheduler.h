#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include <queue>
#include "algorithm"
#include <iostream>
#include <iomanip>
using namespace std;

#define uint32_t unsigned int  
#define TIME_QUANTUM 2

enum Scheduling_Algorithm
{
    FCFS,
    SJF,
    PRIORITY,
    RR,
    PRIORITY_RR
};

// This records a process's activity in the CPU
// This is used to generate the execution history (Gantt chart)
typedef struct ExecutionSlice
{
    uint32_t PID;
    uint32_t length;
    uint32_t startTime;
    uint32_t endTime;
} ExecutionSlice;

// Process control block
struct PCB
{
    uint32_t PID;
    uint32_t arrivalTime;
    uint32_t endTime;
    uint32_t burstTime;
    uint32_t priority;
    uint32_t waitingTime;
    uint32_t turnAroundTime;
    uint32_t remainingTime;
    uint32_t executionCounter;
};

class Scheduler
{
private:
	vector<PCB> createdProcesses;	// Contains initial data
	vector<PCB> terminatedProcesses;	// Contains execution info

	unsigned int totalProcessesCount;
	unsigned int cpuIdleTime;
	unsigned int finishedProcessesCount;
	unsigned int totalWaitingTime;
	unsigned int totalRunningTime;

	Scheduling_Algorithm schedulingAlgorithm;
	vector<ExecutionSlice> processingHistory;

public:
	Scheduler();	// Constructor
	void run();
	void createProcess(uint32_t arrivalTime, uint32_t burstTime, uint32_t priority);
	void printExecutionHistory();
	void printStatistics();
	void printProcesses();
	void setSchedulingAlgorithm(Scheduling_Algorithm schedAlgorithm);
	void runFCFS(vector<PCB> processes);
	void runSJF(vector<PCB> processes);
	void runPriority(vector<PCB> processes);
	void runRR(vector<PCB> processes, uint32_t quanta);
    void runPriorityRR(vector<PCB> processes, uint32_t quanta);
};

#endif /* SCHEDULER_H */
