#pragma once

#include <string>
#include "jansson.h"

namespace foo_maloja {
	class lbz_listen
	{
	public:
		int64_t m_length;
		int64_t m_listened_at;
		int64_t m_listen_length;
		std::string m_artist_name;
		std::string m_track_name;
		std::string m_release_name;

	public:
		lbz_listen();
		bool valid();
		void listen_now();
		bool submit();
	};
}
