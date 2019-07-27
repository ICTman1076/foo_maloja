#pragma once

#include <ctime>

namespace foo_listenbrainz {
	class lbz_timer
	{
	private:
		bool m_is_running;
		unsigned long m_start_time;
		unsigned long m_elapsed_time;

	public:
		lbz_timer();

		void reset();
		void start();
		void stop();
		void restart();

		bool is_running();
		unsigned long get_elapsed_time();

		static unsigned long get_current_time();
	};
}
