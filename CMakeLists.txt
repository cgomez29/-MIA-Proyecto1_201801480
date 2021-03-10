cmake_minimum_required(VERSION 3.17)
project(Proyecto1)

set(CMAKE_CXX_STANDARD 14)

add_executable(Proyecto1 main.cpp Analizador/scanner.cpp Analizador/scanner.h Analizador/parser.cpp Analizador/parser.h Source/Node.cpp Headers/Node.h Source/CommandChecker.cpp Headers/CommandChecker.h Source/Controller.cpp Headers/Controller.h Source/SimpleList.cpp Headers/SimpleList.h Source/Mount.cpp Headers/Mount.h Source/ControllerReport.cpp Headers/ControllerReport.h Source/ControllerFileSystem.cpp Headers/ControllerFileSystem.h Headers/Structure.h)