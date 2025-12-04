Round Robin CPU Scheduling (Simple Simulation)

This is a small backend project for my Operating Systems class.
The goal was to implement a simple Round Robin scheduling simulator in C (no frontend).

The program doesn’t try to be a real OS — it’s just a basic model that shows how Round Robin works with a fixed time quantum.


📝 What the program does
 • Asks the user to enter how many processes there are
 • For each process: arrival time + burst time
 • Simulates the CPU giving each process a fixed quantum (I used 3)
 • If a process doesn’t finish inside the quantum → it goes back to the end of the queue
 • If it finishes → we record when it finished
 • At the end the program calculates:
 • Completion Time (CT)
 • Turnaround Time (TAT = CT – Arrival)
 • Waiting Time (WT = TAT – Burst)

It’s basically a step-by-step demonstration of how Round Robin scheduling works.


⚙️ How to compile

On Windows (MSYS2 MinGW 64-bit):
gcc rr.c -o rr.exe

On Linux/Mac:
gcc rr.c -o rr


▶️ How to run

Windows:
./rr.exe

Linux/Mac:
./rr


🧪 Example (shortened)

Input:
3
0 5
1 3
2 6

Output (example):
Time  0: Running P0
Time  3: Running P1
Time  6: Running P2
...
--- Final Results ---
PID  AT  BT  CT  TAT  WT
P0   0   5   11   11   6
P1   1   3    6    5   2
P2   2   6   14   12   6


Why quantum = 3?

I picked 3 because it’s small enough to show preemption clearly.
If the quantum is too small, the CPU switches too often.
If it’s too big, it basically becomes FCFS.

3 is just a classic number used in OS labs to make the behavior easy to see.


📂 Files
rr.c         # the Round Robin simulator
README.md    # this file


✔️ Notes

This is not a full OS project — just a backend simulation meant for learning and explanation.