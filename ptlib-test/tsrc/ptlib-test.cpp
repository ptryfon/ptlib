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

proto::terminal< std::ostream & >::type cout_ = {std::cout};
template< typename Expr >
inline void evaluate (Expr const & expr)
{
	proto::default_context ctx;
	std::cout << "To w srodku eval" << std::endl;
	std::cout << "Result: " << proto::eval(expr, ctx);
}

struct complicated
{
	int a;
	int b;
	friend std::ostream& operator<< (std::ostream&, const struct complicated);
};

std::ostream& operator<<(std::ostream& outs, const struct complicated com)
{
	outs << "Complicated {a: " << com.a << ",b: " << com.b << "}";
	return outs;
}

int foo() {std::cout<< "foo" << std::endl; return 41;}
int foo2(int i) {std::cout<< "foo" << std::endl; return 2*i+3;}
BOOST_AUTO_TEST_CASE(lazy_fun_test)
{
	complicated c = {1, 2};
	proto::display_expr(parallel::val(c));
	evaluate(parallel::lazify(foo2, 5));
	auto val2 = parallel::evaluate(parallel::val(4) + parallel::val(5));
	std::cout << "Wartosc " << val2() << std::endl;
}

/* parallel::ref works properly, but deferred_expression::type member should be change to support returning references
BOOST_AUTO_TEST_CASE(parallel_ref_test)
{
	//auto val1 = parallel::evaluate(parallel::ref(std::cout) << "Hello world!");
} */


