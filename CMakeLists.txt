cmake_minimum_required(VERSION 3.10.0)
project(restraunt_app_c VERSION 0.1.0 LANGUAGES C)

add_executable(restraunt_app_c 
main.c
data.c
functions.c
constants.c
)




find_package(Threads REQUIRED)
target_link_libraries(restraunt_app_c PRIVATE Threads::Threads)