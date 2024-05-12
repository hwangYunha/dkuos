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
    std::pair<int, int> result = {-1, -1};

    if (front_ != rear_) {
        result = data_[front_];
        front_ = (front_ + 1) % capacity_;
    }
    pthread_mutex_unlock(&mutex_lock);

    return result;
    
}

bool CoarseQueue::empty () {
    pthread_mutex_lock(&mutex_lock);
    bool isEmpty = (front_ == rear_);
    pthread_mutex_unlock(&mutex_lock);
    return isEmpty;
}

void FineQueue::enqueue(int key, int value) {
    pthread_mutex_lock(&mutex_lock);
    // rear_에 락을 걸고, data_에 값 넣기
    int index = (rear_ + 1) % capacity_;
    pthread_mutex_t *element_lock = &mutex_lock[index];
    pthread_mutex_lock(element_lock);
    data_[index] = {key, value};
    rear_ = index;
    pthread_mutex_unlock(element_lock);
    pthread_mutex_unlock(&mutex_lock);
}

std::pair<int, int> FineQueue::dequeue() {
    pthread_mutex_lock(&mutex_lock);
	if (front_ == rear_) {
        pthread_mutex_unlock(&mutex_lock);
		return {-1, -1};
    }
    auto front = data_[front_];
    front_ = (front_ + 1) % capacity_;
    pthread_mutex_unlock(&mutex_lock);
    return front;
}

bool FineQueue::empty () {
    pthread_mutex_lock(&mutex_lock);
    bool isEmpty = (front_ == rear_);
    pthread_mutex_unlock(&mutex_lock);
    return isEmpty;
}