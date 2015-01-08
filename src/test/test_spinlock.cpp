#include <iostream>
#include "common/kyrin_lock.h"
#include <pthread.h>

using namespace std;
using namespace kyrin::common;

KyrinMutex melock;
void *func(void *arg)
{
    for (int i=0; i<100; i++) {
        melock.lock();
        cout << (*((int*)arg))-- << endl;
        melock.unlock();
    }
    return NULL;
}

int main()
{
    int ahaha = 10000;
    pthread_t thd[100];

    for (int i=0; i<100; i++) {
        pthread_create(thd+i, NULL, func, &ahaha);
    }

    for (int i=0; i<100; i++) {
        pthread_join(thd[i], NULL);
    }
}
