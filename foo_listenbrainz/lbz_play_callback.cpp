#include "stdafx.h"
#include <algorithm>
#include "lbz_listen.h"
#include "lbz_timer.h"

namespace foo_listenbrainz {
	class lbz_play_callback : public play_callback_static {
	private:
		lbz_listen *m_listen;
		lbz_timer *m_timer = new lbz_timer();

		static void try_submit_listen(lbz_listen *listen, unsigned long listened_time)
		{
			if (listen != NULL)
			{
				unsigned long required_time = min(listen->m_length / 2, 4 * 60);
				if (listened_time >= required_time)
				{
					listen->listen_now();
					listen->submit();
				}
			}
		}

		virtual void on_playback_new_track(metadb_handle_ptr p_track) {
			try_submit_listen(m_listen, m_timer->get_elapsed_time());
			m_listen = NULL;

			file_info_impl info;
			if (p_track->get_info(info))
			{
				lbz_listen *listen = new lbz_listen();
				listen->m_length = info.get_length();
				listen->m_artist_name = info.meta_get("ARTIST", 0);
				listen->m_track_name = info.meta_get("TITLE", 0);
				listen->m_release_name = info.meta_get("ALBUM", 0);
				m_listen = listen;
			}
			m_timer->restart();
		}

		virtual void on_playback_pause(bool p_state) {
			if (p_state)
			{
				m_timer->stop();
			}
			else
			{
				m_timer->start();
			}
		}

		virtual void on_playback_stop(play_control::t_stop_reason p_reason) {
			m_timer->stop();
			try_submit_listen(m_listen, m_timer->get_elapsed_time());
			m_listen = NULL;
		}

		virtual void on_playback_starting(play_control::t_track_command p_command, bool p_paused) { }
		virtual void on_playback_seek(double p_time) {}
		virtual void on_playback_edited(metadb_handle_ptr p_track) {}
		virtual void on_playback_dynamic_info(const file_info & p_info) {}
		virtual void on_playback_dynamic_info_track(const file_info & p_info) {}
		virtual void on_playback_time(double p_time) {}
		virtual void on_volume_change(float p_new_val) {}

		unsigned int get_flags() {
			return flag_on_playback_new_track | flag_on_playback_pause | flag_on_playback_stop;
		}
	};

	static play_callback_static_factory_t<lbz_play_callback> g_play_playback;
}
