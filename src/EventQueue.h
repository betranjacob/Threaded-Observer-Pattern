/*
 * Queue.h
 *
 *  Created on: 27 Jan 2017
 *      Author: betran
 */

#ifndef EVENTQUEUE_H_
#define EVENTQUEUE_H_
#include <deque>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>

template <class T>
class EventQueue
{
private:
	std::deque<T> e_queue ;
    std::mutex m;
    std::condition_variable cv;
public:
	EventQueue()  = default;

	void insert(T&& item){
		std::lock_guard<std::mutex> lg(m);
		e_queue.emplace_back(std::move(item));
	    cv.notify_one();
	}
	auto remove() {
			std::unique_lock<std::mutex> ul(m);
			cv.wait(ul,[this]{return  e_queue.size();});
			T t = std::move(e_queue.front()); // After moving this item goes to an undefined state.
			e_queue.pop_front();
			return t;
	}
};

#endif /* EVENTQUEUE_H_ */
