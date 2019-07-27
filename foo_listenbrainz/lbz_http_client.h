#pragma once

#include <string>
#include <map>
#include "pfc.h"
#include "SDK/foobar2000.h"

namespace foo_listenbrainz {
	class lbz_http_client
	{
	public:
		static void post_url(const char *host, const char *object,
			const char *header, const char *data, abort_callback &callback);
	};
}
