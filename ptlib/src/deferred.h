/*
 * deferred_value.h
 *
 *  Created on: Mar 20, 2011
 *      Author: pawel
 */

#ifndef DEFERRED_VALUE_H_
#define DEFERRED_VALUE_H_

#include <boost/proto/proto.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/bind.hpp>

namespace ptlib { namespace parallel {

namespace proto = boost::proto;

template <typename T> class deferred_value;

class deferred_expression_base
{

public:
	deferred_expression_base() :
		m_being_evaluated(false), m_was_evaluated(false), m_some_waiting_for_eval(false) {}
	virtual ~deferred_expression_base() {}
	virtual void evaluate() = 0;
	virtual void try_evaluate() = 0;
	virtual void detach_value() = 0;
	bool if_was_evaluated() { return m_was_evaluated; }

protected:

	bool	  m_being_evaluated;
	bool 	  m_was_evaluated;
	bool	  m_some_waiting_for_eval;
	static boost::mutex m_evaluation_mutex;
	static boost::condition_variable m_evaluation_cond;

};

template <typename Exp>
class deferred_expression :
	public deferred_expression_base  //TO DO
{

public:

	typedef typename proto::result_of::eval<Exp, proto::default_context>::type result_type;

	deferred_expression(const Exp & exp_) :
		deferred_expression_base(), m_expression(exp_),
		m_p_value(NULL) {}
	virtual ~deferred_expression();

	void connect_value(const deferred_value<result_type> & def_val_);
	virtual void evaluate();
	virtual void try_evaluate();

private:

	virtual void detach_value();
	void _evaluate_impl();

	const Exp m_expression;
		typename deferred_value<result_type>::deferred_value_impl* m_p_value;

	friend class deferred_value<result_type>::deferred_value_impl;

};

template <typename T>
class deferred_value
{

public:

	class deferred_value_impl //TO DO add appropriate inheritance like noncopyable
	{

	public:

		deferred_value_impl() : m_ready(false), m_p_expression(NULL) {}
		virtual ~deferred_value_impl();

		const T &	get_value() const;
		bool		is_ready() { return m_ready; }

	private:

		void detach_expression();

		T 	 m_value;
		bool m_ready;
		deferred_expression_base* m_p_expression;

		template <typename Exp> friend class deferred_expression;

	};

	deferred_value() : m_p_implementation(new deferred_value_impl()) {}
	deferred_value(deferred_value const & other) = delete;
	//TO DO trzeba zmienić i dać możliwość kopiowania, bo nie ma powodu, żeby tego zabronić.
	deferred_value & operator=(deferred_value const & other) = delete;
	deferred_value & operator=(deferred_value && other) { m_p_implementation = std::move(other.m_p_implementation); return *this; }
	deferred_value(deferred_value && other) { *this = std::move(other); }

	const T & 	get_value() const { return m_p_implementation->get_value(); }
				operator T() const { return get_value(); }
	bool		is_ready() {return m_p_implementation->is_ready(); }

private:

	std::unique_ptr<deferred_value_impl> m_p_implementation;

	template <typename Exp> friend class deferred_expression;

};

/* ************************************ */
/* DEFERRED EXPRESSION FUNCTIONS BODIES */
/* ************************************ */

template <typename Exp>
deferred_expression<Exp>::~deferred_expression()
{
	std::cout << "Def exp destor called" << std::endl;
	if (m_p_value != NULL)
		m_p_value->detach_expression();
}

template <typename Exp>
void
deferred_expression<Exp>::connect_value(const deferred_value<deferred_expression<Exp>::result_type> & def_val_)
{
	m_p_value = def_val_.m_p_implementation.get();
	m_p_value->m_p_expression = this;
}

template <typename Exp>
void
deferred_expression<Exp>::evaluate()
{
	// TO DO
	bool do_evaluate = true;
	{
		boost::mutex::scoped_lock lock(m_evaluation_mutex);
		if (m_being_evaluated)
		{
			m_some_waiting_for_eval = true;
			m_evaluation_cond.wait(lock, boost::bind(&deferred_expression_base::if_was_evaluated, this));
			do_evaluate = false;
		}
		else
		{
			m_being_evaluated = true;
		}
	}
	if (do_evaluate)
	{
		_evaluate_impl();
	}
}

template <typename Exp>
void
deferred_expression<Exp>::try_evaluate()
{
	bool do_evaluate = true;
	{
		boost::mutex::scoped_lock lock(m_evaluation_mutex);
		if (m_being_evaluated)
		{
			do_evaluate = false;
		}
		else
		{
			m_being_evaluated = true;
		}
	}
	if (do_evaluate)
	{
		_evaluate_impl();
	}
}

template <typename Exp>
void
deferred_expression<Exp>::_evaluate_impl()
{
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
	m_was_evaluated = true;
	if (m_some_waiting_for_eval)
	{
		m_evaluation_cond.notify_all();
	}
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
	std::cout << "Def val impl destor called" << std::endl;
	if (m_p_expression != NULL)
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

template <typename T>
void
deferred_value<T>::deferred_value_impl::detach_expression()
{
	m_p_expression = NULL;
}

} }

#endif /* DEFERRED_VALUE_H_ */
