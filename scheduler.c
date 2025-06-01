// #include "scheduler.h"

// // ---------------- Scheduling Algorithms ----------------

// // FCFS Scheduling
// Metrics fcfs_metrics(Process proc[], int n) {
//     //TODO
// }

// // SJF Scheduling (Non-preemptive)
// Metrics sjf_metrics(Process proc[], int n) {
//     //TODO
// }

// // Round Robin Scheduling (Revised)
// Metrics rr_metrics(Process proc[], int n, int timeQuantum) {
//     //TODO
// }


#include "scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // For INT_MAX

// Helper function to sort processes by arrival time (for FCFS)
// and then by PID for tie-breaking (though not strictly necessary for FCFS logic, good for consistency)
int compareProcessesArrivalTime(const void *a, const void *b) {
    Process *p1 = (Process *)a;
    Process *p2 = (Process *)b;
    if (p1->arrivalTime < p2->arrivalTime) {
        return -1;
    } else if (p1->arrivalTime > p2->arrivalTime) {
        return 1;
    } else {
        // Tie-breaking by PID if arrival times are the same
        if (p1->pid < p2->pid) {
            return -1;
        } else if (p1->pid > p2->pid) {
            return 1;
        }
        return 0;
    }
}

// FCFS Scheduling
Metrics fcfs_metrics(Process proc[], int n) {
    Metrics metrics = {0.0, 0.0, 0.0};
    if (n == 0) return metrics;

    // Create a copy of processes to sort, as the original order might be important elsewhere
    Process sorted_procs[n];
    for (int i = 0; i < n; i++) {
        sorted_procs[i] = proc[i];
    }

    qsort(sorted_procs, n, sizeof(Process), compareProcessesArrivalTime);

    int currentTime = 0;
    float totalTurnaroundTime = 0;
    float totalWaitingTime = 0;
    float totalResponseTime = 0;

    for (int i = 0; i < n; i++) {
        // If CPU is idle until this process arrives
        if (currentTime < sorted_procs[i].arrivalTime) {
            currentTime = sorted_procs[i].arrivalTime;
        }

        sorted_procs[i].startTime = currentTime;
        sorted_procs[i].completionTime = currentTime + sorted_procs[i].burstTime;

        int turnaroundTime = sorted_procs[i].completionTime - sorted_procs[i].arrivalTime;
        int waitingTime = turnaroundTime - sorted_procs[i].burstTime;
        int responseTime = sorted_procs[i].startTime - sorted_procs[i].arrivalTime;

        totalTurnaroundTime += turnaroundTime;
        totalWaitingTime += waitingTime;
        totalResponseTime += responseTime;

        currentTime = sorted_procs[i].completionTime;
    }

    metrics.avgTurnaround = totalTurnaroundTime / n;
    metrics.avgWaiting = totalWaitingTime / n;
    metrics.avgResponse = totalResponseTime / n;

    return metrics;
}

