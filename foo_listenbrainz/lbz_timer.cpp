#include "stdafx.h"
#include "lbz_timer.h"
#include <ctime>

using namespace foo_listenbrainz;

lbz_timer::lbz_timer() {
	reset();
}

void lbz_timer::reset() {
	stop();
	m_elapsed_time = 0;
}

void lbz_timer::start() {
	m_start_time = get_current_time();
	m_is_running = true;
}

void lbz_timer::stop() {
	if (m_is_running)
	{
		m_is_running = false;
		m_elapsed_time += get_current_time() - m_start_time;
	}
}

bool lbz_timer::is_running() {
	return m_is_running;
}

void lbz_timer::restart() {
	reset();
	start();
}

unsigned long lbz_timer::get_elapsed_time() {
	unsigned long elapsed_time = m_elapsed_time;
	if (m_is_running)
	{
		elapsed_time += get_current_time() - m_start_time;
	}
	return elapsed_time;
}

unsigned long lbz_timer::get_current_time() {
	return time(NULL);
}
