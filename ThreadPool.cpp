#include <iostream>
#include <cassert> 
#include <cstdio>
#include "ThreadPool.h"


namespace MYWORK {
    ThreadPool::ThreadPool(int threadNum) {
        this->threadNum = threadNum;
        createThreads();
        isRunning = true;
    }

    ThreadPool::~ThreadPool() {
        stop();
    }
    
    int ThreadPool::createThreads() {
        pthread_mutex_init(&mutex, NULL);
        pthread_cond_init(&condition, NULL);
        threads = (pthread_t*)malloc(sizeof(pthread_t)*threadNum);
        for (int i = 0; i < threadNum; ++i) {
            pthread_create(&threads[i], NULL, threadFunc, this);
        }
        return 0;
    }

    size_t ThreadPool::addTask(const Task& task) {
        pthread_mutex_lock(&mutex);
        taskQueue.push_back(task);
        int size = taskQueue.size();
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&condition);
        return size;
    }

    void ThreadPool::stop() {
        if (!isRunning) {
            return;
        }
        isRunning = false;
        pthread_cond_broadcast(&condition);
        for (int i = 0;i < threadNum; ++i) {
            pthread_join(threads[i], NULL);
        }
        free(threads);
        threads = NULL;
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&condition);
    }

    ThreadPool::Task ThreadPool::take() {
        Task task = NULL;
        pthread_mutex_lock(&mutex);
        while (taskQueue.empty() && isRunning) {
            pthread_cond_wait(&condition, &mutex);
        }
        if (!isRunning) {
            pthread_mutex_unlock(&mutex);
            return task;
        }
        assert(!taskQueue.empty());
        task = taskQueue.front();
        taskQueue.pop_front();
        pthread_mutex_unlock(&mutex);
        return task;
    }

    void* ThreadPool::threadFunc(void* arg) {
        pthread_t tid = pthread_self();    
        ThreadPool* pool = static_cast<ThreadPool*>(arg);
        while (pool->isRunning) {
            ThreadPool::Task task = pool->take();
            if (!task) {
                fprintf(stderr, "thread %lu exited\n", tid);
                break;
                //continue;
            }
            assert(task);
            task();
        }
        return 0;
    }
}
