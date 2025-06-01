# CPU Scheduling Algorithms Simulation
# AmirAli Fakhari | Melika Soofi
## Project Overview
This project, developed for an Operating Systems course, implements and analyzes three classic CPU scheduling algorithms: First-Come, First-Served (FCFS), Shortest Job First (SJF) (non-preemptive), and Round Robin (RR). The primary goal is to simulate their behavior with a given set of processes and compute key performance evaluation metrics for each algorithm. This allows for a comparative analysis of their efficiency and fairness under different workloads.

The simulation is written in C and includes a testing suite to verify the correctness of the implemented algorithms against expected outcomes.

## Algorithms Implemented
The following CPU scheduling algorithms are implemented:

### First-Come, First-Served (FCFS)
- A non-preemptive scheduling algorithm where processes are executed in the order they arrive in the ready queue.
- Simple to implement but can lead to the convoy effect, where short processes get stuck waiting for long processes.

### Shortest Job First (SJF) - Non-Preemptive
- A non-preemptive scheduling algorithm where the process with the smallest CPU burst time is selected next.
- Optimal in terms of minimizing average waiting time but requires knowledge of future burst times and can lead to starvation for long processes if short processes keep arriving.

### Round Robin (RR)
- A preemptive scheduling algorithm designed for time-sharing systems.
- Each process is assigned a fixed time slice (time quantum). Processes run for this quantum, and if not completed, they are moved to the back of the ready queue.
- Provides better response times and fairness compared to FCFS and non-preemptive SJF, but performance depends heavily on the chosen time quantum.

## Evaluation Metrics
For each algorithm, the following average metrics are computed for a given set of processes:
- **Turnaround Time (TAT)**: The total time a process spends in the system, from arrival to completion.  
  *Formula*: TAT = Completion Time - Arrival Time
- **Waiting Time (WT)**: The total time a process spends waiting in the ready queue before its execution begins or resumes.  
  *Formula*: WT = Turnaround Time - Burst Time
- **Response Time (RT)**: The time from when a process arrives until it first gets the CPU. For non-preemptive algorithms, this is often the same as the waiting time for the first run.  
  *Formula*: RT = First Execution Start Time - Arrival Time

## File Structure
```
.
├── scheduler.h         # Header file with struct definitions (Process, Metrics) and function prototypes for scheduling algorithms.
├── scheduler.c         # Source file containing the C implementations of FCFS, SJF, and RR scheduling algorithms.
├── scheduler_test.c    # Main program with test cases, process definitions, and assertions to validate the algorithms.
├── Makefile            # Script for automating the build (compilation) and execution process.
└── README.md           # This file: Project documentation, setup, and usage instructions.
```

## Prerequisites
To compile and run this project, you will need:
- A C compiler (e.g., gcc)
- The `make` utility

These are standard tools available on most Unix-like systems (Linux, macOS, WSL on Windows).

## Input Format
The inputs for the scheduling algorithms are defined as test cases within `scheduler_test.c`. Each test case consists of:
- A set of processes, where each process is an instance of the `Process` struct (defined in `scheduler.h`) with the following key fields initialized:
  - `pid`: A unique integer Process ID.
  - `arrivalTime`: An integer representing the time at which the process arrives in the ready queue.
  - `burstTime`: An integer representing the total CPU execution time required by the process.
- For the Round Robin (RR) algorithm, an integer `timeQuantum` is also specified for each test case, defining the duration of a time slice.

**Example of Process Definition in `scheduler_test.c`**:
```c
Process test1[3] = {
    {1, 0, 10, 0, 0, 0}, // Process 1: PID=1, Arrival=0, Burst=10
    {2, 1,  5, 0, 0, 0}, // Process 2: PID=2, Arrival=1, Burst=5
    {3, 2,  8, 0, 0, 0}  // Process 3: PID=3, Arrival=2, Burst=8
};
int quantum1 = 4; // Time quantum for Round Robin for this specific test case
```

