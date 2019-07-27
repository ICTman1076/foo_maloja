#include "stdafx.h"
#include "resource.h"
#include "lbz_preferences.h"
#include "foo_listenbrainz.h"
#include <helpers/atl-misc.h>

namespace foo_listenbrainz {
	namespace lbz_preferences {
		const char *m_server_url = "api.listenbrainz.org";

		// {7D84EE59-4EE8-448C-B5D4-2D3EFA3CC1D0}
		static const GUID guid_listen_enable =
		{ 0x7d84ee59, 0x4ee8, 0x448c,{ 0xb5, 0xd4, 0x2d, 0x3e, 0xfa, 0x3c, 0xc1, 0xd0 } };
		const bool default_listen_enable = false;
		cfg_bool m_listen_enable(guid_listen_enable, default_listen_enable);

		// {2A0E7DA1-C66F-44C2-B9E7-A4E9D9EE5C2F}
		static const GUID guid_user_name =
		{ 0x2a0e7da1, 0xc66f, 0x44c2,{ 0xb9, 0xe7, 0xa4, 0xe9, 0xd9, 0xee, 0x5c, 0x2f } };
		const std::string default_user_name = "";
		cfg_string m_user_name(guid_user_name, "");

		// {4FDB9143-BBDC-4C79-BB02-91DFE19AB29F}
		static const GUID guid_user_token =
		{ 0x4fdb9143, 0xbbdc, 0x4c79,{ 0xbb, 0x2, 0x91, 0xdf, 0xe1, 0x9a, 0xb2, 0x9f } };
		cfg_string m_user_token(guid_user_token, "");
	}

	class lbz_preferences_page_instance : public CDialogImpl<lbz_preferences_page_instance>, public preferences_page_instance {
	private:
		CButton listen_enable_checkbox;
		CEdit user_name_textbox;
		CEdit user_token_textbox;
		preferences_page_callback::ptr on_change_callback;

	public:
		lbz_preferences_page_instance(preferences_page_callback::ptr callback) : on_change_callback(callback) {}

		enum { IDD = IDD_PREFERENCES };

		BEGIN_MSG_MAP(CPreferencesDialog)
			MSG_WM_INITDIALOG(OnInitDialog)
			COMMAND_HANDLER_EX(IDC_LISTEN_ENABLE, BN_CLICKED, OnChanged)
		END_MSG_MAP()

		BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam) {
			listen_enable_checkbox = GetDlgItem(IDC_LISTEN_ENABLE);
			user_name_textbox = GetDlgItem(IDC_USER_NAME);
			user_token_textbox = GetDlgItem(IDC_USER_TOKEN);

			listen_enable_checkbox.SetCheck(lbz_preferences::m_listen_enable.get_value());
			uSetWindowText(user_name_textbox, lbz_preferences::m_user_name);
			uSetWindowText(user_token_textbox, lbz_preferences::m_user_token);

			return 0;
		}

		bool has_changed() {
			bool listen_enable = lbz_preferences::m_listen_enable.get_value();
			if ((bool)listen_enable_checkbox.GetCheck() != listen_enable) return true;

			pfc::string8 temp;
			uGetWindowText(user_name_textbox, temp);
			if (lbz_preferences::m_user_name != temp) return true;
			uGetWindowText(user_token_textbox, temp);
			if (lbz_preferences::m_user_token != temp) return true;

			return false;
		}

		t_uint32 get_state() {
			t_uint32 state = preferences_state::resettable;
			if (has_changed()) state |= preferences_state::changed;
			return state;
		}

		void apply() {
			lbz_preferences::m_listen_enable = (bool)listen_enable_checkbox.GetCheck();
			uGetWindowText(user_name_textbox, lbz_preferences::m_user_name);
			uGetWindowText(user_token_textbox, lbz_preferences::m_user_token);
		}

		void on_change() {
			on_change_callback->on_state_changed();
		}

		void reset() {
			listen_enable_checkbox.SetCheck(lbz_preferences::default_listen_enable);
			uSetWindowText(user_name_textbox, "");
			uSetWindowText(user_token_textbox, "");

			on_change();
		}

		void OnChanged(UINT, int, HWND) {
				on_change();
		}
	};

	class lbz_preferences_page : public preferences_page_impl<lbz_preferences_page_instance> {
	public:
		const char * get_name() {
			return COMPONENT_TITLE;
		}

		GUID get_guid() {
			static const GUID guid =
			{ 0xe2df13c8, 0x4fab, 0x45f0,{ 0x97, 0xfb, 0x31, 0x17, 0x3f, 0x6b, 0xbe, 0xb4 } };

			return guid;
		}

		GUID get_parent_guid() {
			return preferences_page::guid_tools;
		}
	};

	preferences_page_factory_t<lbz_preferences_page> g_preferences_page;
}
