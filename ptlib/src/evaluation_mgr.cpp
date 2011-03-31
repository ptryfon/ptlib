/*
 * evaluation_mgr.cpp
 *
 *  Created on: Mar 20, 2011
 *      Author: pawel
 */

#include "evaluation_mgr.h"

namespace ptlib { namespace parallel {

#ifndef PARALLEL_DEFAULT_THREADS_NUM
#define PARALLEL_DEFAULT_THREADS_NUM 10
#endif

unsigned evaluation_mgr::threads_num = PARALLEL_DEFAULT_THREADS_NUM;

void
evaluation_mgr::init()
{
	if (m_instance == NULL)
		m_instance = new evaluation_mgr();
}

void
evaluation_mgr::close()
{
	delete m_instance;
}

void
evaluation_mgr::set_threads_num(unsigned num)
{

}

evaluation_mgr::evaluation_mgr() :
		m_working_threads(0)
{
	m_threads.reserve(threads_num);
}

evaluation_mgr::~evaluation_mgr()
{}

void
evaluation_mgr::assign_tasks()
{
	// TODO Method stub
}

void evaluation_loop()
{
	// TODO Method stub
}

} }
