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

class deferred_expression_base
{
public:
	virtual void evaluate() = 0;
	virtual ~deferred_expression_base();
};

template <typename Exp>
class deferred_expression : public deferred_expression_base
{
public:

	typedef typename proto::result_of::eval<Exp, proto::default_context>::type type;
	class deferred_value
	{
	public:
		type operator()();
		type get_value();
		bool is_ready() {return m_ready;}
		deferred_value(deferred_expression* exp) : m_ready(false), m_p_expression(exp) {}
		virtual ~deferred_value();
	protected:
		bool m_ready;
		type m_value;
		deferred_expression* m_p_expression;
	};

	virtual void evaluate();
	deferred_expression(const Exp & exp) : deferred_expression_base(), m_expression(exp) {}
	virtual ~deferred_expression();

private:
	const Exp  m_expression;
	type m_value;
};

template <typename Exp>
void
deferred_expression<Exp>::evaluate()
{
	proto::default_context ctx;
	m_value = proto::eval(m_expression, ctx);
	m_ready = true;
}

template <typename Exp>
typename deferred_expression<Exp>::type
deferred_expression<Exp>::operator()()
{
	if (!is_ready())
		evaluate();
	return m_value;
}

} }

#endif /* DEFERRED_VALUE_H_ */