The other fields in the `Process` struct (`remainingTime`, `startTime`, `completionTime`) are calculated by the scheduling algorithms.

## Output Format
When the test program `scheduler_test` is executed, it produces output for each defined test case, detailing:
- The name of the test case.
- For each of the three scheduling algorithms (FCFS, SJF, and RR):
  - **Calculated Metrics**: The average Turnaround Time, average Waiting Time, and average Response Time computed by the implemented algorithm for the given set of processes. These are typically floating-point numbers displayed to two decimal places.
  - **Expected Metrics**: The predefined expected average metrics for that specific test case. These are used by the assert statements in `scheduler_test.c` to verify correctness.
- A status message: `>>> Test Case X PASSED.` if the calculated metrics match the expected metrics within a defined tolerance.
- After all test cases have been processed, a final `ALL TESTS PASSED.` message is displayed if every assertion throughout all test cases was successful.

## How to Compile
1. Navigate to the root directory of the project in your terminal.
2. Run the `make` command:
   ```bash
   make
   ```
This command uses the Makefile to compile the source files (`scheduler.c` and `scheduler_test.c`) and links them to create an executable file named `scheduler_test`.

## How to Run
After successful compilation:
1. Run the compiled test program using the `make run` command:
   ```bash
   make run
   ```
2. Alternatively, you can directly execute the compiled program:
   ```bash
   ./scheduler_test
   ```
The program will execute all predefined test cases and print the results to the console, as described in the "Output Format" section.

