cmake_minimum_required(VERSION 3.10)
project(WiFiLogParserTests)

find_package(GTest REQUIRED)
include(GoogleTest)

# Копируем основную программу в каталог тестов
add_custom_command(TARGET WiFiLogParser POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:WiFiLogParser> ${CMAKE_CURRENT_BINARY_DIR}
)

# Тестовый исполняемый файл
add_executable(${PROJECT_NAME} tests.cpp)
target_link_libraries(${PROJECT_NAME} GTest::gtest_main)

# Добавляем зависимость от основной программы
add_dependencies(${PROJECT_NAME} WiFiLogParser)

# Регистрируем тесты
gtest_discover_tests(${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)