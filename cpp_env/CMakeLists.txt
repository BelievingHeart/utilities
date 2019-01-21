cmake_minimum_required(VERSION 3.12)
project(hello)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

#set(CMAKE_LINK_WHAT_YOU_USE TRUE)
#find_program(TIDY "clang-tidy-6.0")
#set(CMAKE_CXX_CLANG_TIDY ${TIDY} "-checks=*")
#find_program(CPPCHECK cppcheck "--std=c++14")
#set(CMAKE_CXX_CPPCHECK ${CPPCHECK})
#find_program(IWYU include-what-you-use)
#set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE  ${IWYU})


# find_package(OpenCV REQUIRED)
# find_package(fmt REQUIRED CONFIG)
# find_package(Boost REQUIRED)

# set(TBB_ROOT /home/afterburner/clones/tbb-cmake)
            # The following SHARED targets will be defined:
            # tbb tbbmalloc tbbmalloc_proxy
            # The following INCLUDE_DIR will be defined:
            # TBB_INSTALL_INCLUDE_DIR
# add_subdirectory(${TBB_ROOT} ${TBB_ROOT}/build-release)

include_directories(${CMAKE_SOURCE_DIR}/src)


add_executable(main 
                    main.cpp
                    src/timer.cpp
                    )

target_include_directories(main PRIVATE 
    # ${OpenCV_INCLUDE_DIRS}
    # ${Boost_INCLUDE_DIR}
    # ${TBB_INSTALL_INCLUDE_DIR}
    )
target_link_libraries(main PRIVATE 
    # ${OpenCV_LIBS}
    # fmt::fmt
    # ${Boost_LIBRARIES}
    # tbb tbbmalloc tbbmalloc_proxy
)


if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(main PRIVATE DEBUG)
endif()

