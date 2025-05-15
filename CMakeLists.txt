cmake_minimum_required(VERSION 3.30)
project(my_application LANGUAGES CXX VERSION 1.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSLINT_STYLE=fluent-dark")

if (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
elseif (UNIX AND NOT APPLE)
    option(BUILD_STATIC "Build static executable" OFF)

    if (BUILD_STATIC)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
    endif ()

    find_package(X11 REQUIRED)
    # Uncomment if using Wayland
    # find_package(Wayland REQUIRED)
endif ()

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
add_executable("Popup_Window" src/window.cpp)

if (WIN32)
    set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY WIN32_EXECUTABLE TRUE)
    set_property(TARGET "Popup_Window" APPEND PROPERTY WIN32_EXECUTABLE TRUE)
elseif(APPLE)
    set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY MACOSX_BUNDLE TRUE)
    set_property(TARGET "Popup_Window" APPEND PROPERTY MACOSX_BUNDLE TRUE)
elseif (UNIX AND NOT APPLE)
    install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
    install(TARGETS "Popup_Window" RUNTIME DESTINATION bin)
endif ()

target_link_libraries(${PROJECT_NAME} PRIVATE Slint::Slint)
target_link_libraries("Popup_Window" PRIVATE Slint::Slint)

if (UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${X11_LIBRARIES})
    target_link_libraries("Popup_Window" PRIVATE ${X11_LIBRARIES})

    # Uncomment if using Wayland
    # target_link_libraries(${PROJECT_NAME} PRIVATE ${Wayland_LIBRARIES})
    # target_link_libraries("Popup_Window" PRIVATE ${Wayland_LIBRARIES})
endif ()

slint_target_sources(${PROJECT_NAME} ui/my_application_ui.slint)
slint_target_sources("Popup_Window" ui/popup_ui.slint)

# On Windows, copy the Slint DLL next to the application binary so that it's found.
if (WIN32)
    target_sources(${PROJECT_NAME} PRIVATE res/app.rc)
    add_custom_command(TARGET my_application POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:my_application> $<TARGET_FILE_DIR:my_application> COMMAND_EXPAND_LISTS)
    add_custom_command(TARGET my_application POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/src/run_hidden.vbs $<TARGET_FILE_DIR:my_application> VERBATIM)
endif()
