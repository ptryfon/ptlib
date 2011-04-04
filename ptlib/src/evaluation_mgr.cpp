/*
 * evaluation_mgr.cpp
 *
 *  Created on: Mar 20, 2011
 *      Author: pawel
 */

#include "evaluation_mgr.h"

namespace ptlib { namespace parallel {

evaluation_mgr * evaluation_mgr::m_p_instance = NULL;

void
evaluation_mgr::init(unsigned threads_num_)
{
	if (m_p_instance == NULL)
		m_p_instance = new evaluation_mgr(threads_num_);
}

void
evaluation_mgr::close()
{
	delete m_p_instance;
}

void
evaluation_mgr::add_for_evaluation(deferred_expression_base const * const p_def_exp_)
{
	boost::lock_guard<boost::mutex> lock(m_p_instance->m_tasks_mutex);
	m_p_instance->m_tasks.push(std::unique_ptr<deferred_expression_base const>(p_def_exp_));
	m_p_instance->m_threads_cond.notify_one();
}

evaluation_mgr::evaluation_mgr(unsigned threads_num_) :
		m_working_threads(0)
{
	m_threads.reserve(threads_num_);
	for (unsigned i = 0; i < threads_num_; i++)
		m_threads[i] = boost::thread(&evaluation_mgr::evaluation_loop);
}

evaluation_mgr::~evaluation_mgr()
{}

void
evaluation_mgr::evaluation_loop()
{
	// TODO Method stub
	std::cout << "Eval loop" << std::endl;
}

} }
