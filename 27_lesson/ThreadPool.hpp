#pragma once

#include <iostream>
#include <queue>
#include <pthread.h>

typedef void (*handler_t)(int);
class Task{
    private:
        int sock;
        handler_t handler;
    public:
        Task(int sock_=-1, handler_t h_=NULL):sock(sock_), handler(h_)
        {}
        void Run()
        {
            handler(sock);
        }
        ~Task()
        {}
};
class ThreadPool{
    private:
        int thread_nums;
        int idle_nums;
        std::queue<Task> task_queue;
        pthread_mutex_t lock;
        pthread_cond_t cond;
    public:
        void LockQueue()
        {
            pthread_mutex_lock(&lock);
        }
        void UnlockQueue()
        {
            pthread_mutex_unlock(&lock);
        }
        bool IsQueueEmpty()
        {
            return task_queue.size() == 0 ? true : false;
        }
        void IdleThread()
        {
            pthread_cond_wait(&cond, &lock);
        }
        void WakeupOneThread()
        {
            pthread_cond_signal(&cond);
        }
        void TaskPop(Task &t_)
        {
            t_ = task_queue.front();
            task_queue.pop();
        }
    public:
        ThreadPool(int nums_):thread_nums(nums_), idle_nums(0)
        {
            pthread_mutex_init(&lock, NULL);
            pthread_cond_init(&cond, NULL);
        }
        static void *ThreadRoutine(void *arg)
        {
            ThreadPool *tp = (ThreadPool*)arg;
            pthread_detach(pthread_self());
            for(;;){
                tp->LockQueue();
                while(tp->IsQueueEmpty()){
                    tp->IdleThread();
                }
                Task t;
                tp->TaskPop(t);
                tp->UnlockQueue();
                t.Run();
            }
        }
        void InitThreadPool()
        {
            pthread_t tid;
            for(auto i = 0; i < thread_nums; i++){
                pthread_create(&tid, NULL, ThreadRoutine, this);
            }
        }
        void AddTask(const Task &t)
        {
            LockQueue();
            task_queue.push(t);
            WakeupOneThread();
            UnlockQueue();
        }
        ~ThreadPool()
        {
            pthread_mutex_destroy(&lock);
            pthread_cond_destroy(&cond);
        }
};

