// SJF Scheduling (Non-preemptive)
Metrics sjf_metrics(Process proc[], int n) {
    Metrics metrics = {0.0, 0.0, 0.0};
    if (n == 0) return metrics;

    Process processes[n];
    for(int i=0; i<n; i++) {
        processes[i] = proc[i]; // Work with a copy
        processes[i].remainingTime = processes[i].burstTime; // Initialize remaining time
    }

    int currentTime = 0;
    int completed = 0;
    float totalTurnaroundTime = 0;
    float totalWaitingTime = 0;
    float totalResponseTime = 0;
    int visited[n]; // To keep track of processed processes
    for(int i=0; i<n; i++) visited[i] = 0;


    while (completed < n) {
        int shortestJobIndex = -1;
        int shortestBurst = INT_MAX;

        // Find the shortest job that has arrived and not yet completed
        for (int i = 0; i < n; i++) {
            if (!visited[i] && processes[i].arrivalTime <= currentTime) {
                if (processes[i].burstTime < shortestBurst) {
                    shortestBurst = processes[i].burstTime;
                    shortestJobIndex = i;
                }
                // Tie-breaking: if burst times are equal, pick the one that arrived earlier
                else if (processes[i].burstTime == shortestBurst) {
                    if (processes[i].arrivalTime < processes[shortestJobIndex].arrivalTime) {
                        shortestJobIndex = i;
                    }
                    // Further tie-breaking by PID if arrival times are also equal
                    else if (processes[i].arrivalTime == processes[shortestJobIndex].arrivalTime && processes[i].pid < processes[shortestJobIndex].pid) {
                         shortestJobIndex = i;
                    }
                }
            }
        }

        if (shortestJobIndex == -1) {
            // No process is ready, CPU is idle. Advance time to the next arrival.
            int nextArrivalTime = INT_MAX;
            for(int i=0; i<n; i++) {
                if(!visited[i] && processes[i].arrivalTime < nextArrivalTime) {
                    nextArrivalTime = processes[i].arrivalTime;
                }
            }
            if (nextArrivalTime != INT_MAX) { // Check if there are still processes to run
                 currentTime = nextArrivalTime;
            } else {
                break; // All processes processed or no process to advance time to
            }
            continue; // Re-evaluate at the new current time
        }

        // Process the selected job
        Process *currentProcess = &processes[shortestJobIndex];

        currentProcess->startTime = currentTime; // Start time is when it's picked
        currentProcess->completionTime = currentTime + currentProcess->burstTime;

        int turnaroundTime = currentProcess->completionTime - currentProcess->arrivalTime;
        int waitingTime = turnaroundTime - currentProcess->burstTime;
        int responseTime = currentProcess->startTime - currentProcess->arrivalTime; // For non-preemptive, same as waiting if first run

        totalTurnaroundTime += turnaroundTime;
        totalWaitingTime += waitingTime;
        totalResponseTime += responseTime;

        currentTime = currentProcess->completionTime;
        visited[shortestJobIndex] = 1; // Mark as completed
        completed++;
    }

    metrics.avgTurnaround = totalTurnaroundTime / n;
    metrics.avgWaiting = totalWaitingTime / n;
    metrics.avgResponse = totalResponseTime / n;

    return metrics;
}


