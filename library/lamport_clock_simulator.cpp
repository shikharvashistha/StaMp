#include "lamport_clock_simulator.h"
#include <iostream>

LamportClockSimulator::LamportClockSimulator(int n) : numProcesses(n) {
    lamportClocks.resize(n, 0);
    messageQueues.resize(n);
}

void LamportClockSimulator::processEvent(const Event& event) {
    if (event.type == "send") {
        for (int recipient : event.recipients) {
            messageQueues[recipient].push(event);
        }
        lamportClocks[event.process]++;
    } else if (event.type == "recv") {
        int sender = event.process;
        if (!messageQueues[sender].empty()) {
            Event received = messageQueues[sender].front();
            messageQueues[sender].pop();
            lamportClocks[sender] = std::max(lamportClocks[sender], received.timestamp) + 1;
            processEvent(received);
        }
    } else if (event.type == "print") {
        terminalOutput[event.process] += event.message + " ";
        lamportClocks[event.process]++;
    }
}

void LamportClockSimulator::simulate(const std::vector<std::vector<Event>>& processes) {
    for (int i = 0; i < numProcesses; i++) {
        for (const Event& event : processes[i]) {
            eventQueue.push(event);
        }
    }

    while (!eventQueue.empty()) {
        Event currentEvent = eventQueue.top();
        eventQueue.pop();
        processEvent(currentEvent);
        std::cout << currentEvent.type << " p" << currentEvent.process << " " << currentEvent.message << " "
                  << currentEvent.timestamp << std::endl;
    }

    bool deadlock = true;
    for (int i = 0; i < numProcesses; i++) {
        if (!messageQueues[i].empty()) {
            deadlock = false;
            break;
        }
    }
    if (deadlock) {
        std::cout << "system deadlocked" << std::endl;
    }

    for (int i = 0; i < numProcesses; i++) {
        std::cout << "printed p" << i << " " << terminalOutput[i] << lamportClocks[i] << std::endl;
    }
}
