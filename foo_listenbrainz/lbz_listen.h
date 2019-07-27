#pragma once

#include <string>
#include "jansson.h"

namespace foo_listenbrainz {
	class lbz_listen
	{
	public:
		int64_t m_length;
		int64_t m_listened_at;
		std::string m_artist_name;
		std::string m_track_name;
		std::string m_release_name;

	public:
		lbz_listen();
		void listen_now();
		bool submit();
	private:
		json_t *json_encode();
	};
}
