#include "stdafx.h"
#include "lbz_listen.h"
#include "lbz_http_client.h"
#include "lbz_timer.h"
#include "lbz_preferences.h"

using namespace foo_maloja;

lbz_listen::lbz_listen() {
	m_listened_at = -1;
}

bool lbz_listen::valid() {
	return (m_length > 0) &&
		(!m_artist_name.empty()) &&
		(!m_track_name.empty()) &&
		(!m_release_name.empty());
}

void lbz_listen::listen_now() {
	assert(m_listened_at < 0);
	if (m_listened_at < 0)
	{
		m_listened_at = lbz_timer::get_current_time();
	}
}

bool lbz_listen::submit() {
	/*json_t *j_root = json_object();
	json_t *j_payload = json_array();
	json_t *j_listen = json_object();
	json_t *j_meta = json_object();

	json_object_set_new(j_root, "listen_type", json_string("single"));
	json_object_set_new(j_root, "payload", j_payload);
	json_object_set_new(j_listen, "listened_at", json_integer(m_listened_at));
	json_object_set_new(j_meta, "artist_name", json_string(m_artist_name.c_str()));
	json_object_set_new(j_meta, "track_name", json_string(m_track_name.c_str()));
	json_object_set_new(j_meta, "release_name", json_string(m_release_name.c_str()));
	json_object_set_new(j_listen, "track_metadata", j_meta);
	json_array_append(j_payload, j_listen);*/
	json_t *j_listen = json_object();
	m_listen_length = lbz_timer::get_current_time() - m_listened_at;
	json_object_set_new(j_listen, "artist", json_string(m_artist_name.c_str()));
	json_object_set_new(j_listen, "title", json_string(m_track_name.c_str()));
	json_object_set_new(j_listen, "seconds", json_integer(m_listen_length));
	json_object_set_new(j_listen, "key", json_string(lbz_preferences::m_user_token.c_str()));

	char *json_data = json_dumps(j_listen, 0);
	json_decref(j_listen);

	pfc::string8 header = "Content-Type: application/json";

	lbz_http_client::post_url(lbz_preferences::m_server, "/apis/mlj_1/newscrobble", header, json_data, abort_callback_dummy());

	free(json_data);
	return true;
}
