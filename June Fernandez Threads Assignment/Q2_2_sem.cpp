#include <iostream>
#include <thread>
#include <semaphore.h>


using namespace std;

sem_t semaphoreOne;
sem_t semaphoreTwo;



void runMeFirst()
{
    cout << "Run me first" << endl;
    sem_post(&semaphoreOne);
}

void runMeSecond()
{
    sem_wait(&semaphoreOne);
    cout << "I run second" << endl;
    sem_post(&semaphoreTwo);
}

void runMeThird()
{
    sem_wait(&semaphoreTwo);
    cout << "I run third" << endl;
}

int main()
{
    sem_init(&semaphoreOne, 0, 0);
    sem_init(&semaphoreTwo, 0, 0);
    

    thread t1 = thread(runMeFirst);
    thread t2 = thread(runMeSecond);
    thread t3 = thread(runMeThird);

    t1.join();
    t2.join();
    t3.join();

	
    sem_destroy(&semaphoreOne); 
    sem_destroy(&semaphoreTwo);
    return 0;
}