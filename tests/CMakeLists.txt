include_directories(../src)

include_directories(.)

add_executable(tests
# skiplist tests
    journal_test.cpp
    skip_list_test.cpp
#
# skiplist sources
    ../src/time_stamp.h
    ../src/time_stamp.cpp
    ../src/ordered_list.hpp    
    ../src/ordered_list.h
    ../src/skip_list.h
    ../src/skip_list.hpp
    ../src/net_activity.h
    ../src/net_activity.cpp
    ../src/journal_net_activity.h
    ../src/journal_net_activity.hpp    
#
# gtest sources
    gtest/gtest-all.cc
    gtest/gtest_main.cc
)

# add pthread for unix systems
if (UNIX)
    target_link_libraries(tests pthread)
endif ()

# for coverage
# TARGET_COMPILE_OPTIONS(tests PUBLIC --coverage)
# target_link_libraries(tests gcov)