// Round Robin Scheduling
Metrics rr_metrics(Process proc[], int n, int timeQuantum) {
    Metrics metrics = {0.0, 0.0, 0.0};
    if (n == 0) return metrics;

    Process processes[n];
    // int first_response_recorded[n]; // REMOVED: To track if response time has been recorded

    for (int i = 0; i < n; i++) {
        processes[i] = proc[i];
        processes[i].remainingTime = processes[i].burstTime;
        processes[i].startTime = -1; // Initialize start time to -1 (not started)
        // first_response_recorded[i] = 0; // REMOVED
    }

    // Queue for ready processes (using an array as a simple queue)
    int ready_queue[n * 10]; // Max queue size (can be optimized)
    int front = 0, rear = 0;
    int processes_in_queue_or_arrived[n]; // 0: not arrived, 1: in queue/arrived but not finished
    for(int i=0; i<n; i++) processes_in_queue_or_arrived[i] = 0;


    int currentTime = 0;
    int completed = 0;
    float totalTurnaroundTime = 0;
    float totalWaitingTime = 0;
    float totalResponseTime = 0;

    Process temp_procs_for_sort[n];
     for (int i = 0; i < n; i++) temp_procs_for_sort[i] = processes[i];
    qsort(temp_procs_for_sort, n, sizeof(Process), compareProcessesArrivalTime);

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(processes[j].pid == temp_procs_for_sort[i].pid){
                if(processes[j].arrivalTime <= currentTime && processes_in_queue_or_arrived[j] == 0){
                    ready_queue[rear++] = j;
                    processes_in_queue_or_arrived[j] = 1;
                }
                break;
            }
        }
    }


    while (completed < n) {
        for (int i = 0; i < n; i++) {
            int original_proc_idx = -1;
            for(int k=0; k<n; k++){
                if(processes[k].pid == temp_procs_for_sort[i].pid){
                    original_proc_idx = k;
                    break;
                }
            }
            if (original_proc_idx != -1 && processes[original_proc_idx].arrivalTime <= currentTime && processes_in_queue_or_arrived[original_proc_idx] == 0) {
                int already_in_q = 0;
                for(int qi = front; qi < rear; qi++){
                    if(ready_queue[qi] == original_proc_idx) {
                        already_in_q = 1;
                        break;
                    }
                }
                if(!already_in_q && processes[original_proc_idx].remainingTime > 0){
                    ready_queue[rear++] = original_proc_idx;
                    processes_in_queue_or_arrived[original_proc_idx] = 1;
                }
            }
        }


        if (front == rear) {
            int nextMinArrivalTime = INT_MAX;
            int found_pending = 0;
            for(int i=0; i<n; i++){
                if(processes[i].remainingTime > 0 && processes_in_queue_or_arrived[i] == 0) {
                    if(processes[i].arrivalTime < nextMinArrivalTime){
                        nextMinArrivalTime = processes[i].arrivalTime;
                        found_pending = 1;
                    }
                }
            }
            if(found_pending){
                currentTime = nextMinArrivalTime;
            } else {
                 if (completed == n) break;
                 currentTime++;
            }
            continue;
        }

        int currentProcessIndex = ready_queue[front++];

        if (processes[currentProcessIndex].startTime == -1) {
            processes[currentProcessIndex].startTime = currentTime;
            totalResponseTime += (processes[currentProcessIndex].startTime - processes[currentProcessIndex].arrivalTime);
            // first_response_recorded[currentProcessIndex] = 1; // REMOVED
        }

        int timeSlice = timeQuantum;
        if (processes[currentProcessIndex].remainingTime <= timeQuantum) {
            timeSlice = processes[currentProcessIndex].remainingTime;
            currentTime += timeSlice;
            processes[currentProcessIndex].remainingTime = 0;
            processes[currentProcessIndex].completionTime = currentTime;

            int turnaroundTime = processes[currentProcessIndex].completionTime - processes[currentProcessIndex].arrivalTime;
            int waitingTime = turnaroundTime - processes[currentProcessIndex].burstTime;
            totalTurnaroundTime += turnaroundTime;
            totalWaitingTime += waitingTime;
            completed++;
            processes_in_queue_or_arrived[currentProcessIndex] = 2;
        } else {
            currentTime += timeSlice;
            processes[currentProcessIndex].remainingTime -= timeSlice;

            for (int i = 0; i < n; i++) {
                 int original_proc_idx_inner = -1;
                 for(int k=0; k<n; k++){
                     if(processes[k].pid == temp_procs_for_sort[i].pid){
                         original_proc_idx_inner = k;
                         break;
                     }
                 }
                if (original_proc_idx_inner != -1 && processes[original_proc_idx_inner].arrivalTime <= currentTime && processes_in_queue_or_arrived[original_proc_idx_inner] == 0) {
                    int already_in_q_inner = 0;
                    for(int qi = front; qi < rear; qi++){
                        if(ready_queue[qi] == original_proc_idx_inner) {
                            already_in_q_inner = 1;
                            break;
                        }
                    }
                    if(!already_in_q_inner && processes[original_proc_idx_inner].remainingTime > 0){
                         ready_queue[rear++] = original_proc_idx_inner;
                         processes_in_queue_or_arrived[original_proc_idx_inner] = 1;
                    }
                }
            }
            ready_queue[rear++] = currentProcessIndex;
        }
    }

    metrics.avgTurnaround = totalTurnaroundTime / n;
    metrics.avgWaiting = totalWaitingTime / n;
    metrics.avgResponse = totalResponseTime / n;

    return metrics;
}