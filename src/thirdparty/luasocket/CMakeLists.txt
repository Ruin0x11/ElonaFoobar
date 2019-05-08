# Copyright (C) 2007-2013 LuaDist.
# Created by Peter Drahoš, David Manura
# Redistribution and use of this file is allowed according to the terms of the MIT license.
# For details see the COPYRIGHT file distributed with LuaDist.
# Please note that the package source code is licensed under its own license.

option(LUASOCKET_UNIX "Use only unix domain features." OFF)

find_package(Lua REQUIRED)

include_directories(${LUA_INCLUDE_DIR})
if(MSVC)
  target_link_libraries(luasocket ${LUA_LIBRARIES} ws2_32)
endif()

# The standard luasocket build has this enabled in release mode.
# It is required for running the test suite.
add_definitions(-DLUASOCKET_DEBUG)
# if(MINGW)
#   add_definitions(-DLUASOCKET_INET_PTON -D_WIN32_WINNT=0x501)
# endif()

# Shared library exporting.
if(WIN32)
  add_definitions("-DMIME_API=__declspec(dllexport)" "-DLUASOCKET_API=__declspec(dllexport)")
endif()

if(WIN32 AND NOT CYGWIN)
  set(SOCKETC src/wsocket.c)
else()
  set(SOCKETC src/usocket.c)
endif()

set(SRC_SOCKET src/luasocket.c src/timeout.c src/buffer.c src/io.c src/auxiliar.c
  src/options.c src/inet.c src/tcp.c src/udp.c src/except.c src/select.c ${SOCKETC})

set(UNIX_SOCKET src/unix.c src/io.c src/auxiliar.c src/timeout.c src/options.c
  src/buffer.c ${SOCKETC})

if(WIN32 AND NOT CYGWIN)
  set(LIB_SOCKET ws2_32)
endif ()

# Solaris needs specific links
# if(CMAKE_SYSTEM_NAME STREQUAL "SunOS")
# 	set(LIB_SOCKET socket resolv rt)
# endif()

ADD_LIBRARY(socket_core SHARED ${SRC_SOCKET})
set_target_properties(socket_core PROPERTIES PREFIX "")
target_link_libraries(socket_core ${LIB_SOCKET} ${LUALIB})

ADD_LIBRARY(mime_core SHARED src/mime.c)
set_target_properties(mime_core PROPERTIES PREFIX "")
target_link_libraries(mime_core ${LIB_SOCKET} ${LUALIB})

if(LUASOCKET_UNIX)
  ADD_LIBRARY(socket_unix MODULE ${UNIX_SOCKET})
  set_target_properties(socket_unix PROPERTIES PREFIX "")
  target_link_libraries(socket_unix ${LIB_SOCKET} ${LUALIB})
endif ()

## Binary
INSTALL(TARGETS socket_core mime_core
  LIBRARY DESTINATION ${LIBDIR} COMPONENT Runtime
  RUNTIME DESTINATION bin COMPONENT Runtime
)

# Lua
set(LUASOCKET_FILES
    src/ltn12.lua
    src/socket.lua
    src/mime.lua
    PARENT_SCOPE)

set(LUASOCKET_SOCKET_FILES
    src/http.lua
    src/url.lua
    src/tp.lua
    src/ftp.lua
    src/headers.lua
    src/smtp.lua
    PARENT_SCOPE)
