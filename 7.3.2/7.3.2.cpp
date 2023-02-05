#include <iostream>
#include <algorithm>
#include <chrono>
#include <exception>
#include <future>
#include <numeric>
#include <thread>
#include <vector>
#include <random>

template<typename Iterator, class UnaryFunction>
void parallel_for_each(Iterator first, Iterator last, UnaryFunction f)
{
	auto length = std::distance(first, last);
	auto max_chunk_size = 25;
	if (length <= max_chunk_size)
	{
		std::for_each(first, last, f);
	}
	else
	{
		Iterator mid_point = first;
		std::advance(mid_point, length / 2);
		std::future<void> first_half_result = std::async(std::launch::async, parallel_for_each<Iterator, UnaryFunction >, first, mid_point, f);		
		parallel_for_each(mid_point, last, f);
		first_half_result.get();
	}
}


int main()
{
	std::vector<int> V(60);
	generate(V.begin(), V.end(), []() {return rand() % 10; });
	std::cout << "Vector\n";
	for(int j = 0; j < V.size(); j++) {
		std::cout << V[j] << " ";
	}std::cout << "\n\n\n";

	std::cout << "parallel for_each:\n";
	parallel_for_each(V.begin(), V.end(), [](int i) {
		if (i % 2 == 1)std::cout << i * 3 << " ";
		else std::cout << i << " ";
	}); std::cout << "\n\n\n";
	
    return 0;
}