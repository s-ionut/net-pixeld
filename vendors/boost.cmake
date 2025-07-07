# NOT USED YET

set(Boost_REQUIRED_VERSION 1.81.0)
set(Boost_COMPONENTS filesystem system)

find_package(Boost ${Boost_REQUIRED_VERSION} COMPONENTS ${Boost_COMPONENTS} QUIET)

if (NOT Boost_FOUND)
    include(FetchContent)

    FetchContent_Declare(
        boost
        GIT_REPOSITORY https://github.com/boostorg/boost.git
        GIT_TAG boost-${Boost_REQUIRED_VERSION}
    )
    FetchContent_MakeAvailable(boost)

    set(Boost_INCLUDE_DIRS ${boost_SOURCE_DIR})
endif()