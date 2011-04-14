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

template< typename Expr >
inline
typename proto::result_of::eval<Expr, typename proto::default_context>::type
evaluate (Expr const & expr)
{
	proto::default_context ctx;
	std::cout << "To w srodku eval" << std::endl;
	auto ret_val = proto::eval(expr, ctx);
	std::cout << "Result: " << ret_val << std::endl;
	return ret_val;
}

int foo() {std::cout<< "foo" << std::endl; return 41;}
int foo2(int i) {std::cout<< "foo" << std::endl; return 2*i+3;}
void foo_void() {return;}
BOOST_AUTO_TEST_CASE(lazy_fun_test)
{
	parallel::evaluation_mgr::init(3);
	evaluate(parallel::lazify(foo2, 5));
	auto val2 = parallel::evaluate(parallel::make_lazy_function(foo)());
	std::cout << "Wartosc " << val2 << std::endl;
	parallel::evaluation_mgr::close();
	std::cout << "THE END" << std::endl;
}

/* parallel::ref works properly, but deferred_expression::type member should be change to support returning references
BOOST_AUTO_TEST_CASE(parallel_ref_test)
{
	//auto val1 = parallel::evaluate(parallel::ref(std::cout) << "Hello world!");
} */


