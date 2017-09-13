include(GoogleTest)

embed_resources(dbc_samples dbc/tesla_can.dbc)
add_library(dbc_sample_lib ${dbc_samples})

add_executable(dbcparser_tests dbcparser_tests.cpp)
target_link_libraries(dbcparser_tests CANdbc dbc_sample_lib ${CMAKE_THREAD_LIBS_INIT} gtest gtest_main)
target_include_directories(dbcparser_tests PRIVATE ${CMAKE_SOURCE_DIR}/3rdParty/cpp-peglib/)

gtest_add_tests( dbcparser_tests "" AUTO)

add_executable(opendbc_tests opedbc_tests.cpp)
target_link_libraries(opendbc_tests CANdbc dbc_sample_lib ${CMAKE_THREAD_LIBS_INIT} gtest gtest_main)
target_include_directories(opendbc_tests PRIVATE ${CMAKE_SOURCE_DIR}/3rdParty/cpp-peglib/)

gtest_add_tests( opendbc_tests "" AUTO)
