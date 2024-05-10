#include "queue.h"
#include "queue_impl.h"


void Queue::enqueue (int key, int value) {
    std::lock_guard<std::mutex> lock(mutex_);
    data_.push_back({key, value});
}

std::pair<int, int> Queue::dequeue () {
	std::lock_guard<std::mutex> lock(mutex_);
	if (data_.empty())
		return {-1, -1};
	auto front = data_.front();
	data_.erase(data_.begin());
    return front;
}

bool Queue::empty () {
    std::lock_guard<std::mutex> lock(mutex_);
    return data_.empty();
}

void CoarseQueue::enqueue (int key, int value) {
	std::lock_guard<std::mutex> lock(mutex_);
    data_.push_back({key, value});
}

std::pair<int, int> CoarseQueue::dequeue () {
	std::lock_guard<std::mutex> lock(mutex_);
	if (data_.empty())
		return {-1, -1};
    auto front = data_.front();
    data_.erase(data_.begin());
    return front;
}

bool CoarseQueue::empty () {
    std::lock_guard<std::mutex> lock(mutex_);
    return data_.empty();
}

void FineQueue::enqueue (int key, int value) {
    pthread_mutex_lock(&mutex_lock);
    while (size_ >= capacity_) {
        pthread_cond_wait(&cv_, &mutex_lock);
    }
    data_[rear_] = {key, value};
    rear_ = (rear_ + 1) % capacity_;
    ++size_;
    pthread_cond_signal(&cv_);
    pthread_mutex_unlock(&mutex_lock);
}

std::pair<int, int> FineQueue::dequeue () {
	pthread_mutex_lock(&mutex_lock);
    	while (size_ <= 0) {
			pthread_cond_wait(&cv_, &mutex_lock);
    	}
    	auto item = data_[front_];
    	front_ = (front_ + 1) % capacity_;
    	--size_;
    	pthread_cond_signal(&cv_);
        pthread_mutex_unlock(&mutex_lock);
    	return item;
}

bool FineQueue::empty () {
    pthread_mutex_lock(&mutex_lock);
    bool result = size_ <= 0;
    pthread_mutex_unlock(&mutex_lock);
    return result;
}