/*
 * parallel.h
 *
 *  Created on: Mar 20, 2011
 *      Author: pawel
 */

#ifndef PARALLEL_H_
#define PARALLEL_H_

#include "lazy.h"
#include "deferred.h"
#include "evaluation_mgr.h"

namespace ptlib { namespace parallel {

namespace proto = boost::proto;

template <typename Exp>
deferred_value<typename deferred_expression<Exp>::result_type>
eval(const Exp && exp)
{
	auto const p_def_exp = new deferred_expression<Exp>(exp);
	auto def_val = deferred_value<typename deferred_expression<Exp>::result_type>();
	p_def_exp->connect_value(def_val);
	evaluation_mgr::add_for_evaluation(p_def_exp);
	return std::move(def_val);
}

#ifndef PARALLEL_DEFAULT_THREADS_NUM
#define PARALLEL_DEFAULT_THREADS_NUM 10
#endif

inline
void
init(unsigned thread_num = PARALLEL_DEFAULT_THREADS_NUM)
{
	evaluation_mgr::init(thread_num);
}

inline
void
close()
{
	evaluation_mgr::close();
}

} } // namespace ptlib::parallel

#endif /* PARALLEL_H_ */
