cmake_minimum_required(VERSION 3.25)
project(ProjektGrafy)

set(CMAKE_CXX_STANDARD 23)

add_executable(ProjektGrafy main.cpp ZapisDoPliku.cpp ZapisDoPliku.h GenerowanieGrafu.cpp GenerowanieGrafu.h Dijkstry.cpp Dijkstry.h Kruskal.cpp Kruskal.h Prima.cpp Prima.h BellmanaFord.cpp BellmanaFord.h)
