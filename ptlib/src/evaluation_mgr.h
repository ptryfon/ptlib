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

#ifndef PARALLEL_DEFAULT_THREADS_NUM
#define PARALLEL_DEFAULT_THREADS_NUM 10
#endif

class evaluation_mgr {

public:
	static void init(unsigned threads_num = PARALLEL_DEFAULT_THREADS_NUM);
	static void close();
	static void add_for_evaluation(deferred_expression_base const * const);

private:
	evaluation_mgr(unsigned threads_num_);
	virtual ~evaluation_mgr();
	void evaluation_loop();

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
std::vector<std::unique_ptr<boost::thread> > m_threads;
unsigned				   m_working_threads;

/**
 * Tasks queue
 */
std::queue<std::unique_ptr<parallel::deferred_expression_base const> > m_tasks;
std::queue<int> m_foo;

};

} }

#endif /* EVALUATION_MGR_H_ */
