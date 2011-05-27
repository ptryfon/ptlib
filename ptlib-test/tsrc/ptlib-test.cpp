//============================================================================
// Name        : ptlib-test.cpp
// Author      : pt
// Version     : 0.1
// Description : Initial test case set for ptlib
//============================================================================

#define BOOST_TEST_MODULE ParallelTest
#define BOOST_TEST_DYN_LINK

#include <ptlib/src/parallel.h>

#include <boost/test/unit_test.hpp>
#include <cmath>
#include <iostream>
namespace parallel = ptlib::parallel;

using namespace std;
using namespace parallel;


struct CorrectnessFixture {
    CorrectnessFixture() { parallel::init(); }
    ~CorrectnessFixture() { parallel::close(); }
};

BOOST_FIXTURE_TEST_SUITE(correctness_tests, CorrectnessFixture)

//BOOST_AUTO_TEST_CASE ( basic_exp_test )
//{
//	int a = 4;
//	int b = 5;
//	auto d = parallel::eval(val(a) + cref(b));
//	BOOST_CHECK(4 + 5 == d);
//}

struct complex
{
	double r;
	double i;
	struct complex & operator =(const struct complex &);
	void swap(struct complex & r);
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

//BOOST_AUTO_TEST_CASE ( complex_exp_test )
//{
//	struct complex a = {2,3};
//	struct complex b = {3,4};
//	auto d = parallel::eval(parallel::ref(a) + *val(&b));
//	struct complex c = {5,7};
//	BOOST_CHECK(c == d);
//}

BOOST_AUTO_TEST_CASE ( complex_assignment_test )
{
	struct complex a = {2,3};
	struct complex b = {3,4};
	struct complex c = b;
	auto d = parallel::eval(parallel::ref(a) = b);
	BOOST_CHECK(c == d);
}

//class Foo
//{
//public:
//	void foo() { cout << "Fo" << endl; return;}
//};
//
//BOOST_AUTO_TEST_CASE( member_function_test )
//{
//	Foo f;
//	auto d = parallel::eval(lazym(&Foo::foo, f));
//	d.force();
//}

BOOST_AUTO_TEST_SUITE_END()



