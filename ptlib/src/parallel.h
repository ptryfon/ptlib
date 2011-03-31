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
	//std::unique_ptr<deferred_expression_base> ret_val;
	//evaluation_mgr::add_for_evaluation()
	auto def_exp = deferred_expression<Exp>(exp);
	auto def_val = deferred_value<typename deferred_expression<Exp>::type>(&def_exp);
	return def_val;
}

} } // namespace ptlib::parallel

#endif /* PARALLEL_H_ */
