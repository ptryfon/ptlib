/*
 * evaluation_mgr.h
 *
 *  Created on: Mar 20, 2011
 *      Author: pawel
 */

#ifndef EVALUATION_MGR_H_
#define EVALUATION_MGR_H_

#include "deferred_expression.h"

#include <boost/thread.hpp>

#include <memory>
#include <queue>

namespace ptlib { namespace parallel {

class evaluation_mgr {

public:
	static void init(unsigned threads_num);
	static void close();
	static void add_for_evaluation(deferred_expression_base * const);

private:
	evaluation_mgr(unsigned threads_num_);
	virtual ~evaluation_mgr();
	static void evaluation_loop_wrapper() { m_p_instance->evaluation_loop(); }
	void evaluation_loop();
	bool if_stop_waiting() { return !(m_tasks.empty() && m_continue); }

private:

	static evaluation_mgr* m_p_instance;
/**
 * Synchronization mutex
 */
	boost::mutex m_tasks_mutex;
	boost::condition_variable m_threads_cond;

/**
 * Threads collection
 * m_threads.capacity() is a maximum number of threads
 * m_threads.size() is a current number of threads
 * Threads are created only when they are needed for evaluation.
 */
boost::thread_group m_threads;
//std::vector<boost::thread> m_threads;
unsigned				   m_working_threads;
bool					   m_continue;

/**
 * Tasks queue
 */
typedef std::unique_ptr<parallel::deferred_expression_base> task_ptr_type;
typedef std::queue<task_ptr_type> tasks_queue_type;
tasks_queue_type m_tasks;
std::queue<int>  m_foo;

};

} }

#endif /* EVALUATION_MGR_H_ */
