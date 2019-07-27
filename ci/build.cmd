setlocal
cd "%~dp0"

@if defined BUILD_TYPE set BUILD_CMD=/p:Configuration=%BUILD_TYPE%
@if defined TARGET set TARGET_CMD=/target:%TARGET%

msbuild.cmd ..\foo_listenbrainz.sln %BUILD_CMD% %TARGET_CMD%
