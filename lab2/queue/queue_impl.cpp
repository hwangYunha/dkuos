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
    rear_ = (rear_ + 1) & capacity_;
    pthread_mutex_unlock(&mutex_lock);
}

std::pair<int, int> CoarseQueue::dequeue () {
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

bool CoarseQueue::empty () {
    pthread_mutex_lock(&mutex_lock);
    bool isEmpty = (front_ == rear_);
    pthread_mutex_unlock(&mutex_lock);
    return isEmpty;
}

void FineQueue::enqueue (int key, int value) {
    pthread_mutex_lock(&mutex_lock);
    if (size_ >= capacity_) {
        pthread_cond_wait(&de_cv_, &mutex_lock);
    }
    data_[rear_] = {key, value};
    rear_ = (rear_ + 1) % capacity_;
    ++size_;
    pthread_cond_signal(&en_cv_);
    pthread_mutex_unlock(&mutex_lock);
}

std::pair<int, int> FineQueue::dequeue () {
	pthread_mutex_lock(&mutex_lock);
    	while (size_ <= 0) {
			pthread_cond_wait(&en_cv_, &mutex_lock);
    	}
    	auto item = data_[front_];
    	front_ = (front_ + 1) % capacity_;
    	--size_;
    	pthread_cond_signal(&de_cv_);
        pthread_mutex_unlock(&mutex_lock);
    	return item;
}

bool FineQueue::empty () {
    pthread_mutex_lock(&mutex_lock);
    bool result = size_ <= 0;
    pthread_mutex_unlock(&mutex_lock);
    return result;
}