## Sample Program Output
The following is the full output produced when running `./scheduler_test`, demonstrating the results for all test cases:
```
==== Test Case 1 ====
FCFS: Calculated: Turnaround: 15.00, Waiting: 7.33, Response: 7.33
      Expected:   Turnaround: 15.00, Waiting: 7.33, Response: 7.33
SJF:  Calculated: Turnaround: 15.00, Waiting: 7.33, Response: 7.33
      Expected:   Turnaround: 15.00, Waiting: 7.33, Response: 7.33
RR (Quantum = 4): Calculated: Turnaround: 19.33, Waiting: 11.67, Response: 3.00
         Expected:   Turnaround: 19.33, Waiting: 11.67, Response: 3.00
>>> Test Case 1 PASSED.

==== Test Case 2 ====
FCFS: Calculated: Turnaround: 5.67, Waiting: 1.67, Response: 1.67
      Expected:   Turnaround: 5.67, Waiting: 1.67, Response: 1.67
SJF:  Calculated: Turnaround: 5.67, Waiting: 1.67, Response: 1.67
      Expected:   Turnaround: 5.67, Waiting: 1.67, Response: 1.67
RR (Quantum = 2): Calculated: Turnaround: 5.67, Waiting: 1.67, Response: 1.00
         Expected:   Turnaround: 5.67, Waiting: 1.67, Response: 1.00
>>> Test Case 2 PASSED.

==== Test Case 3 ====
FCFS: Calculated: Turnaround: 5.75, Waiting: 3.25, Response: 3.25
      Expected:   Turnaround: 5.75, Waiting: 3.25, Response: 3.25
SJF:  Calculated: Turnaround: 5.00, Waiting: 2.50, Response: 2.50
      Expected:   Turnaround: 5.00, Waiting: 2.50, Response: 2.50
RR (Quantum = 2): Calculated: Turnaround: 6.25, Waiting: 3.75, Response: 1.75
         Expected:   Turnaround: 6.25, Waiting: 3.75, Response: 1.75
>>> Test Case 3 PASSED.

==== Test Case 4 ====
FCFS: Calculated: Turnaround: 8.60, Waiting: 4.60, Response: 4.60
      Expected:   Turnaround: 8.60, Waiting: 4.60, Response: 4.60
SJF:  Calculated: Turnaround: 7.60, Waiting: 3.60, Response: 3.60
      Expected:   Turnaround: 7.60, Waiting: 3.60, Response: 3.60
RR (Quantum = 3): Calculated: Turnaround: 10.60, Waiting: 6.60, Response: 2.60
         Expected:   Turnaround: 10.60, Waiting: 6.60, Response: 2.60
>>> Test Case 4 PASSED.

==== Test Case 5 (Single Process) ====
FCFS: Calculated: Turnaround: 5.00, Waiting: 0.00, Response: 0.00
      Expected:   Turnaround: 5.00, Waiting: 0.00, Response: 0.00
SJF:  Calculated: Turnaround: 5.00, Waiting: 0.00, Response: 0.00
      Expected:   Turnaround: 5.00, Waiting: 0.00, Response: 0.00
RR (Quantum = 2): Calculated: Turnaround: 5.00, Waiting: 0.00, Response: 0.00
         Expected:   Turnaround: 5.00, Waiting: 0.00, Response: 0.00
>>> Test Case 5 (Single Process) PASSED.

==== Test Case 6 (Two Processes, Same Arrival) ====
FCFS: Calculated: Turnaround: 7.00, Waiting: 2.00, Response: 2.00
      Expected:   Turnaround: 7.00, Waiting: 2.00, Response: 2.00
SJF:  Calculated: Turnaround: 7.00, Waiting: 2.00, Response: 2.00
      Expected:   Turnaround: 7.00, Waiting: 2.00, Response: 2.00
RR (Quantum = 3): Calculated: Turnaround: 8.50, Waiting: 3.50, Response: 1.50
         Expected:   Turnaround: 8.50, Waiting: 3.50, Response: 1.50
>>> Test Case 6 (Two Processes, Same Arrival) PASSED.

==== Test Case 7 (Varying Arrivals) ====
FCFS: Calculated: Turnaround: 15.25, Waiting: 8.75, Response: 8.75
      Expected:   Turnaround: 15.25, Waiting: 8.75, Response: 8.75
SJF:  Calculated: Turnaround: 14.25, Waiting: 7.75, Response: 7.75
      Expected:   Turnaround: 14.25, Waiting: 7.75, Response: 7.75
RR (Quantum = 3): Calculated: Turnaround: 20.00, Waiting: 13.50, Response: 3.00
         Expected:   Turnaround: 20.00, Waiting: 13.50, Response: 3.00
>>> Test Case 7 (Varying Arrivals) PASSED.

==== Test Case 8 (Six Processes) ====
FCFS: Calculated: Turnaround: 17.00, Waiting: 11.17, Response: 11.17
      Expected:   Turnaround: 17.00, Waiting: 11.17, Response: 11.17
SJF:  Calculated: Turnaround: 15.17, Waiting: 9.33, Response: 9.33
      Expected:   Turnaround: 15.17, Waiting: 9.33, Response: 9.33
RR (Quantum = 4): Calculated: Turnaround: 20.67, Waiting: 14.83, Response: 6.33
         Expected:   Turnaround: 20.67, Waiting: 14.83, Response: 6.33
>>> Test Case 8 (Six Processes) PASSED.

ALL TESTS PASSED.
```

## How to Clean
To remove the compiled executable (`scheduler_test`) and any intermediate object files created during compilation:
1. Navigate to the root directory of the project.
2. Run the `make clean` command:
   ```bash
   make clean
   ```

## Notes
- The `Process` struct (defined in `scheduler.h`) is central to the simulation, holding all necessary timing information for each process, including `pid`, `arrivalTime`, `burstTime`, `remainingTime`, `startTime`, and `completionTime`.
- The implementation of the Shortest Job First (SJF) algorithm is non-preemptive.
- The Round Robin (RR) algorithm is preemptive and its behavior is influenced by the `timeQuantum` parameter.
- The project is written in standard C and is designed for compatibility with Unix-like operating systems (such as Linux, macOS, or Windows Subsystem for Linux (WSL)) that provide the `gcc` compiler and `make` utility.