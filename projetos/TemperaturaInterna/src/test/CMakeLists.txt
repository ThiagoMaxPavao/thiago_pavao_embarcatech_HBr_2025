cmake_minimum_required(VERSION 3.13)
project(unit_tests C)

set(CMAKE_C_STANDARD 11)

# Adiciona o executável de teste
add_executable(unit_tests
    test.c
    ../app/convert_temperature.c
    ../unity/unity.c
)

# Inclui os headers
target_include_directories(unit_tests PRIVATE
    ../inc
    ../unity
    ../app
)
