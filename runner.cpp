#include "header.h"
#include <iostream>

Customer customerArrive(double arrival, double duration, int priority, std::string value) {

    Customer customerEvent;
    customerEvent.time = arrival;
    customerEvent.arrivalTime = arrival;
    customerEvent.serviceTime = duration;
    customerEvent.priority = priority;
    customerEvent.queueTime = 0.0;
    customerEvent.value = value;

    return customerEvent;
}

PriorityQueue::PriorityQueue() : currentSize(0), maxLength(0){}

void PriorityQueue::heapify(int index) {

    if (index == 0) {return;}

    int parentIndex = (index - 1) / 2;

    if (customerHeap[index].priority > customerHeap[parentIndex].priority) {

        std::swap(customerHeap[index], customerHeap[parentIndex]);
        heapify(parentIndex);

    }
}

void PriorityQueue::heapifyDown(int index) {

    int leftChild = 2 * index + 1;
    int rightChild = 2 * index + 2;
    int largest = index;

    if (leftChild < currentSize &&
        (customerHeap[leftChild].priority > customerHeap[largest].priority ||
         (customerHeap[leftChild].priority == customerHeap[largest].priority &&
          customerHeap[leftChild].arrivalTime < customerHeap[largest].arrivalTime))) {

        largest = leftChild;

    }

    if (rightChild < currentSize &&
        (customerHeap[rightChild].priority > customerHeap[largest].priority ||
         (customerHeap[rightChild].priority == customerHeap[largest].priority &&
          customerHeap[rightChild].arrivalTime < customerHeap[largest].arrivalTime))) {

        largest = rightChild;

    }

    if (largest != index) {

        std::swap(customerHeap[index], customerHeap[largest]);
        heapifyDown(largest);

    }

}

void PriorityQueue::insert(Customer& c) {

    customerHeap[currentSize] = c;
    heapify(currentSize);
    currentSize++;
    maxLength = std::max(maxLength,currentSize);

}

Customer PriorityQueue::extractMax() {

    Customer max = customerHeap[0];

    customerHeap[0] = customerHeap[currentSize - 1];
    currentSize--;

    heapifyDown(0);
    return max;
}

bool PriorityQueue::isEmpty() const{
    return currentSize ==0;
}

eventHeap::eventHeap(int c) {

    eventsHolder = c+1;
    customerEvent = new Customer[eventsHolder];
    eventsExist = 0;

}


void eventHeap::insertEvent(Customer& customer) {

    customerEvent[eventsExist++] = customer;
    heapifyUp(eventsExist - 1);

}


Customer eventHeap::dequeue() {

    Customer cust = customerEvent[0];
    customerEvent[0] = customerEvent[--eventsExist];

    heapifyDown(0);
    return cust;
}

bool eventHeap::isEmpty() const {
    return eventsExist == 0;
}


void eventHeap::heapifyUp(int top) {

    while (top > 0 && customerEvent[top].time < customerEvent[(top - 1) / 2].time) {
        std::swap(customerEvent[top], customerEvent[(top - 1) / 2]);
        top = (top - 1) / 2;
    }
}

void eventHeap::heapifyDown(int top) {

    int smallest = top;
    int leftChild = 2 * top + 1;
    int rightChild = 2 * top + 2;

    if (leftChild < eventsExist && customerEvent[leftChild].time < customerEvent[smallest].time) {
        smallest = leftChild;
    }
    if (rightChild < eventsExist && customerEvent[rightChild].time < customerEvent[smallest].time) {
        smallest = rightChild;
    }

    if (smallest != top) {
        std::swap(customerEvent[top], customerEvent[smallest]);
        heapifyDown(smallest);
    }
}

int Servers::addToCustomer(double start, double duration) {

    int selected = -1;

    for (int i = nextServer; selected == -1; i = (i + 1) % totalServers) {
        if (tellers[i].current == "i") {
            selected = i;
        }
    }

    Server& assignedTeller = tellers[selected];
    assignedTeller.totalServed++;
    assignedTeller.idle += start - assignedTeller.lastFinish;
    assignedTeller.lastFinish = start + duration;
    assignedTeller.current = "w";
    serversBusy++;

    nextServer = selected + 1;
    if (nextServer >= totalServers) {
        nextServer = 0;
    }

    return selected;
}

Servers::Servers(int c) : totalServers(c), serversBusy(0), nextServer(0), tellers(new Server[totalServers]){}

void completeCustomer(PriorityQueue& p, eventHeap& e, Servers& s, double &totalTime){

    Customer customer = p.extractMax();
    customer.value = "s";
    customer.time = totalTime + customer.serviceTime;
    customer.queueTime = totalTime - customer.arrivalTime;
    customer.serverID = s.addToCustomer(totalTime, customer.serviceTime);
    e.insertEvent(customer);

}

void outputResults(const Servers& serverQueue, const PriorityQueue& arrivalQueue, const globalValues& gb){

    using namespace std;

    cout << "\n| Customers Served by Tellers   |" << endl;
    cout << "-------------------------------------"<<endl;
    for (int i = 0; i < gb.servers; i++) {
        cout << "Teller " << i+1 << " served a total of " << serverQueue.tellers[i].totalServed << " people." << endl;
    }
    cout << "-------------------------------------"<<endl;

    cout << "| Simulation Ran For: " << gb.totalTime << "       |"<< endl;
    cout << "| Average Service Time: "<< gb.totalServe / gb.customerCount << "     |" << endl;
    cout << "| Average Waiting Time: " << gb.totalQueue / gb.customerCount << "     |" << endl;

    cout << "| Maximum Length of the Queue: " << arrivalQueue.maxLength << "   |" << endl;
    cout << "| Average Length of the Queue: " << gb.totalQueue / gb.totalTime << "   |" << endl;
    cout << "| Average Idle Rate of Each Teller  |" << endl;

    for (int i = 0; i < gb.servers; i++) {
        cout << "Teller [" << i+1 << "]: " << serverQueue.tellers[i].idle / gb.totalTime << endl;
    }
}