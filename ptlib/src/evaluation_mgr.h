/*
 * evaluation_mgr.h
 *
 *  Created on: Mar 20, 2011
 *      Author: pawel
 */

#ifndef EVALUATION_MGR_H_
#define EVALUATION_MGR_H_

#include "deferred.h"

#include <boost/thread.hpp>

#include <memory>
#include <queue>

namespace ptlib { namespace parallel {

class evaluation_mgr {

public:
	static void init();
	static void close();
	static void set_threads_num(unsigned num);
	template <typename T>
	static void add_for_evaluation(const parallel::deferred_expression_base& exp);

private:
	evaluation_mgr();
	virtual ~evaluation_mgr();
	void assign_tasks();
	void evaluation_loop();

private:
/** Threads number */

	static evaluation_mgr* m_instance;
	static unsigned threads_num;
/**
 * Synchronization mutex
 */
	boost::mutex m_task_queue_mutex;
	boost::condition_variable m_cond;

/**
 * Threads collection
 * m_threads.capacity() is a maximum number of threads
 * m_threads.size() is a current number of threads
 * Threads are created only when they are needed for evaluation.
 */
std::vector<std::unique_ptr<boost::thread> > m_threads;
unsigned				   m_working_threads;

/**
 * Tasks queue
 */
std::queue<std::unique_ptr<const parallel::deferred_expression_base> > m_tasks;

};

} }
#endif /* EVALUATION_MGR_H_ */
