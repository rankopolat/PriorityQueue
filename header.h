#ifndef ASS2CSCI203_HEADER_H
#define ASS2CSCI203_HEADER_H
#include <string>

const int MAX_SIZE = 100;

struct globalValues{

    int customerCount = 1;
    double totalServe = 0;
    double totalQueue = 0;
    double totalTime = 0;
    int servers = 0;

};

struct Customer{

    std::string value;
    double time;
    double arrivalTime;
    int priority;
    double serviceTime;
    double queueTime;
    int serverID;

};

struct Server{

    std::string current = "i";
    int totalServed = 0;
    double idle = 0;
    double lastFinish = 0;

};

class PriorityQueue{

private:

    Customer customerHeap[MAX_SIZE];
    int currentSize;

public:

    int maxLength;

    PriorityQueue();
    void heapify(int);
    void heapifyDown(int);
    void insert(Customer&);
    Customer extractMax();
    bool isEmpty() const;

};

class eventHeap {

    Customer *customerEvent;
    int eventsExist;
    int eventsHolder;

public:

    eventHeap(int);
    void insertEvent(Customer&);
    Customer dequeue();
    bool isEmpty() const;
    void heapifyDown(int);
    void heapifyUp(int);
};

class Servers{

public:

    int totalServers;
    int serversBusy;
    int nextServer;
    Server *tellers;

    Servers(int);
    int addToCustomer(double, double);

};

void completeCustomer(PriorityQueue&, eventHeap&, Servers&, double&);
void outputResults(const Servers&, const PriorityQueue&, const globalValues&);
Customer customerArrive(double, double, int, std::string);

#endif //ASS2CSCI203_HEADER_H
