#include "queue.h"
#include "queue_impl.h"
#include <mutex>
#include <condition_variable>


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
        int new_capacity = capacity_ * 2;
        std::vector<std::pair<int, int>> new_data(new_capacity);
        for (int i = 0; i < size_; ++i) {
            new_data[i] = data_[(front_ + i) % capacity_];
        }
        data_ = std::move(new_data);
        capacity_ = new_capacity;
        front_ = 0;
        rear_ = size_;
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