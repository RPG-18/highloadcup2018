cmake_minimum_required(VERSION 3.10)
project(highloadcup)

set(CMAKE_CXX_STANDARD 17)

find_package(PostgreSQL REQUIRED)

if(PostgreSQL_FOUND)
    include_directories(${PostgreSQL_INCLUDE_DIRS})
endif(PostgreSQL_FOUND)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-msse4.1")
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS}  "-msse4.1")

add_definitions(-DRAPIDJSON_SSE2)

include_directories(3rdparty/rapidjson-1.1.0/include
                    3rdparty/spdlog-1.2.1/include
                    )

add_library(highloadcup SHARED highloadcup.h Phone.cpp Phone.h BitSet128.cpp BitSet128.h highloadcup.cpp BitSet128_GIN.cpp Logger.cpp Logger.h)
target_link_libraries(highloadcup ${PostgreSQL_LIBRARIES})

install(TARGETS highloadcup
        LIBRARY DESTINATION lib/postgresql/10/lib)

install(FILES highloadcup--0.1.sql highloadcup.control
        DESTINATION share/postgresql/10/extension)
