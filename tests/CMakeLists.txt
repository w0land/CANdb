add_executable(parsingTest parsing.cpp)
target_link_libraries(parsingTest CANdbc ${CMAKE_THREAD_LIBS_INIT})
add_test( NAME ParsingTest COMMAND parsingTest)

add_executable(dbcparser_tests dbcparser_tests.cpp)
target_link_libraries(dbcparser_tests CANdbc ${CMAKE_THREAD_LIBS_INIT} ${GTEST_BOTH_LIBRARIES})
add_test( NAME DBCParserTests COMMAND dbcparser_tests)
