/*
 * evaluation_mgr.cpp
 *
 *  Created on: Mar 20, 2011
 *      Author: pawel
 */

#include "evaluation_mgr.h"

#include <boost/bind.hpp>

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
	//std::cout << "End of eval_mgr close" << std::endl;
}

void
evaluation_mgr::add_for_evaluation(deferred_expression_base * const p_def_exp_)
{
	{// CRITICAL_SECTION
		boost::lock_guard<boost::mutex> lock(m_p_instance->m_tasks_mutex);
		m_p_instance->m_tasks.push(std::unique_ptr<deferred_expression_base>(p_def_exp_));
	}
	m_p_instance->m_threads_cond.notify_one();
}

evaluation_mgr::evaluation_mgr(unsigned threads_num_) :
		m_tasks_mutex(),
		m_working_threads(0),
		m_continue(true)
{
	for (unsigned i = 0; i < threads_num_; i++)
		m_threads.create_thread(boost::bind(&evaluation_mgr::evaluation_loop, this));
}

evaluation_mgr::~evaluation_mgr()
{
	m_continue = false;
	m_threads_cond.notify_all();
	m_threads.join_all();
	//std::cout << "Eval_mgr dest finished" << std::endl;
}

void
evaluation_mgr::evaluation_loop()
{
	//std::cout << "Entering evaluation loop" << std::endl;
	while(m_continue)
	{
		task_ptr_type p_task;
		{// CRITICAL_SECTION
			//std::cout << "Before mutex" << std::endl;
			boost::mutex::scoped_lock lock(m_tasks_mutex);
			//std::cout << "Loop critical section" << std::endl;
			if (m_foo.empty())
			{
				//std::cout << "No tasks" << std::endl;
				m_threads_cond.wait(lock, boost::bind(&evaluation_mgr::if_stop_waiting, this));
			}
			if (!m_continue)
			{
				//std::cout << "Stop execution" << std::endl;
				break;
			}
			else // m_tasks is not empty
			{
				//std::cout << "Some task" << std::endl;
				p_task = std::move(m_tasks.front());
				m_tasks.pop();
			}
		}
		//std::cout << "Evaluation from eval loop" << std::endl;
		p_task->evaluate();
	}
}

} }
