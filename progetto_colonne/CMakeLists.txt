cmake_minimum_required(VERSION 3.15)
project(progetto_colonne)

set(CMAKE_CXX_STANDARD 14)

add_executable(progetto_colonne main.cpp Tabella.cpp Tabella.h Colonna.cpp Colonna.h ColonnaInt.cpp ColonnaInt.h ColonnaFloat.cpp ColonnaFloat.h ColonnaText.cpp ColonnaText.h)