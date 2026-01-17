# Config
- copy ``/dc_client/config.json`` into the root directory of the exe
- ``token=<discord token>``
- ``subscriptions: follow provided format~``

# Project Setup
## packages

``vcpkg install boost-beast boost-asio zlib openssl nlohmann-json``

## linking

- additional include dirs: ``ultralight/include``
- additional lib dirs: ``ultralight/lib``
- additional dependancies: ``AppCore.lib`` ``Ultralight.lib`` ``UltralightCore.lib`` ``WebCore.lib``

## Files
### ./ <small>``(root directory)``</small>
- ``AppCore.dll``
- ``Ultralight.dll``
- ``UltralightCore.dll``
- ``WebCore.dll``

### ./resources
- ``cacert.pem``
- ``icudt67l.dat``

## Preprocessor Definitions
- ``_WIN32_WINNT=0x0A00``
a
