cmake_minimum_required(VERSION 3.7.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# --- Fetch FTXUI --------------------------------------------------------------
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(
    ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
    GIT_TAG main
    # Important: Specify a GIT_TAG XXXXX here.
)

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
    FetchContent_Populate(ftxui)
    add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
# ------------------------------------------------------------------------------

# --- Fetch OpenSSL ------------------------------------------------------------
set(OPENSSL_ROOT_DIR "C:/Program Files/OpenSSL-Win64")
set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)

set(OPENSSL_INCLUDE_DIR "C:/Program Files/OpenSSL-Win64/include")
set(OPENSSL_LIBRARIES
    "C:/Program Files/OpenSSL-Win64/lib/libcrypto.lib"
    "C:/Program Files/OpenSSL-Win64/lib/libssl.lib"
)

message("OpenSSL include dir: ${OPENSSL_INCLUDE_DIR}")
message("OpenSSL libraries: ${OPENSSL_LIBRARIES}")
# ------------------------------------------------------------------------------

# --- Fetch Google Test --------------------------------------------------------
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG main
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# ------------------------------------------------------------------------------

# --- Fetch LibE1.31 -----------------------------------------------------------
FetchContent_Declare(
    libe131
    GIT_REPOSITORY https://github.com/hhromic/libe131
    GIT_TAG master
)
# ------------------------------------------------------------------------------

FetchContent_MakeAvailable(googletest libe131)

set(INCLUDE_DIRECTORY ${CMAKE_SOURCE_DIR}/include)
set(SOURCE_DIRECTORY ${CMAKE_SOURCE_DIR}/src)

file(GLOB_RECURSE SOURCE_FILES
    ${SOURCE_DIRECTORY}/monet/*.cpp
    ${libe131_SOURCE_DIR}/src/*.c
)

include_directories(
    src
    ${OPENSSL_INCLUDE_DIR}
    ${INCLUDE_DIRECTORY}
    ${libe131_SOURCE_DIR}/src
)

link_libraries(
    ftxui::screen
    ftxui::dom
    ftxui::component
    ${OPENSSL_LIBRARIES}
)

project(master_server)
add_executable(master_server src/main.cpp ${SOURCE_FILES})


file(GLOB_RECURSE TESTING_SOURCE_FILES
    ${SOURCE_DIRECTORY}/testing/*.cpp
)

enable_testing()
add_executable(testing src/testing.cpp ${SOURCE_FILES} ${TESTING_SOURCE_FILES})
target_link_libraries(testing
    GTest::gtest_main
)

target_include_directories(testing PRIVATE
    ${gtest_SOURCE_DIR}/include
    ${gtest_SOURCE_DIR}
)

include(GoogleTest)
gtest_discover_tests(testing)
