project(CHATUOL-SERVER)

include(GNUInstallDirs)
cmake_minimum_required(VERSION 3.1.0)
cmake_policy(VERSION 3.1.0)

# busca os headers do threading
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set (EXECUTABLE_NAME "chatuol")
# seta o c++ no c++20
set (CMAKE_CXX_STANDARD 20)

# set sources and headers files set sources from plugin on google style.
file(GLOB_RECURSE
     SW_SRCS
     ./src/*.cpp
     ./src/*/*.cxx
     ./src/*/*.c)
# set headers from plugin on google style.

file(GLOB_RECURSE
     SW_HDRS
     ./include/*.h
     ./include/*/*.hxx
     ./include/*/*.hpp
		 ./include/*/*.hh)
		 
add_executable(${EXECUTABLE_NAME} ${SW_SRCS} ${SW_HDRS})

#inclue os headers como dependencia
target_link_libraries(chatuol PUBLIC Threads::Threads)
