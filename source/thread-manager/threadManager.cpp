#include "threadManager.h"

using namespace std;

ThreadManager::ThreadManager()
{
    threads.reserve(THREAD_COUNT);
}

vector<Result> *ThreadManager::processJobs(vector<Job> *jobs)
{
    // Speicher für Ergebnisvektor anlegen
    this->finalResult.reserve(jobs->capacity());

    // Anzahl der Elemente pro Thread ohne Rest
    int jobsPerThread = floor(jobs->size() / THREAD_COUNT);

    // Bestimmung des Restes für faire Verteilung über die anderen Threads
    int restJobs = jobs->size() % THREAD_COUNT;

    int fstElement = 0;
    int lastElement = jobsPerThread - 1;
    //auto fstElement = jobs->begin();
    //auto lastElement = jobs->begin() + jobsPerThread;

    for (int i = 0; i < THREAD_COUNT; i++)
    {
        // Füge je ein Element zu den Threads hinzu bis der Rest gleichmäßg verteilt wurde
        if (restJobs > 0)
        {
            puts("Job angehängt");
            lastElement += 1;
            restJobs--;
        }

        printf("THREAD %d: Index %d - Index %d\n", i, fstElement, lastElement);
        threads.emplace_back(&ThreadManager::_processJobs, this, jobs, fstElement, lastElement);

        fstElement = lastElement + 1;
        lastElement = fstElement + jobsPerThread - 1;
    }

    for (thread &thread : threads)
    {
        thread.join();
    }
    
    return &finalResult;
}

// Funktion für die einzelnen Threads
void ThreadManager::_processJobs(
    vector<Job> *jobs, 
    int fstElement,
    int lastElement)
    //vector<Job>::iterator fstElement, 
    //vector<Job>::iterator lastElement)
{
    
    for (int curr = fstElement; curr <= lastElement; curr++)
    {
        //printf("%d + %d = %d\n", jobs->at(curr).fstNumber, jobs->at(curr).scndNumber, (jobs->at(curr).fstNumber + jobs->at(curr).scndNumber));
        
        m.lock();
        finalResult.emplace_back( Result {.result = jobs->at(curr).fstNumber + jobs->at(curr).scndNumber});
        m.unlock();
    }
    
}