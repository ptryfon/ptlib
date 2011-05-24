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
namespace proto = boost::proto;
namespace parallel = ptlib::parallel;

using namespace std;
using namespace parallel;

class Foo
{
public:
	void foo() { cout << "Fo" << endl; return;}
};

BOOST_AUTO_TEST_CASE( foo_oo )
{
	Foo f;
	//parallel::eval(lazyf(&Foo::foo, f));
	parallel::eval(lazym(&Foo::foo, f));
	//parallel::eval(parallel::lazyf(&Foo::foo, f));
}



