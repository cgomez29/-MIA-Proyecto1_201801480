cmake_minimum_required(VERSION 3.17)
project(Proyecto1)

set(CMAKE_CXX_STANDARD 14)

add_executable(Proyecto1 main.cpp scanner.cpp scanner.h parser.cpp parser.h Node.cpp Node.h CommandChecker.cpp CommandChecker.h)