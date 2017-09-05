include(GoogleTest)

add_executable(dbcparser_tests dbcparser_tests.cpp)
target_link_libraries(dbcparser_tests CANdbc ${CMAKE_THREAD_LIBS_INIT} ${GTEST_BOTH_LIBRARIES})
gtest_add_tests( dbcparser_tests "" AUTO)
