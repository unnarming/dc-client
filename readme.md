# Config
- copy ``config.json`` into the root directory of the exe
- ``token=<discord token>``
- ``subscriptions: follow provided format~``

# Project Setup
## packages

``vcpkg install boost-beast boost-asio zlib openssl nlohmann-json``

## linking

- additional include dirs: ``sciter/include``

## Files
### ./ <small>``(root directory)``</small>
- ``sciter.dll``
- ``index.html``
- ``config.json``

## Preprocessor Definitions
- ``_WIN32_WINNT=0x0A00``