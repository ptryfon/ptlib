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

template <typename T>
inline typename proto::result_of::make_expr<
	proto::tag::terminal,
	T >::type const
val(T arg)
{
	return proto::deep_copy(proto::make_expr<proto::tag::terminal>(arg));
}

template <typename T>
inline typename proto::result_of::make_expr<
	proto::tag::terminal,
	T const>::type const
cval(T const arg)
{
	return proto::deep_copy(proto::make_expr<proto::tag::terminal>(arg));
}

template <typename T>
inline typename proto::result_of::make_expr<
	proto::tag::terminal,
	boost::reference_wrapper<T> >::type const
ref(T & arg)
{
	return proto::make_expr<proto::tag::terminal>(boost::reference_wrapper<T>(arg));
}

template <typename T>
inline typename proto::result_of::make_expr<
	proto::tag::terminal,
	const T &>::type const
cref(const T & arg)
{
	return proto::make_expr<proto::tag::terminal>(boost::cref(arg));
}

template <typename T>
inline typename proto::result_of::make_expr<
	proto::tag::terminal,
	T>::type const
rval(T && arg)
{
	return proto::make_expr<proto::tag::terminal>(arg);
}

template <typename T>
inline typename proto::result_of::make_expr<
	proto::tag::function,
	T>::type const
lazyf(T fun) {return proto::make_expr<proto::tag::function>(fun);}

template <typename T, typename U>
inline typename proto::result_of::make_expr<
	proto::tag::function,
	T,
	U>::type const
lazyf(T fun, U arg1) { return proto::make_expr<proto::tag::function>(fun, arg1); }


template <typename T, typename Arg1, typename Arg2>
inline typename proto::result_of::make_expr<
	proto::tag::function,
	T,
	Arg1,
	Arg2>::type const
lazyf(T fun, Arg1 arg1, Arg2 arg2) { return proto::make_expr<proto::tag::function>(fun, arg1, arg2); }

template <typename T, typename Arg1, typename Arg2, typename Arg3>
inline typename proto::result_of::make_expr<
	proto::tag::function,
	T,
	Arg1,
	Arg2,
	Arg3>::type const
lazyf(T fun, Arg1 arg1, Arg2 arg2, Arg3 arg3) { return proto::make_expr<proto::tag::function>(fun, arg1, arg2, arg3); }

/*template <typename T, typename Arg, typename...RArgs>
inline typename proto::result_of::make_expr<
	proto::tag::function,
	T,
	Arg,
	RArgs...>::type const
	lazifyx(T fun, Arg a, Rargs ... args) {return proto::make_expr<proto::tag::function>(fun, a, args...); }
	}
	*/
/* Should work according to standard, but it doesn't
template <typename T, class...A>
inline typename proto::result_of::make_expr<
	proto::tag::function,
	T,
	A ...>::type const
	lazifyx(T fun,Arg a, A...args) { return proto::make_expr<proto::tag::function>(fun, args...); }


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

	*/

} }


#endif /* LAZY_H_ */
