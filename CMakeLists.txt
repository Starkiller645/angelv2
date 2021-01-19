cmake_minimum_required(VERSION 3.1.0)

project(angelv1 VERSION 1.0.01 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(USE_SYSTEM_CURL ON)
set(BUILD_CPR_TESTS OFF)
set(MADDY_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(QTLOADINGSPINNER_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/QtWaitingSpinner)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Network REQUIRED)
find_package(Qt5 COMPONENTS WebSockets REQUIRED)
find_package(Qt5 COMPONENTS WebEngineWidgets REQUIRED)
find_package(CURL)
if(CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIR})
    set(requiredlibs ${requiredlibs} ${CURL_LIBRARIES})
else(CURL_FOUND)
    message(FATAL_ERROR "Could not find CURL library")
endif(CURL_FOUND)
include(FetchContent)
add_definitions(-DQT_DEBUG_OUTPUT)

FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/whoshuu/cpr.git GIT_TAG c8d33915dbd88ad6c92b258869b03aba06587ff9)
FetchContent_MakeAvailable(cpr)
FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
  GIT_TAG v3.9.1)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()


add_library(maddy INTERFACE)
target_include_directories(maddy INTERFACE ${MADDY_INCLUDE_DIR})

add_library(qtloadingspinner INTERFACE)
target_include_directories(qtloadingspinner INTERFACE ${QTLOADINGSPINNER_INCLUDE_DIR})


add_executable(angelv1
    src/mainwindow.cpp
    src/main.cpp
    src/authworker.cpp
    src/transparentbutton.cpp
    src/filejson.cpp
    src/mainui.cpp
    src/subredditwidget.cpp
    src/submissionwidget.cpp
    src/redditasyncrequest.cpp
    resources.qrc
    include/QtWaitingSpinner/waitingspinnerwidget.cpp
)

target_link_libraries(angelv1 Qt5::Widgets Qt5::WebEngineWidgets  Qt5::Core Qt5::Gui Qt5::Network Qt5::WebSockets)
target_link_libraries(angelv1 cpr::cpr nlohmann_json::nlohmann_json)
target_link_libraries(angelv1 maddy)
target_link_libraries(angelv1 qtloadingspinner)

install(TARGETS angelv1)
