/*
 * parallel.h
 *
 *  Created on: Mar 20, 2011
 *      Author: pawel
 */

#ifndef PARALLEL_H_
#define PARALLEL_H_

#include <boost/proto/proto.hpp>

#include "lazy.h"
#include "deferred.h"
#include "evaluation_mgr.h"

namespace ptlib { namespace parallel {

namespace proto = boost::proto;

template <typename Exp>
deferred_value<typename deferred_expression<Exp>::type> evaluate(const Exp && exp)
{
	//TO DO Zmienić kolejność przypisywania wskaźników, bo teraz jest gorzej
	/* Można zrobić tak, że wskaźnik się przypisze gdy wyrażenie nie jest void-owe,
	 * Może to się da przy pomocy MPL zrobić
	 */
	auto const p_def_exp = new deferred_expression<Exp>(exp);
	auto def_val = deferred_value<typename deferred_expression<Exp>::type>(p_def_exp);
	evaluation_mgr::add_for_evaluation(p_def_exp);
	return std::move(def_val);
}

} } // namespace ptlib::parallel

#endif /* PARALLEL_H_ */
