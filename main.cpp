#include <iostream>
#include <fstream>
#include "header.h"

using namespace std;

globalValues gb;

int main() {
    cout << "How many servers are to be used?: ";
    cin >> gb.servers;

    PriorityQueue priorityQueue;
    eventHeap eventHeap(gb.servers);
    Servers serverQueue(gb.servers);

    ifstream tasks("queue.txt");
    if (!tasks) {
        std::cerr << "Fail to open file " << std::endl;
        return 1;
    }

    double arrive,serve;
    int priority;

    tasks >> arrive >> serve >> priority;

    Customer customer = customerArrive(arrive, serve, priority, "a");
    eventHeap.insertEvent(customer);

    while (!eventHeap.isEmpty()) {

        Customer customerInfo = eventHeap.dequeue();
        gb.totalTime = customerInfo.time;

        if (customerInfo.value == "a") {

            priorityQueue.insert(customerInfo);

            if (serverQueue.serversBusy < serverQueue.totalServers) {
                completeCustomer(priorityQueue, eventHeap, serverQueue, gb.totalTime);
            }

            tasks >> arrive >> serve >> priority;
            if (arrive) {

                gb.customerCount++;
                customer = customerArrive(arrive, serve, priority, "a");
                eventHeap.insertEvent(customer);

            }
        }

        else if (customerInfo.value == "s") {

            gb.totalServe += customerInfo.serviceTime;
            gb.totalQueue += customerInfo.queueTime;
            serverQueue.tellers[customerInfo.serverID].current = "i";
            serverQueue.serversBusy--;

            if (!priorityQueue.isEmpty()) {
                completeCustomer(priorityQueue, eventHeap, serverQueue, gb.totalTime);
            }
        }
    }

    tasks.close();
    outputResults(serverQueue, priorityQueue, gb);

    return 0;
}
