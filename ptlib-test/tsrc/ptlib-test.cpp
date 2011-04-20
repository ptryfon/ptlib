//============================================================================
// Name        : ptlib-test.cpp
// Author      : pt
// Version     : 0.1
// Description : Initial test case set for ptlib
//============================================================================

#define BOOST_TEST_MODULE ParallelTest
#define BOOST_TEST_DYN_LINK

#include <parallel.h>

#include <boost/test/unit_test.hpp>

namespace parallel = ptlib::parallel;

using namespace std;
using namespace parallel;


struct CorrectnessFixture {
    CorrectnessFixture() { parallel::init(); }
    ~CorrectnessFixture() { parallel::close(); }
};

BOOST_FIXTURE_TEST_SUITE(correctness_tests, CorrectnessFixture)

BOOST_AUTO_TEST_CASE ( basic_exp_test )
{
	int a = 4;
	int b = 5;
	auto d = parallel::eval(val(a) + parallel::cref(b));
	auto f = parallel::eval(val(a) + val(b));
	auto e = 5 + d;
	BOOST_CHECK(static_cast<bool>(4 + 5 == d));
	//BOOST_CHECK(4 + 5 + 5 == e);
}

struct complex
{
	double r;
	double i;
	struct complex & operator =(const struct complex &);
	void transpose() { std::swap(r, i); }
	static void swap(struct complex& l, struct complex & r)
	{
		std::swap(l.r, r.r);
		std::swap(l.i, r.i);
	}
};

struct complex operator + (const struct complex & l, const struct complex & r)
{
	struct complex ret = {l.r + r.r, l.i + r.i};
	return ret;
}

bool operator == (struct complex l, struct complex r)
{
	return (l.r == r.r) && (l.i == r.i);
}

struct complex & complex::operator = (const struct complex & r)
{
	if (this != &r)
	{
		this->r = r.r;
		this->i = r.i;
	}
	return *this;
}
//
//BOOST_AUTO_TEST_CASE ( complex_exp_test )
//{
//	struct complex a = {2,3};
//	struct complex b = {3,4};
//	auto d = parallel::eval(parallel::ref(a) + *val(&b));
//	struct complex c = {5,7};
//	BOOST_CHECK(c == d);
//}
//
//BOOST_AUTO_TEST_CASE ( complex_assignment_test )
//{
//	struct complex a = {2,3};
//	struct complex b = {3,4};
//	struct complex c = b;
//	auto d = parallel::eval(parallel::ref(a) = b);
//	BOOST_CHECK(c == d);
//}

int power(int b, int e)
{
	unsigned ret = 1;
	for (;e > 0; e--)
	{
		ret *= b;
	}
	return ret;
}

//BOOST_AUTO_TEST_CASE ( basic_function_test)
//{
//	auto d = parallel::eval(lazyf(power, 10, 3));
//	BOOST_CHECK(1000 == d);
//}


//BOOST_AUTO_TEST_CASE( complex_swap_test )
//{
//	struct complex a = {2,3};
//	struct complex b = {3,4};
//	struct complex c = a;
//	cout << &a << endl;
//	auto d = parallel::eval(parallel::lazyf(&complex::swap, boost::ref(a), boost::ref(b)));
//	d.force();
//	BOOST_CHECK(c == b);
//}

//BOOST_AUTO_TEST_CASE( complex_member_function_test )
//{
//	struct complex a = {2,3};
//	struct complex b = {3,2};
//	auto d = parallel::eval(lazyf(&complex::transpose, &a));
//	d.force();
//	BOOST_CHECK(a == b);
//}

int throw_exception_number(int n)
{
	throw n;
	return n;
}

//BOOST_AUTO_TEST_CASE ( simple_exception_test )
//{
//	auto d = parallel::eval(lazyf(throw_exception_number, 5));
//	BOOST_CHECK_THROW(d.get_value(), std::exception);
//}

BOOST_AUTO_TEST_SUITE_END()



