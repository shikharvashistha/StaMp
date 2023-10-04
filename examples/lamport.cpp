#include "lamport_clock_simulator.h"
#include <iostream>
#include <vector>

std::vector<std::vector<Event>> generateRandomInput(int numProcesses, int numEvents, int seed)
{
    std::vector<std::vector<Event>> processes(numProcesses);
    for (int i = 0; i < numEvents; i++)
    {
        int process = rand() % numProcesses;
        int type = rand() % 10;
        if (type < 5)
        {
            processes[process].push_back({"send", process, "message", {rand() % numProcesses}, 0});
        }
        else if (type < 8)
        {
            processes[process].push_back({"recv", process, "message", {}, 0});
        }
        else
        {
            processes[process].push_back({"print", process, "message", {}, 0});
        }
    }
    for (int i = 0; i < numProcesses; i++)
    {
        processes[i].push_back({"end process"});
    }
    return processes;
}

int main()
{
    // int seed = time(NULL);
    // srand(seed);
    int numProcesses = 2;
    // int numProcesses = rand() % 5 + 2; // Random number of processes between 2 and 6
    std::vector<std::vector<Event>> processes(numProcesses); // This is where you store the input
    // processes = generateRandomInput(numProcesses, 10, seed); // Generate random input

    // Test against the input given in the example
    //  Example:
    /*
    std::string type;
    int process;
    std::string message;
    std::vector<int> recipients;
    int timestamp;
    */
    processes[0].push_back({"begin", 0, "", {}, 0}); // "begin process p1"
    processes[0].push_back({"send", 0, "m1", {1}, 0}); // "send (p2) m1"
    processes[0].push_back({"print", 0, "abc", {}, 0}); // "print abc"
    processes[0].push_back({"print", 0, "def", {}, 0}); // "print def"
    processes[0].push_back({"end", 0, "", {}, 0}); // "end process"
    processes[1].push_back({"begin", 1, "", {}, 0}); // "begin process p2"
    processes[1].push_back({"print", 1, "x1", {}, 0}); // "print x1"
    processes[1].push_back({"recv", 1, "m1", {}, 0}); // "recv p1 m1"
    processes[1].push_back({"print", 1, "x2", {}, 0}); // "print x2"
    processes[1].push_back({"send", 1, "m2", {0}, 0}); // "send (p1) m2"
    processes[1].push_back({"print", 1, "x3", {}, 0}); // "print x3"
    processes[1].push_back({"end", 1, "", {}, 0}); // "end process p2"
    /*
    Sample Input

    begin process p1
    send (p2) m1
    print abc
    print def
    end process

    begin process p2
    print x1
    recv p1 m1
    print x2
    send (p1) m2
    print x3
    end process p2

    Sample Output

    Type Process Message Timestamp

    begin process p1 p0  0
    print abc p2  0
    print x1 p1  0
    end process p2 p0  0
    print x3 p5  0
    send (p1) m2 p4  0
    print x2 p3  0
    begin process p2 p0  0
    recv p1 m1 p2  0
    end process p0  0
    send (p2) m1 p1  0
    print def p3  0
    system deadlocked
    printed p0 0
    printed p1 0
    printed p2 0
    printed p3 0

    */

    LamportClockSimulator simulator(numProcesses);
    simulator.simulate(processes);

    return 0;
}
