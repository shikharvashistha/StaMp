#ifndef LAMPORT_CLOCK_SIMULATOR_H
#define LAMPORT_CLOCK_SIMULATOR_H

#include <vector>
#include <string>
#include <queue>
#include <map>

struct Event {
    std::string type;
    int process;
    std::string message;
    std::vector<int> recipients;
    int timestamp;
};

struct EventComparator {
    bool operator()(const Event& e1, const Event& e2) {
        return e1.timestamp > e2.timestamp;
    }
};

class LamportClockSimulator {
private:
    int numProcesses;
    std::vector<int> lamportClocks;
    std::vector<std::queue<Event>> messageQueues;
    std::priority_queue<Event, std::vector<Event>, EventComparator> eventQueue;
    std::map<int, std::string> terminalOutput;

    void processEvent(const Event& event);

public:
    LamportClockSimulator(int n);

    void simulate(const std::vector<std::vector<Event>>& processes);
};

#endif // LAMPORT_CLOCK_SIMULATOR_H
