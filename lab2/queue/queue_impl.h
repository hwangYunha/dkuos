#include "queue.h"

#ifndef QUE_IMPI_H
#define QUE_IMPI_H

/**
 * @brief Queue without lock
 * DefaultQueue의 함수를 오버라이드하여, 클래스를 완성한다.
 * 구현에 필요한 멤버 변수/함수를 추가하여 구현한다.
 */
class Queue : public DefaultQueue {
	private:
 	   std::pair<int, int> *data_;
	   int capacity_;
	   int front_;
	   int rear_;
	public:
	    Queue() : capacity_(1000000), front_(0), rear_(0) {
        	data_ = new std::pair<int, int>[capacity];
    	}

	    ~Queue() {
    	    delete[] data_;
    	}

        void enqueue (int key, int value) override;
        std::pair<int, int> dequeue () override;
		bool empty() override;
};


/**
 * @brief Queue with coarse-grained lock
 * Queue 전체를 critical section으로 가정하여, 하나의 lock으로 이를 관리한다.
 * DefaultQueue의 함수를 오버라이드하여, 클래스를 완성한다.
 * 구현에 필요한 멤버 변수/함수를 추가하여 구현한다.
 */
class CoarseQueue : public DefaultQueue {
	private:
		std::pair<int, int> *data_;
	    int capacity_;
   		int front_;
    	int rear_;

		pthread_mutex_t mutex_lock;
	
	public:
		CoarseQueue() : capacity_(1000000), front_(0), rear_(0) {
       		data_ = new std::pair<int, int>[capacity];
        	pthread_mutex_init(&mutex_lock, NULL);
    	}

    	~CoarseQueue() {
        	delete[] data_;
        	pthread_mutex_destroy(&mutex_lock);
    	}

        void enqueue (int key, int value) override;
        std::pair<int, int> dequeue () override;
		bool empty() override;
};


/**
 * @brief Queue with fine-grained lock
 * Queue 내부의 critical section을 개별적으로 lock으로 관리한다.
 * DefaultQueue의 함수를 오버라이드하여, 클래스를 완성한다.
 * 구현에 필요한 멤버 변수/함수를 추가하여 구현한다.
 */
class FineQueue : public DefaultQueue {
	private:
		std::pair<int, int> *data_;
    	int capacity_;
    	int front_;
    	int rear_;

		pthread_mutex_t mutex_lock;
		pthread_cond_t cv_;
		pthread_cond_t en_cv;
		pthread_cont_t de_cv;

	public:
		// 멤버 함수 추가 선언 가능

		FineQueue() : capacity_(1000000), front_(0), rear_(0) {
		    data_ = new std::pair<int, int>[capacity_];
    		pthread_mutex_init(&mutex_lock, NULL);
    		pthread_cond_init(&cv_, NULL);
		}

    	~FineQueue() {
        	delete[] data_;
        	pthread_mutex_destroy(&mutex_lock);
        	pthread_cond_destroy(&cv_);
   		}

        void enqueue (int key, int value) override;
        std::pair<int, int> dequeue () override;
		bool empty() override;
};
#endif