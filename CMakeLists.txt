cmake_minimum_required(VERSION 3.23)

# --- Fetch FTXUI --------------------------------------------------------------
include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
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

# Find OpenSSL ------------
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

# -------------------------

project(master_server)

set(CMAKE_CXX_STANDARD 20)

set(INCLUDE_DIRECTORY ${CMAKE_SOURCE_DIR}/include)
set(INCLUDE_BOOST /dev/boost_1_80_0/boost)

set(SOURCE_DIRECTORY ${CMAKE_SOURCE_DIR}/src)

include_directories(
    ${INCLUDE_DIRECTORY}
    ${INCLUDE_BOOST}
)

file(GLOB_RECURSE SOURCE_FILES
    ${SOURCE_DIRECTORY}/*.cpp
)

add_executable(master_server src/main.cpp ${SOURCE_FILES})

target_include_directories(master_server
    PRIVATE src
    PRIVATE ${OPENSSL_INCLUDE_DIR}
)
target_link_libraries(master_server
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component
    PRIVATE OpenSSL::SSL
    PRIVATE OpenSSL::Crypto
)