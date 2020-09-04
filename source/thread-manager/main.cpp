#include <stdio.h>  // puts
#include <vector>
#include <thread>

// custom library

#include "threadManager.h"

using namespace std;

int main(int argc, char *argv[])
{
	int retVal = 0;

    Job fst = {.fstNumber = 1, .scndNumber = 2};
    Job scnd = {.fstNumber = 0, .scndNumber = 4};
    Job thrd = {.fstNumber = 2, .scndNumber = 3};
    Job fourth = {.fstNumber = 5, .scndNumber = 5};
    Job fifth = {.fstNumber = 8, .scndNumber = 3};

    vector<Job> jobs = {fst, scnd, thrd, fourth, fifth};

	ThreadManager *m = new ThreadManager();

    vector<Result> *result = m->processJobs(&jobs);

    for (Result &r : *result)
    {
        printf("%d\n", r.result);
    }

	delete m;	

	return retVal;
}
