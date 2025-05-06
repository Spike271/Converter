cmake_minimum_required(VERSION 3.30)
project(my_application LANGUAGES CXX)

set(CMAKE_INSTALL_PREFIX "E:/Software/slint")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

# Dependencies
# find_package(Slint)
if (NOT Slint_FOUND)
    message("Slint could not be located in the CMake module search path. Downloading it from Git and building it locally")
    include(FetchContent)
    FetchContent_Declare(
            Slint
            GIT_REPOSITORY https://github.com/slint-ui/slint.git
            GIT_TAG release/1
            SOURCE_SUBDIR api/cpp
    )
    FetchContent_MakeAvailable(Slint)
endif (NOT Slint_FOUND)

add_executable(${PROJECT_NAME} src/main.cpp)
set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY WIN32_EXECUTABLE TRUE)
target_link_libraries(${PROJECT_NAME} PRIVATE Slint::Slint)
slint_target_sources(${PROJECT_NAME} ui/my_application_ui.slint)
target_sources(${PROJECT_NAME} PRIVATE res/app.rc)

# On Windows, copy the Slint DLL next to the application binary so that it's found.
if (WIN32)
    add_custom_command(TARGET my_application POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:my_application> $<TARGET_FILE_DIR:my_application> COMMAND_EXPAND_LISTS)
endif()
