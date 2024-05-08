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
    std::unique_lock<std::mutex> lock(mutex_);
    while (size_ >= capacity_) {
        cv_.wait(lock);
    }
    data_[rear_] = {key, value};
    rear_ = (rear_ + 1) % capacity_;
    ++size_;
    cv_.notify_one();
}

std::pair<int, int> FineQueue::dequeue () {
	std::unique_lock<std::mutex> lock(mutex_);
    	while (size_ <= 0) {
			cv_.wait(lock);
    	}
    	auto item = data_[front_];
    	front_ = (front_ + 1) % capacity_;
    	--size_;
    	cv_.notify_one();
    	return item;
}

bool FineQueue::empty () {
    std::unique_lock<std::mutex> lock(mutex_);
    return size_ <= 0;
}