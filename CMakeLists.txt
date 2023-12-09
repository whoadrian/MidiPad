cmake_minimum_required(VERSION 3.22)

project(WhoaAudioPluginTemplateProject)

# C++ standard
set(CMAKE_CXX_STANDARD 20)

# Third party libraries download location
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs)

# Downloads CPM if not already downloaded. CPM is an easy-to-use package manager nicely integrated with CMake.
include(cmake/cpm.cmake)

# Run the autofill python tool
message(STATUS "Running Python CMake autofill tool")
execute_process(COMMAND python cmake_autofill.py
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tools/
        RESULT_VARIABLE autofill_result)
message(STATUS "Python CMake autofill tool - Result: ${autofill_result}" )

# This commands downloads AND configures JUCE. It sets up some variables, like JUCE_SOURCE_DIR.
CPMAddPackage(
        NAME JUCE
        GIT_TAG 7.0.8
        VERSION 7.0.8
        GITHUB_REPOSITORY juce-framework/JUCE
        SOURCE_DIR ${LIB_DIR}/juce
)

# Build the audio plugin host
message(STATUS "JUCE AudioPluginHost - Building...")
if (MSVC)
    execute_process(
            WORKING_DIRECTORY ${LIB_DIR}/juce/extras/AudioPluginHost/Builds/VisualStudio2022/
            COMMAND msbuild AudioPluginHost.sln /p:Configuration=Release /p:Platform=x64
            RESULT_VARIABLE host_build_result
    )
    message(STATUS "JUCE AudioPluginHost - Build Result: ${host_build_result}" )
else()
    execute_process(
            WORKING_DIRECTORY ${LIB_DIR}/juce/extras/AudioPluginHost/Builds/MacOSX/
            COMMAND xcodebuild -configuration Release build
            RESULT_VARIABLE host_build_result
    )
    message(STATUS "JUCE AudioPluginHost - Build Result: ${host_build_result}" )
endif()

# Adds googletest
CPMAddPackage(
        NAME GOOGLETEST
        GITHUB_REPOSITORY google/googletest
        GIT_TAG v1.14.0
        VERSION 1.14.0
        SOURCE_DIR ${LIB_DIR}/googletest
        OPTIONS
        "INSTALL_GTEST OFF"
        "gtest_force_shared_crt ON"
)
# This command allows running tests from the "build" folder (the one where CMake generates the project to).
enable_testing()

# Add all the targets configured in subdirectories
add_subdirectory(plugin)
add_subdirectory(test)

# Enables all warnings and treats warnings as errors.
# This needs to be added AFTER all the third-party dependencies.
if(MSVC)
    add_compile_options(/Wall /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()
