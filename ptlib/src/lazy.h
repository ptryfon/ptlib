/*
 * lazy.h
 *
 *  Created on: Mar 24, 2011
 *      Author: pawel
 */

#ifndef LAZY_H_
#define LAZY_H_

#include <boost/proto/proto.hpp>

namespace ptlib { namespace parallel
{

namespace proto = boost::proto;

/** lit - function
 * @param arg - expression that should be turned into literal
 */
/*template <typename T>
inline typename proto::literal<T> value(T arg)
{
	return proto::lit(arg);
}*/

/* Maybe I have to use terminal notation and make expression do achieve here what I want */
template <typename T>
inline typename proto::result_of::make_expr<
	proto::tag::terminal,
	T & >::type const
ref(T & arg)
{
	return proto::make_expr<proto::tag::terminal>(boost::ref(arg));
}

template <typename T>
inline typename proto::result_of::make_expr<
	proto::tag::terminal,
	T >::type const
val(T arg)
{
	return proto::make_expr<proto::tag::terminal>(arg);
}

template <typename T>
class lazy_function
{
public:
	lazy_function(T fun) : m_fun(fun){}
private:
	T m_fun;
public:
	inline typename proto::result_of::make_expr<
		proto::tag::function,
		T>::type const
	operator()() {return proto::make_expr<proto::tag::function>(m_fun);}
	template <typename U>
	inline typename proto::result_of::make_expr<
		proto::tag::function,
		T,
		U>::type const
	operator()(U arg1) {return proto::make_expr<proto::tag::function>(m_fun, arg1);}
};

template <typename T>
lazy_function<T>
make_lazy_function(T fun) {return lazy_function<T>(fun);}

template <typename T>
inline typename proto::result_of::make_expr<
	proto::tag::function,
	T>::type const
	lazify(T fun) {return proto::make_expr<proto::tag::function>(fun);}

template <typename T, typename U>
inline typename proto::result_of::make_expr<
	proto::tag::function,
	T,
	U>::type const
	lazify(T fun, U arg1) { return proto::make_expr<proto::tag::function>(fun, arg1); }


template <typename T, typename Arg1, typename Arg2>
inline typename proto::result_of::make_expr<
	proto::tag::function,
	T,
	Arg1,
	Arg2>::type const
	lazify(T fun, Arg1 arg1, Arg2 arg2) { return proto::make_expr<proto::tag::function>(fun, arg1, arg2); }


/* Should work according to standard, but it doesn't
template <typename T, class...A>
inline typename proto::result_of::make_expr<
	proto::tag::function,
	T,
	A>::type const
	lazifyx(T fun,A...args) { return proto::make_expr<proto::tag::function>(fun, args); }
*/

/*
template <typename T>
inline typename proto::result_of::make_expr<
	proto::tag::terminal,
	T>::type const
literal(T arg)
{
	return proto::make_expr<proto::tag::terminal>(arg);
}*/

} }


#endif /* LAZY_H_ */
