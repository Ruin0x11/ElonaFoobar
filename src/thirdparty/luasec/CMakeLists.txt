# Copyright (C) 2007-2012 LuaDist.
# Submitted by David Manura
# Redistribution and use of this file is allowed according to the terms of the MIT license.
# For details see the COPYRIGHT file distributed with LuaDist.
# Please note that the package source code is licensed under its own license.

cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

if(MSVC)
    set(CMAKE_C_FLAGS           "/W3 /WX")
    set(CMAKE_C_FLAGS_RELEASE   "/MD /O2 /Ob2")
else()
    set(CMAKE_C_FLAGS           "-std=gnu99 -Wall -pedantic -fPIC -D_GNU_SOURCE")
    set(CMAKE_C_FLAGS_RELEASE   "-O2")
endif()
set(CMAKE_SHARED_LIBRARY_PREFIX "")

find_package(Lua REQUIRED)

find_package(OpenSSL REQUIRED)

# if (APPLE)
#   set(OPENSSL_ROOT_DIR /opt/local)
# endif()

if(WIN32)
  set(SOCKET_LIBRARY ws2_32)
  set(SOCKETC src/luasocket/wsocket.c)
else()
  set(SOCKETC src/luasocket/usocket.c)
endif()

set(LUASOCKET_SRC
src/luasocket/buffer.c
src/luasocket/io.c
src/luasocket/timeout.c
${SOCKETC}
)
add_library(luasocket STATIC ${LUASOCKET_SRC})
target_compile_definitions(luasocket PRIVATE LUASOCKET_DEBUG)
include_directories(src SYSTEM PRIVATE)

set(LUASEC_SRC
src/config.c
src/context.c
src/ec.c
src/ssl.c
src/x509.c
)

include_directories(${LUA_INCLUDE_DIR} ${OPENSSL_INCLUDE_DIR})
add_library(ssl SHARED ${LUASEC_SRC})
if(MSVC)
  target_link_libraries(ssl luasocket ${LUA_LIBRARIES} ws2_32)
endif()
target_link_libraries(ssl luasocket ${OPENSSL_LIBRARIES})

## Binary
INSTALL(TARGETS ssl
  LIBRARY DESTINATION ${LIBDIR} COMPONENT Runtime
  RUNTIME DESTINATION bin COMPONENT Runtime
)

# Lua
set(LUASEC_FILES
    src/ssl.lua
    PARENT_SCOPE)

set(LUASEC_SSL_FILES
    src/https.lua
    PARENT_SCOPE)
