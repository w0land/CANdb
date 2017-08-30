find_package(Boost REQUIRED)

set(SRC
    candb.cpp
    dbcparser.cpp
)

add_library(CANdbc ${SRC})
target_include_directories(CANdbc PRIVATE ${Boost_INCLUDE_DIRS})

