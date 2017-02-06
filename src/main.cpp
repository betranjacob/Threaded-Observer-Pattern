//============================================================================
// Name        : main.cpp
// Author      : Betran Jacob
// Version     :
// Copyright   : GPL
// Description : Threaded-Observer-Pattern
//============================================================================

#include <iostream>
#include <thread>
#include "EventQueue.h"
#include "Observer.h"

#include <unistd.h>
#include <iomanip>

using namespace std;

struct Resource {
    string _s;

    Resource():_s("Nil") {
        //cout << "ctor() " << _s << endl;
    }
    Resource(string s):_s(s) {
        //cout << "p-ctor() " << _s  << endl;
    }

    Resource(const Resource& lhs) {
        _s = lhs._s;
        //cout << "c-ctor() " << _s  << endl;
    }
    Resource& operator=(const Resource& lhs) {
        _s = lhs._s;
        //cout << "c=ctor() " << _s  << endl;
        return *this;
    }
    Resource(const Resource&& lhs) {
        _s = std::move(lhs._s);
        //cout << "m-ctor() " << _s  << endl;
    }
    Resource& operator=(const Resource&& lhs) {
        _s = std::move(lhs._s);
        //cout << "m=ctor() " << _s  << endl;
        return *this;
    }
    bool operator==(const Resource& lhs) {
        return lhs._s == this->_s;
    }

};

class IListner : public Observer<Resource>::Listner
{
public:
	EventQueue<Resource> q;
	IListner() = default;
	void updateData(Resource item) { q.insert(std::move(item)); };
	void readData(int id) {
		while(1)
		{
		    Resource data = q.remove();
			cout << "T:" << id << " - " << std::quoted(data._s) << std::endl;
			this_thread::sleep_for(chrono::milliseconds(10));
		}
	}
	std::thread spawnReadThread(int id){
	    return std::thread([=] { readData(id); });
	}
};

int main() {

    Observer<Resource> ob;

    IListner *it1 = new IListner();
    ob.registerListner(it1);
    thread t1 = it1->spawnReadThread(1);
#if 1
    IListner *it2 = new IListner();
    ob.registerListner(it2);
    thread t2 = it2->spawnReadThread(2);

    IListner *it3 = new IListner();
    ob.registerListner(it3);
    thread t3 = it3->spawnReadThread(3);

    IListner *it4 = new IListner();
    ob.registerListner(it4);
    thread t4 = it4->spawnReadThread(4);
#endif
    for (int i=1; i<=10; ++i)
        ob.update((Resource("Old : " + to_string(i))));
    for (int i=1; i<=10; ++i)
        ob.update((Resource("New : " + to_string(i))));

    t1.join();
 //   t2.join();

	return 0;
}
