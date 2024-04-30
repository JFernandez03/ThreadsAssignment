#include <iostream>
#include <thread>
#include <semaphore.h>
#include <queue>
#include <string>
#include <chrono>
#include <ctime>

using namespace std;

struct requestStructure {
    int request_id;
    string ip_address;
    string page_requested;
};


string webPages[10] = {"google.com", "yahoo.com", "duckDuckGo.com", "wikipedia.com", "bing.com", "facebook.com", "amazon.com", "instagram.com", "reddit.com", "ebay.com"};

const int VALUE = 5;
queue<requestStructure> myQueue;

sem_t locked;
sem_t filled;
sem_t emptySemaphore;



int request_id = 0;

void listen() {
    while (true) {

        this_thread::sleep_for(chrono::seconds(1));

        requestStructure newReq;
        newReq.request_id = request_id;
        newReq.ip_address = "";
        newReq.page_requested = webPages[rand() % 10];

        sem_wait(&emptySemaphore);
        sem_wait(&locked);

        myQueue.push(newReq);
        cout << "The following request: " << request_id << "  has been added to the message queue." << endl;
        request_id++;

        sem_post(&locked);
        sem_post(&filled);
    }
}

void do_request(int thread_id) {
    while (true) {
        sem_wait(&filled);
        sem_wait(&locked);
        if (myQueue.empty()) {
            sem_post(&locked);
            continue;
        }
        requestStructure reqStruc = myQueue.front();
        myQueue.pop();
        cout << "The thread " << thread_id << " has completed the request " << reqStruc.request_id << ". It is now requesting the following webpage: " << reqStruc.page_requested << endl;
        sem_post(&locked);
        sem_post (&emptySemaphore);

    }
}

int main() {
    
    sem_init(&emptySemaphore, 0, VALUE);
    sem_init(&filled, 0, 0);
    sem_init(&locked, 0, 1);


    thread listener(listen);
    int numThreads = 4;
    thread worker [numThreads];
    for (int i = 0; i < numThreads; i++) {
        worker[i] = thread(do_request, i);
    }


    listener.join();
    for (int i = 0; i < numThreads; i++) {
        worker[i].join();
    }


    sem_destroy(&emptySemaphore);
    sem_destroy(&filled);
    sem_destroy(&locked);

    return 0;
}