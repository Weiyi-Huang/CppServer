# CppServer
This is a Server based on Linux, which build with CMake

## Project Architecture
- src: Network library code
- test: Network test code
- build_support: Code review and formatting
```text
.
├── CMakeLists.txt
├── README.md
├── build_support
│   ├── clang_format_exclusions.txt
│   ├── cpplint.py
│   ├── run_clang_format.py
│   ├── run_clang_tidy.py
│   └── run_clang_tidy_extra.py
├── cmake_gen_linux.sh
├── src
│   ├── Acceptor.cpp
│   ├── Buffer.cpp
│   ├── CMakeLists.txt
│   ├── Channel.cpp
│   ├── Connection.cpp
│   ├── Epoll.cpp
│   ├── EventLoop.cpp
│   ├── Server.cpp
│   ├── Socket.cpp
│   ├── ThreadPool.cpp
│   ├── Util.cpp
│   └── include
│       ├── Acceptor.h
│       ├── Buffer.h
│       ├── Channel.h
│       ├── Connection.h
│       ├── Epoll.h
│       ├── EventLoop.h
│       ├── Macros.h
│       ├── Server.h
│       ├── Socket.h
│       ├── ThreadPool.h
│       └── Util.h
└── test
    ├── CMakeLists.txt
    ├── multiple_client.cpp
    ├── server.cpp
    ├── single_client.cpp
    └── thread_test.cpp

```
