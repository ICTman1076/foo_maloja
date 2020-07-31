#include "stdafx.h"
#include "resource.h"
#include "lbz_preferences.h"
#include "foo_maloja.h"
#include <helpers/atl-misc.h>


namespace foo_maloja {
	namespace lbz_preferences {
		//const char *m_server_url = "api.listenbrainz.org"; // TODO: remove

		// {68C775BE-A025-44A2-81FC-80DA7BFA3463}
		static const GUID guid_listen_enable =
		{ 0x68c775be, 0xa025, 0x44a2, { 0x81, 0xfc, 0x80, 0xda, 0x7b, 0xfa, 0x34, 0x63 } };

		const bool default_listen_enable = false;
		cfg_bool m_listen_enable(guid_listen_enable, default_listen_enable);

		// {84624BF3-F912-4E00-AFCA-07AA846F514B}
		static const GUID guid_server =
		{ 0x84624bf3, 0xf912, 0x4e00, { 0xaf, 0xca, 0x7, 0xaa, 0x84, 0x6f, 0x51, 0x4b } };

		const std::string default_server = "";
		cfg_string m_server(guid_server, "");

		// {BD13AAC0-332F-4368-958D-8CA8660087B4}
		static const GUID guid_user_token =
		{ 0xbd13aac0, 0x332f, 0x4368, { 0x95, 0x8d, 0x8c, 0xa8, 0x66, 0x0, 0x87, 0xb4 } };
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
			uSetWindowText(user_name_textbox, lbz_preferences::m_server);
			uSetWindowText(user_token_textbox, lbz_preferences::m_user_token);

			return 0;
		}

		bool has_changed() {
			bool listen_enable = lbz_preferences::m_listen_enable.get_value();
			if ((bool)listen_enable_checkbox.GetCheck() != listen_enable) return true;

			pfc::string8 temp;
			uGetWindowText(user_name_textbox, temp);
			if (lbz_preferences::m_server != temp) return true;
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
			if(lbz_preferences::m_server )
			uGetWindowText(user_name_textbox, lbz_preferences::m_server);
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
			// {1C44C7B9-6997-44DF-8963-21FCBF53F0D4}
			static const GUID guid =
			{ 0x1c44c7b9, 0x6997, 0x44df, { 0x89, 0x63, 0x21, 0xfc, 0xbf, 0x53, 0xf0, 0xd4 } };


			return guid;
		}

		GUID get_parent_guid() {
			return preferences_page::guid_tools;
		}
	};

	preferences_page_factory_t<lbz_preferences_page> g_preferences_page;
}
