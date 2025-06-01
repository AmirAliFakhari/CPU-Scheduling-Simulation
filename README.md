# CPU Scheduling Algorithms Simulation
# AmirAli Fakhari | Melika Soofi
## Project Overview
This project, developed for an Operating Systems course, implements and analyzes three classic CPU scheduling algorithms: First-Come, First-Served (FCFS), Shortest Job First (SJF) (non-preemptive), and Round Robin (RR). The goal is to simulate their behavior with a given set of processes and compute key performance metrics for comparative analysis of their efficiency and fairness under different workloads.

The simulation is written in C and includes a testing suite to verify the correctness of the implemented algorithms against expected outcomes.

## Algorithms Implemented
The following CPU scheduling algorithms are implemented:

### First-Come, First-Served (FCFS)
- A non-preemptive scheduling algorithm where processes are executed in the order they arrive in the ready queue.
- Simple to implement but can lead to the convoy effect, where short processes wait for long processes.

### Shortest Job First (SJF) - Non-Preemptive
- A non-preemptive scheduling algorithm where the process with the smallest CPU burst time is selected next.
- Optimal for minimizing average waiting time but requires knowledge of burst times and can cause starvation for long processes.

### Round Robin (RR)
- A preemptive scheduling algorithm designed for time-sharing systems.
- Each process gets a fixed time slice (quantum). If not completed, it moves to the back of the ready queue.
- Offers better response times and fairness, but performance depends on the time quantum.

## Evaluation Metrics
For each algorithm, the following average metrics are computed:
- **Turnaround Time (TAT)**: Total time from arrival to completion.  
  *Formula*: TAT = Completion Time - Arrival Time
- **Waiting Time (WT)**: Total time spent waiting in the ready queue.  
  *Formula*: WT = Turnaround Time - Burst Time
- **Response Time (RT)**: Time from arrival to first CPU execution.  
  *Formula*: RT = First Execution Start Time - Arrival Time

## File Structure
```
.
├── scheduler.h         # Header file with struct definitions and function prototypes
├── scheduler.c         # Implementations of FCFS, SJF, and RR algorithms
├── scheduler_test.c    # Main program with test cases and assertions
├── Makefile            # Script for compiling and running the project
└── README.md           # This file: Project documentation and instructions
```

## Prerequisites
To compile and run this project, you need:
- A C compiler (e.g., gcc)
- The `make` utility

These are standard tools on Unix-like systems (Linux, macOS, WSL on Windows).

## Input Format
Test cases are defined in `scheduler_test.c`. Each test case includes:
- A set of processes, each an instance of the `Process` struct (`scheduler.h`) with:
  - `pid`: Unique integer Process ID
  - `arrivalTime`: Integer time of arrival in the ready queue
  - `burstTime`: Integer total CPU execution time
- For RR, an integer `timeQuantum` specifying the time slice duration.

**Example Process Definition**:
```c
Process test1[3] = {
    {1, 0, 10, 0, 0, 0}, // PID=1, Arrival=0, Burst=10
    {2, 1,  5, 0, 0, 0}, // PID=2, Arrival=1, Burst=5
    {3, 2,  8, 0, 0, 0}  // PID=3, Arrival=2, Burst=8
};
int quantum1 = 4; // Time quantum for RR
```

Other fields (`remainingTime`, `startTime`, `completionTime`) are calculated by the algorithms.

## Output Format
Running `scheduler_test` produces output for each test case, including:
- Test case name
- For each algorithm (FCFS, SJF, RR):
  - **Calculated Metrics**: Average TAT, WT, and RT (to 2 decimal places)
  - **Expected Metrics**: Predefined values for validation
- Status: `>>> Test Case X PASSED.` if metrics match within tolerance
- Final: `ALL TESTS PASSED.` if all assertions pass

## How to Compile
1. Navigate to the project root directory.
2. Run:
   ```bash
   make
   ```
   This compiles `scheduler.c` and `scheduler_test.c` into the `scheduler_test` executable.

## How to Run
After compilation, run:
```bash
make run
```
Or directly:
```bash
./scheduler_test
```
This executes all test cases and prints results to the console.

## Sample Program Output
```
==== Test Case 1 ====
FCFS: Calculated: Turnaround: 15.00, Waiting: 7.33, Response: 7.33
      Expected:   Turnaround: 15.00, Waiting: 7.33, Response: 7.33
SJF:  Calculated: Turnaround: 15.00, Waiting: 7.33, Response: 7.33
      Expected:   Turnaround: 15.00, Waiting: 7.33, Response: 7.33
RR (Quantum = 4): Calculated: Turnaround: 19.33, Waiting: 11.67, Response: 3.00
         Expected:   Turnaround: 19.33, Waiting: 11.67, Response: 3.00
>>> Test Case 1 PASSED.

==== Test Case 3 ====
FCFS: Calculated: Turnaround: 5.75, Waiting: 3.25, Response: 3.25
      Expected:   Turnaround: 5.75, Waiting: 3.25, Response: 3.25
SJF:  Calculated: Turnaround: 5.00, Waiting: 2.50, Response: 2.50
      Expected:   Turnaround: 5.00, Waiting: 2.50, Response: 2.50
RR (Quantum = 2): Calculated: Turnaround: 6.25, Waiting: 3.75, Response: 1.75
         Expected:   Turnaround: 6.25, Waiting: 3.75, Response: 1.75
>>> Test Case 3 PASSED.

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
To remove the executable and object files:
```bash
make clean
```

## Notes
- The `Process` struct (`scheduler.h`) holds all timing information: `pid`, `arrivalTime`, `burstTime`, `remainingTime`, `startTime`, `completionTime`.
- SJF is non-preemptive.
- RR is preemptive, with behavior influenced by `timeQuantum`.
- The project uses standard C and is compatible with Unix-like systems (Linux, macOS, WSL) with `gcc` and `make`.