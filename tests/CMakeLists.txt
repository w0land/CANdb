include(GoogleTest)

embed_resources(dbc_samples dbc/tesla_can.dbc)
add_library(dbc_sample_lib ${dbc_samples})

add_executable(dbcparser_tests dbcparser_tests.cpp)
target_link_libraries(dbcparser_tests CANdbc dbc_sample_lib ${CMAKE_THREAD_LIBS_INIT} ${GTEST_BOTH_LIBRARIES})
gtest_add_tests( dbcparser_tests "" AUTO)
