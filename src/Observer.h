/*
 * Observer.h
 *
 *  Created on: 31 Jan 2017
 *      Author: betran
 */

#ifndef OBSERVER_H_
#define OBSERVER_H_


#include <vector>
#include <algorithm>
#include <string>

#include <mutex>
#include <condition_variable>
#include <thread>
#include <chrono>
#include <memory>
#include <atomic>
using namespace std;
template <class T>
class Observer{
	T data;
	std::recursive_mutex mu;
	typedef std::lock_guard<std::recursive_mutex> LockGuard;

public:
	void update(T&& d){
		LockGuard lock(mu);
		if(!(data == d)){
			data = std::move(d);
			notifyListners();
		}
	}
	class Listner{
	public:
			virtual  ~Listner() {}
			virtual void updateData(T data) = 0;
	};
	void registerListner(Listner *l){
		LockGuard lock(mu);
		listners.push_back(l);
	}
	bool unregisterListner(Listner *l){
		LockGuard lock(mu);
		for(auto it = listners.rend(); it != listners.begin(); ){
			it--;
			if(*it == l){
				listners.erase(it);
				return true;
			}
		}
		return false;
	}
private:
	void notifyListners(){
		LockGuard lock(mu);
		for(auto l : listners)
		{
			l->updateData(data);
		}
	}
	std::vector<Listner *> listners;
};

#endif /* OBSERVER_H_ */
