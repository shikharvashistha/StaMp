#include "lamport_clock_simulator.h"
#include <iostream>
#include <vector>

std::vector<std::vector<Event>> generateRandomInput(int numProcesses, int numEvents, int seed) {
    std::vector<std::vector<Event>> processes(numProcesses);
    for (int i = 0; i < numEvents; i++) {
        int process = rand() % numProcesses;
        int type = rand() % 10;
        if (type < 5) {
            processes[process].push_back({"send", process, "message", {rand() % numProcesses}, 0});
        } else if (type < 8) {
            processes[process].push_back({"recv", process, "message", {}, 0});
        } else {
            processes[process].push_back({"print", process, "message", {}, 0});
        }
    }
    for (int i = 0; i < numProcesses; i++) {
        processes[i].push_back({"end process"});
    }
    return processes;
}


int main() {
    // int seed = time(NULL);
    // srand(seed);
    int numProcesses = 2;
    // int numProcesses = rand() % 5 + 2; // Random number of processes between 2 and 6
    std::vector<std::vector<Event>> processes(numProcesses); // This is where you store the input
    // processes = generateRandomInput(numProcesses, 10, seed); // Generate random input
    
    //Test against the input given in the example
    // Example:
    processes[0].push_back({"begin process p1"});
    processes[0].push_back({"send (p2) m1", 1});
    processes[0].push_back({"print abc", 2});
    processes[0].push_back({"print def", 3});
    processes[0].push_back({"end process"});

    processes[1].push_back({"begin process p2"});
    processes[1].push_back({"print x1", 1});
    processes[1].push_back({"recv p1 m1", 2});
    processes[1].push_back({"print x2", 3});
    processes[1].push_back({"send (p1) m2", 4});
    processes[1].push_back({"print x3", 5});
    processes[1].push_back({"end process p2"});

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
    
    */
    

    LamportClockSimulator simulator(numProcesses);
    simulator.simulate(processes);

    return 0;
}
