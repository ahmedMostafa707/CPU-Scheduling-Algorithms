#include "Scheduler.h"
#define uint32_t unsigned int  

// Initialization
Scheduler::Scheduler()
{
    totalProcessesCount = 0;
    cpuIdleTime = 0;
    finishedProcessesCount = 0;
    totalWaitingTime = 0;
    totalRunningTime = 0;
    schedulingAlgorithm = FCFS; // Set first come first serve as a default scheduling algorithm
}

void Scheduler::run()
{
    switch (schedulingAlgorithm)
    {
    case FCFS:
        runFCFS(createdProcesses);
        break;

    case SJF:
        runSJF(createdProcesses);
        break;

    case PRIORITY:
        runPriority(createdProcesses);
        break;

    case RR:
        runRR(createdProcesses, TIME_QUANTUM);
        break;

    case PRIORITY_RR:
        runPriorityRR(createdProcesses, TIME_QUANTUM);
        break;

    default:
        runFCFS(createdProcesses);
        break;
    }
}

// All processes should be created using this function
void Scheduler::createProcess(uint32_t arrivalTime, uint32_t burstTime, uint32_t priority)
{
    PCB process;

    process.PID = totalProcessesCount + 1;
    process.arrivalTime = arrivalTime;
    process.burstTime = burstTime;
    process.priority = priority;

    process.remainingTime = burstTime;
    process.waitingTime = 0;

    createdProcesses.push_back(process);

    totalProcessesCount++;
}

// Print input processes
void Scheduler::printProcesses()
{
    // Print the processes
    cout << "Process"
        << "  "
        << "ArvlTime"
        << "  "
        << "burstTime"
        << "  "
        << "Priority" << endl;

    for (auto p : createdProcesses)
    {

        cout << left;
        cout << setw(9) << p.PID;
        cout << setw(10) << p.arrivalTime;
        cout << setw(11) << p.burstTime;
        cout << setw(9) << p.priority << endl;
    }
}

// Print execution history (Similar to Gantt chart)
void Scheduler::printExecutionHistory()
{
    cout << "The execution order: " << endl;
    cout << "From  " << "To    " << "PID" <<endl;

    for (uint32_t i = 0; i < processingHistory.size(); ++i)
    {
        cout << left;
        cout << setw(6) << processingHistory[i].startTime;
        cout << setw(6) << processingHistory[i].endTime;
        cout << "P" << processingHistory[i].PID <<endl;

    }
}

// Print waiting time, turn-around time, utilization, etc.
void Scheduler::printStatistics()
{
    cout << "Average waiting time = " << (float)totalWaitingTime / totalProcessesCount << endl;
    cout << "CPU Utilization = " << (float)totalRunningTime / (totalRunningTime + cpuIdleTime) * 100 << "%" << endl;

    // Prints stats for each process
    cout << "\nPID"
        << "  "
        << "ArvlTime"
        << "  "
        << "EndTime"
        << "  "
        << "TrnArndTime"
        << "  "
        << "WaitingTime" << endl;

    for (uint32_t i = 0; i < terminatedProcesses.size(); ++i)
    {
        cout << left;
        cout << setw(5) << terminatedProcesses[i].PID;
        cout << setw(10) << terminatedProcesses[i].arrivalTime;
        cout << setw(9) << terminatedProcesses[i].endTime;
        cout << setw(13) << terminatedProcesses[i].turnAroundTime;
        cout << setw(12) << terminatedProcesses[i].waitingTime << endl;
    }
}

void Scheduler::setSchedulingAlgorithm(Scheduling_Algorithm schedAlgorithm)
{
    this->schedulingAlgorithm = schedAlgorithm;
};
