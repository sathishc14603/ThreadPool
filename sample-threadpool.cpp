// sample-threadpool.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include<chrono>
#include "ThreadPool.cpp";
std::mutex mtx;

using  namespace std;
    void generate(int i) {
        
            unique_lock<mutex>lock(mtx);
            std::cout << i <<"id="<< std::this_thread::get_id() << "\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));


       
       
}

int main()
{
    ThreadPool obj;
    obj.pool(3);
    for (int i = 0; i < 50; ++i) {       
        obj.queuejobs([i]()->void {
            generate(i);
            });
    }


    }


