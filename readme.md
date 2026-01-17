## packages

``vcpkg install boost-beast boost-asio zlib openssl nlohmann-json``

## linking

- additional include dirs: ``ultralight/include``
- additional lib dirs: ``ultralight/lib``
- additional dependancies: ``AppCore.lib`` ``Ultralight.lib`` ``UltralightCore.lib`` ``WebCore.lib``

## Files
### ./ (exe)
- ``AppCore.dll``
- ``Ultralight.dll``
- ``UltralightCore.dll``
- ``WebCore.dll``

### ./resources
- ``cacert.pem``
- ``icudt67l.dat``

## Preprocessor Definitions
- ``_WIN32_WINNT=0x0A00``