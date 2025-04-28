cmake_minimum_required(VERSION 3.13)
project(UnitTests C)

set(CMAKE_C_STANDARD 11)

# Adiciona o executável de teste
add_executable(test
    test.c
    ../app/convert_temperature.c
    ../unity/unity.c
)

# Inclui os headers
target_include_directories(test PRIVATE
    ../inc
    ../unity
    ../app
)
