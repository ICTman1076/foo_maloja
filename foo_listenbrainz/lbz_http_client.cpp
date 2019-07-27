#include "stdafx.h"
#include "lbz_http_client.h"
#include <WinInet.h>

#define HTTP_USER_AGENT COMPONENT_NAME "/" COMPONENT_VERSION

using namespace foo_listenbrainz;

void lbz_http_client::post_url(const char *host, const char *object,
	const char *header, const char *data, abort_callback &callback) {
	char httpUseragent[512];
	DWORD szhttpUserAgent = sizeof(httpUseragent);
	ObtainUserAgentString(0, httpUseragent, &szhttpUserAgent);

	HINTERNET internet = InternetOpenA(httpUseragent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (internet != NULL)
	{
		HINTERNET connect = InternetConnectA(internet, host, INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
		if (connect != NULL)
		{
			HINTERNET request = HttpOpenRequestA(connect, "POST", object, "HTTP/1.1", NULL, NULL,
				INTERNET_FLAG_HYPERLINK |
				INTERNET_FLAG_IGNORE_CERT_CN_INVALID |
				INTERNET_FLAG_IGNORE_CERT_DATE_INVALID |
				INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP |
				INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS |
				INTERNET_FLAG_NO_AUTH |
				INTERNET_FLAG_NO_CACHE_WRITE |
				INTERNET_FLAG_NO_UI |
				INTERNET_FLAG_PRAGMA_NOCACHE |
				INTERNET_FLAG_RELOAD |
				INTERNET_FLAG_SECURE, NULL);

			if (request != NULL)
			{
				int datalen = 0;
				if (data != NULL) datalen = strlen(data);
				int headerlen = 0;
				if (header != NULL) headerlen = strlen(header);

				if (!HttpSendRequestA(request, header, headerlen, (LPVOID)data, datalen))
				{
					DWORD err = GetLastError();
				}

				char responseText[256];
				DWORD responseTextSize = sizeof(responseText);

				if (!HttpQueryInfo(request, HTTP_QUERY_STATUS_CODE, &responseText, &responseTextSize, NULL))
				{
					DWORD err = GetLastError();
				}

				/*DWORD dwRead = 0;
				bool avail = InternetQueryDataAvailable(request, &dwRead, 0, 0);
				InternetReadFile(request, (LPVOID)responseText, responseTextSize, &dwRead);
				console::info(responseText);*/

				InternetCloseHandle(request);
			}
		}
		InternetCloseHandle(connect);
	}
	InternetCloseHandle(internet);
}
