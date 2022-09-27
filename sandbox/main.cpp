#include <iostream>
#include <thread>
#include <vector>

// simple test on a parallel loop
static const int TOTAL_LOOP = 1000;

void loop (int *extern_count) {
	using namespace std::literals::chrono_literals;
	(*extern_count) += 1;
	std::cout << "THID : " << std::this_thread::get_id() << " added 1" << std::endl;
	std::this_thread::sleep_for(1s);
}

int main () {
	int total = 0;

	// std::thread worker1(loop, &total);
	// std::thread worker2(loop, &total);
	// worker1.join();
	// worker2.join();
	std::vector<std::thread> list_workers;
	for (int i = 0; i < 10; i++) {
		std::thread worker(loop, &total);
		list_workers.push_back (std::move(worker));
	}

	for (auto & worker : list_workers)
		worker.join();
	
	std::cout << total;
	return 0;
}