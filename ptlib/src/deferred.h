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
	public deferred_expression_base  //TO DO
{

public:

	typedef typename proto::result_of::eval<Exp, proto::default_context>::type type;

	deferred_expression(const Exp & exp) : deferred_expression_base(), m_expression(exp) {}
	virtual ~deferred_expression();

	virtual void evaluate();

private:

	virtual void detach_value();

	const Exp  m_expression;
	typename deferred_value<type>::deferred_value_impl* m_p_value;

	template <typename T> friend class deferred_value<T>::deferred_value_impl;

};

template <typename T>
class deferred_value
{

public:

	class deferred_value_impl //TO DO add appropriate inheritance like noncopyable
	{

	public:

		deferred_value_impl(deferred_expression_base* exp) : m_ready(false), m_p_expression(exp) {}
		virtual ~deferred_value_impl();

		const T &	get_value() const;
		bool		is_ready() { return m_ready; }

	private:

		T 	 m_value;
		bool m_ready;
		deferred_expression_base* m_p_expression;

		template <typename Exp> friend class deferred_expression;

	};

	deferred_value(deferred_expression_base* exp) : m_p_implementation(new deferred_value_impl(exp)) {}
	deferred_value(deferred_value const & other) = delete;
	deferred_value & operator=(deferred_value const & other) = delete;
	deferred_value & operator=(deferred_value && other) { m_p_implementation = std::move(other.m_p_implementation); return *this; }
	deferred_value(deferred_value && other) { *this = std::move(other); }

	const T & 	get_value() const { return m_p_implementation->get_value(); }
				operator T() const { return get_value(); }
	bool		is_ready() {return m_p_implementation->is_ready(); }

private:

	std::unique_ptr<deferred_value_impl> m_p_implementation;

};

/* ************************************ */
/* DEFERRED EXPRESSION FUNCTIONS BODIES */
/* ************************************ */

template <typename Exp>
deferred_expression<Exp>::~deferred_expression()
{
}

template <typename Exp>
void
deferred_expression<Exp>::evaluate()
{
	// TO DO
	proto::default_context ctx;
	std::cout << "Evaluate function" << std::endl;
	if (m_p_value != NULL)
	{
		m_p_value->m_value = proto::eval(m_expression, ctx);
		std::cout << "Eval assigned value " << m_p_value->m_value << std::endl;
		m_p_value->m_ready = true;
	}
	else
		proto::eval(m_expression, ctx);
}

template <typename Exp>
void
deferred_expression<Exp>::detach_value()
{
	m_p_value = NULL;
}

/* ******************************* */
/* DEFERRED VALUE FUNCTIONS BODIES */
/* ******************************* */

template <typename T>
deferred_value<T>::deferred_value_impl::~deferred_value_impl()
{
	m_p_expression->detach_value();
}

template <typename T> inline
const T &
deferred_value<T>::deferred_value_impl::get_value() const
{
	std::cout << "In impl get_value" << std::endl;
	if (!m_ready)
		m_p_expression->evaluate();
	return m_value;
}

} }

#endif /* DEFERRED_VALUE_H_ */
