#include<iostream>
#include<vector>
#include<mutex>
#include<thread>
#include<functional>
#include<condition_variable>
#include<queue>
#include<string>;
using namespace std;
class ThreadPool {
public:
	void pool(int num_pool) {
		for (int i = 0; i < num_pool; ++i) {
			threads.emplace_back([this] 
				{
					while (true) {

						function<void()>job;
						{
							unique_lock<mutex>lock(job_mutex);
							mutex_condition.wait(
								lock,[this]{
									return !jobs.empty() || stop_not;
								});
							if (stop_not && jobs.empty()) return;
							job = jobs.front();
							jobs.pop();
						}
						job();
					}

				});
		}
	}
	void queuejobs(std::function<void()> job) {
		{
			unique_lock<mutex>loc(job_mutex);
			jobs.push(job);
		}
		mutex_condition.notify_one();

	}
	~ThreadPool() {
		{
			unique_lock<mutex>lock(job_mutex);
			stop_not = true;
		}
		mutex_condition.notify_all();
		for (std::thread&  t:threads) {
			if(t.joinable())t.join();
		}
		//threads.clear();

	}
private:
	std::vector<std::thread> threads;
	std::queue<function<void()>>jobs;
	mutex job_mutex;
	condition_variable mutex_condition;
	bool stop_not=false;
};