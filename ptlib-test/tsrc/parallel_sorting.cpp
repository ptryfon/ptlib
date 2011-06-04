//============================================================================
// Name        : parallel_sorting.cpp
// Author      : pt
// Version     : 1.0
// Description : Sorting test suite for Parallel library
//============================================================================

#define BOOST_TEST_NO_MAIN
#define BOOST_TEST_DYN_LINK

#include <parallel.h>

#include <boost/test/included/unit_test.hpp>

#include <boost/lambda/lambda.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>

#include <algorithm>

namespace parallel = ptlib::parallel;


struct SortingFixture {
    SortingFixture() : array(new int[array_size]) { parallel::init(3); reset(); BOOST_TEST_MESSAGE( "setup fixture" ); }
    ~SortingFixture() { parallel::close(); delete[] array; BOOST_TEST_MESSAGE( "teardown fixture" ); }
    void reset()
    {
    	srand(random_seed);
		for (size_t i = 0; i < array_size; i++)
		{
			array[i] = static_cast<int>( array_size * (static_cast<double>(rand())/RAND_MAX));
		}
    }

    static const size_t array_size = 200000000;
    int * array;
    static const int random_seed = 24;
};

template <typename InputIterator>
bool is_sorted(InputIterator begin, InputIterator end)
{
	InputIterator prev = begin;
	InputIterator curr = begin;
	if (curr != end)
		curr++;
	for ( ; curr++ != end; begin++)
	{
		if (*prev > *curr)
			return false;
	}
	return true;
}

BOOST_FIXTURE_TEST_SUITE(parallel_sorting, SortingFixture)

int partition(int array[], int l, int r)
{
	int s = (l+r)/2;
	int x = array[s];
	std::swap(array[s], array[r]);
	int store = l;
	for (int i = l; i < r; i++)
	{
		if (array[i] < x)
		{
			std::swap(array[i], array[store]);
			store++;
		}
	}
	std::swap(array[store], array[r]);
	return store;
}

void quicksort(int array[], int l, int r)
{
	if (l < r)
	{
		int s = partition(array, l, r);
		quicksort(array, l, s - 1);
		quicksort(array, s + 1, r);
	}

}

const int thread_limit = 75000;

void thread_quicksort(int array[], int l, int r)
{
	if (l < r)
	{
		int s = partition(array, l, r);
		boost::thread t;
		if (s - l > thread_limit)
			t = boost::thread(boost::bind(thread_quicksort, array, l, s - 1));
		else
			quicksort(array, l, s - 1);
		if (r - s > thread_limit)
			thread_quicksort(array, s + 1, r);
		else
			quicksort(array, s + 1, r);
		if (s - l > thread_limit)
			t.join();
	}
}

const int parallel_limit = 5000000;

void parallel_quicksort(int array[], int l, int r)
{
	if (l < r)
	{
		int s = partition(array, l, r);
		parallel::deferred_value<void> tmp;
		if (s - l > parallel_limit)
			tmp = parallel::eval(parallel::lazyf(parallel_quicksort, array, l, s - 1));
		else
			quicksort(array, l, s - 1);
		if (r - s > parallel_limit)
			parallel_quicksort(array, s + 1, r);
		else
		{
			quicksort(array, s + 1, r);
		}
		if (s - l > parallel_limit)
			tmp.force();

	}
}

//BOOST_AUTO_TEST_CASE( sequential_quicksort_test )
//{
//	boost::posix_time::ptime start_time, end_time;
//	start_time = boost::posix_time::microsec_clock::local_time();
//	quicksort(array, 0, array_size);
//	end_time = boost::posix_time::microsec_clock::local_time();
//	std::cout << "Wykonanie parallel zajęło " << boost::posix_time::time_period(start_time, end_time).length() << "." << std::endl;
//	BOOST_CHECK_PREDICATE(is_sorted<int*>, (array) (array + array_size));
//}
//
//BOOST_AUTO_TEST_CASE( parallel_quicksort_test )
//{
//	boost::posix_time::ptime start_time, end_time;
//	start_time = boost::posix_time::microsec_clock::local_time();
//	parallel_quicksort(array, 0, array_size);
//	end_time = boost::posix_time::microsec_clock::local_time();
//	std::cout << "Wykonanie parallel zajęło " << boost::posix_time::time_period(start_time, end_time).length() << "." << std::endl;
//	BOOST_CHECK_PREDICATE(is_sorted<int*>, (array) (array + array_size));
//}

//BOOST_AUTO_TEST_CASE ( thread_quicksort_test )
//{
//	boost::posix_time::ptime start_time, end_time;
//	start_time = boost::posix_time::microsec_clock::local_time();
//	thread_quicksort(array, 0, array_size);
//	end_time = boost::posix_time::microsec_clock::local_time();
//	std::cout << "Wykonanie thread zajęło " << boost::posix_time::time_period(start_time, end_time).length() << "." << std::endl;
//	BOOST_CHECK_PREDICATE(is_sorted<int*>, (array) (array + array_size));
//}

BOOST_AUTO_TEST_SUITE_END()

