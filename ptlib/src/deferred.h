/*
 * deferred_value.h
 *
 *  Created on: Mar 20, 2011
 *      Author: pawel
 */

#ifndef DEFERRED_VALUE_H_
#define DEFERRED_VALUE_H_

#include <boost/proto/proto.hpp>

namespace ptlib { namespace parallel {

namespace proto = boost::proto;

template <typename T> class deferred_value;

class deferred_expression_base
{
public:
	virtual ~deferred_expression_base() {}
	virtual void evaluate() = 0;
	virtual void detach_value() = 0;
};

template <typename Exp>
class deferred_expression :
	public deferred_expression_base
{

public:

	typedef typename proto::result_of::eval<Exp, proto::default_context>::type type;

	deferred_expression(const Exp & exp) : deferred_expression_base(), m_expression(exp) {}
	virtual ~deferred_expression();

	virtual void evaluate();

private:

	virtual void detach_value();

	const Exp  m_expression;
	deferred_value<type>* m_p_value;

	template <typename T> friend class deferred_value;

};

template <typename T>
class deferred_value
{
public:

	deferred_value(deferred_expression_base* exp) : m_ready(false), m_p_expression(exp) {}
	virtual ~deferred_value();

	const T & 	get_value() const;
	T & 		get_value();
				operator T() const { return get_value(); }
	bool		is_ready() { return m_ready; }

private:

	void detach_expression();

	T 	 m_value;
	bool m_ready;
	deferred_expression_base* m_p_expression;

	template <typename Exp> friend class deferred_expression;
};

/* ************************************ */
/* DEFERRED EXPRESSION FUNCTIONS BODIES */
/* ************************************ */

template <typename Exp>
deferred_expression<Exp>::~deferred_expression()
{
	m_p_value->detach_expression();
}

template <typename Exp>
void
deferred_expression<Exp>::evaluate()
{
	proto::default_context ctx;
	m_p_value->m_value = proto::eval(m_expression, ctx);
	throw "TO DO";
}

template <typename Exp>
void
deferred_expression<Exp>::detach_value()
{
	m_p_value = NULL;
}

template <typename T>
deferred_value<T>::~deferred_value()
{
	m_p_expression->detach_value();
}

template <typename T> inline
const T &
deferred_value<T>::get_value() const
{
	throw "TO DO";
	return m_value;
}

template <typename T> inline
T &
deferred_value<T>::get_value()
{
	throw "TO DO";
	return m_value;
}

template <typename T>
void
deferred_value<T>::detach_expression()
{
	m_p_expression = NULL;
}

} }

#endif /* DEFERRED_VALUE_H_ */
