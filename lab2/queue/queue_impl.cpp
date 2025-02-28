// 32215002 황윤하

#include "queue.h"
#include "queue_impl.h"


void Queue::enqueue (int key, int value) {
    data_[rear_] = {key, value};
    rear_ = (rear_ + 1) % capacity_;
}

std::pair<int, int> Queue::dequeue () {
    if (front_ == rear_) {
        return {-1, -1};
    }
    auto front = data_[front_];
    front_ = (front_ + 1) % capacity_;
    return front;
}

bool Queue::empty () {
    return front_ == rear_;
}

void CoarseQueue::enqueue (int key, int value) {
	pthread_mutex_lock(&mutex_lock);
    data_[rear_] = {key, value};
    rear_ = (rear_ + 1) % capacity_;
    pthread_mutex_unlock(&mutex_lock);
}

std::pair<int, int> CoarseQueue::dequeue () {
    pthread_mutex_lock(&mutex_lock);
    auto front = data_[front_];
    if (front_ == rear_) {
        front = {-1, -1};
    }
    else {
    front_ = (front_ + 1) % capacity_;
    }
    pthread_mutex_unlock(&mutex_lock);
    return front;
}

bool CoarseQueue::empty () {
    pthread_mutex_lock(&mutex_lock);
    bool isEmpty = (front_ == rear_);
    pthread_mutex_unlock(&mutex_lock);
    return isEmpty;
}

void FineQueue::enqueue(int key, int value) {
    std::pair<int, int> newItem = {key, value};
	pthread_mutex_lock(&mutex_lock);
    data_[rear_] = newItem;
    rear_ = (rear_ + 1) % capacity_;
    pthread_mutex_unlock(&mutex_lock);
}

std::pair<int, int> FineQueue::dequeue() {
    std::pair<int, int> front = {-1, -1};
    pthread_mutex_lock(&mutex_lock);
    if (front_ != rear_) {
        front = data_[front_];
        front_ = (front_ + 1) % capacity_;
    }
    pthread_mutex_unlock(&mutex_lock);
    return front;
}
    
bool FineQueue::empty () {
    return (front_ == rear_);
}