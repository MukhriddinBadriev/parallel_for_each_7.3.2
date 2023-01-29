#include <iostream>
#include <algorithm>
#include <chrono>
#include <exception>
#include <future>
#include <numeric>
#include <thread>
#include <vector>
#include <random>


//template< class InputIt, class UnaryFunction >
//UnaryFunction for_each(InputIt first, InputIt last, UnaryFunction f);

template<typename Iterator, typename T, class UnaryFunction>
T parallel_for_each(Iterator first, Iterator last, UnaryFunction f)
{
	auto length = std::distance(first, last);
	auto max_chunk_size = 25;
	if (length <= max_chunk_size)
	{
		return std::for_each(first, last, f);
	}
	else
	{
		Iterator mid_point = first;
		std::advance(mid_point, length / 2);
		std::future<T> first_half_result = std::async(std::launch::async, parallel_for_each<Iterator, T>, first, mid_point, f);
		std::future<T> second_half_result = std::async(std::launch::async, parallel_for_each<Iterator, T>, mid_point, last, f);
		
		return first_half_result.get() + second_half_result.get();
	}
}

int main()
{
	std::vector<int> V1(1000);

	generate(V1.begin(), V1.end(), []() {return rand() % 10; });

	

	auto R2 = parallel_for_each(V1.begin(), V1.end(), [](int i) {
		if (i % 2 == 1)std::cout << i * 3 << " ";
		else std::cout << i << " ";
	});

	
	
    return 0;
}