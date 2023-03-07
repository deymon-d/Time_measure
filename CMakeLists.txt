cmake_minimum_required(VERSION 3.24)
project(time_measure)
set(CMAKE_C_STANDARD 17)

add_executable(client client/client.c shared/copy.c)
add_executable(server server/server.c server/parser.c server/user.c shared/copy.c)
