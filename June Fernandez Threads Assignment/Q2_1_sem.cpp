#include <iostream>
#include <thread>
#include <semaphore.h>
#include <chrono>

using namespace std;

sem_t semaphoreOne;


void runMeFirst()
{
    cout << "Run me first" << endl;
}

void runMeSecond()
{
    sem_wait(&semaphoreOne);
    cout << "I run second" << endl;
}

void signalSemaphore()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    sem_post(&semaphoreOne);
}

int main()
{
    
    
    sem_init(&semaphoreOne, 0, 0);
    

    thread t1 = thread(runMeFirst);
    thread t2 = thread(runMeSecond);
    thread signal(signalSemaphore);

    
    t1.join();
    t2.join();
    signal.join();

    sem_destroy(&semaphoreOne); 
    return 0;
}