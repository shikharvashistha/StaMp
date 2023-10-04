#include "lamport_clock_simulator.h"
#include <iostream>

LamportClockSimulator::LamportClockSimulator(int n) : numProcesses(n)
{
    lamportClocks.resize(n, 0);
    messageQueues.resize(n);

    for (int i = 0; i < n; i++)
    {
        terminalOutput[i] = "";
    }

    eventQueue = std::priority_queue<Event, std::vector<Event>, EventComparator>();
}

void LamportClockSimulator::processEvent(const Event &event)
{
    if (event.type == "send")
    {
        int sender = event.process;
        for (int recipient : event.recipients)
        {
            messageQueues[recipient].push(event);
        }
        lamportClocks[sender]++;
    }
    else if (event.type == "recv")
    {
        int recipient = event.process;
        if (!messageQueues[recipient].empty())
        {
            Event message = messageQueues[recipient].front();
            messageQueues[recipient].pop();
            lamportClocks[recipient] = std::max(lamportClocks[recipient], message.timestamp) + 1;
            eventQueue.push(message);
        }
        lamportClocks[recipient]++;
    }
    else if (event.type == "print")
    {
        int process = event.process;
        terminalOutput[process] += event.message + " p" + std::to_string(process) + " " + std::to_string(lamportClocks[process]) + "\n";
        lamportClocks[process]++;
    }
    else if (event.type == "begin")
    {
        lamportClocks[event.process]++;
    }
    else if (event.type == "end")
    {
        lamportClocks[event.process]++;
    }
    else
    {
        std::cout << "Error: Invalid event type. Please use send, recv, or print."
                  << "You sent: " << event.type << std::endl; // "Error: Invalid event type. Please use send, recv, or print.
    }
}

void LamportClockSimulator::simulate(const std::vector<std::vector<Event>> &processes)
{
    for (int i = 0; i < numProcesses; i++)
    {
        for (Event event : processes[i])
        {
            eventQueue.push(event);
        }
    }

    while (!eventQueue.empty())
    {
        Event event = eventQueue.top();
        eventQueue.pop();
        processEvent(event);
    }

    bool deadlock = true;
    for (int i = 0; i < numProcesses; i++)
    {
        if (!messageQueues[i].empty())
        {
            deadlock = false;
        }
    }

    if (deadlock)
    {
        std::cout << "system deadlocked" << std::endl; // "system deadlocked"
        exit(0);
    }

    std::cout << "Message Process Timestamp" << std::endl;

    for (int i = 0; i < numProcesses; i++)
    {
        std::cout << terminalOutput[i];
    }

    std::cout << std::endl;

    std::cout << "The lamport clocks are: " << std::endl; // "The lamport clocks are:

    for (int i = 0; i < numProcesses; i++)
    {
        std::cout << "printed p" << i << " " << lamportClocks[i] << std::endl; // "printed p" << i << " " << lamportClocks[i] << std::endl;
    }

    return;
}
