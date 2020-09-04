#include <vector>
#include <iostream>
#include <thread>
#include <cmath>
#include <mutex>
#include <utility>

#pragma once

#define THREAD_COUNT 3

/*
struct Job
{
	std::vector<float> imageData; 	// Bildinformationen
	std::vector<float> expectation; // erwarteter Ergebnisvektor
	enum Mode {training, test}; 	// Modusvorgabe
};
*/

struct Job
{
    int fstNumber;
    int scndNumber;
};

struct Result
{
    int result;
};

class ThreadManager
{
private:
    std::mutex m;
    std::vector<std::thread> threads;
    std::vector<Result> finalResult;
    void _processJobs(
        std::vector<Job> *jobs, 
        int fstElement,
        int lastElement);
        //std::vector<Job>::iterator fstElement, 
        //std::vector<Job>::iterator lastElement);

public:
    ThreadManager();
    std::vector<Result> *processJobs(std::vector<Job> *jobs);
};
