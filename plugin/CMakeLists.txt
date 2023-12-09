cmake_minimum_required(VERSION 3.22)

# Version is needed by JUCE.
project(WhoaAudioPlugin VERSION 0.1.0)

# Adds a plugin target (that's basically what the Projucer does).
juce_add_plugin(${PROJECT_NAME}

        # Publishing
        PRODUCT_NAME "My Plugin Title" # this will show up as the plugin file
        PLUGIN_NAME "My Plugin Title" # this will show up in DAWs
        ICON_BIG assets/pluginIcon.jpg
        ICON_SMALL assets/pluginIcon.jpg
        BUNDLE_ID com.whoadrian.plugins
        COMPANY_NAME Whoadrian
        COMPANY_WEBSITE whoadrian.github.io
        COMPANY_EMAIL whoa.adrian@gmail.com
        PLUGIN_MANUFACTURER_CODE Whoa
        PLUGIN_CODE WA01

        # Builds
        FORMATS VST3 Standalone
        COPY_PLUGIN_AFTER_BUILD TRUE

        # Plugin settings
        VST3_CATEGORIES Fx Instrument Generator Stereo Synth Tools
        IS_SYNTH FALSE
        IS_MIDI_EFFECT FALSE
        NEEDS_MIDI_INPUT TRUE
        NEEDS_MIDI_OUTPUT TRUE
        BACKGROUND_AUDIO_ENABLED TRUE
        MICROPHONE_PERMISSION_ENABLED TRUE
        EDITOR_WANTS_KEYBOARD_FOCUS TRUE
        IPHONE_SCREEN_ORIENTATIONS UIInterfaceOrientationLandscapeLeft UIInterfaceOrientationLandscapeRight
        IPAD_SCREEN_ORIENTATIONS UIInterfaceOrientationLandscapeLeft UIInterfaceOrientationLandscapeRight
        )

# Links to all necessary dependencies. The present ones are recommended by JUCE.
# If you use one of the additional modules, like the DSP module, you need to specify it here.
target_link_libraries(${PROJECT_NAME}
        PRIVATE
            AudioPluginBinaryData # assets binary data
            juce::juce_audio_basics
            juce::juce_audio_utils
            juce::juce_audio_devices
        PUBLIC
            juce::juce_recommended_config_flags
            juce::juce_recommended_lto_flags
            juce::juce_recommended_warning_flags
        )

# These definitions are recommended by JUCE.
target_compile_definitions(${PROJECT_NAME}
        PUBLIC
            JUCE_WEB_BROWSER=0
            JUCE_USE_CURL=0
            JUCE_VST3_CAN_REPLACE_VST2=0
            JUCE_SILENCE_XCODE_15_LINKER_WARNING=1
            # JUCE_DEBUG=1
        )

# Sets the source files of the plugin project.
target_sources(${PROJECT_NAME}
        PRIVATE
            # py_generated=files dir=include extensions=h|cpp
            include/PluginEditor.h
            include/PluginProcessor.h
            # /py_generated

            # py_generated=files dir=source extensions=h|cpp
            source/PluginProcessor.cpp
            source/PluginEditor.cpp
            # /py_generated
        )

# Sets the include directories of the plugin project.
target_include_directories(${PROJECT_NAME}
        PUBLIC
            # py_generated=directories dir=include
            include
            # /py_generated

        PRIVATE
            # py_generated=directories dir=source
            source
            # /py_generated
        )

# Extra binary assets. First argument is the name of the static library. SOURCES is followed by a list of source files (wav, images, etc). Conversion happening during build. Optional NAMESPACE argument for the binary data class.
juce_add_binary_data(AudioPluginBinaryData SOURCES
            # py_generated=files dir=assets
            assets/pluginIcon.jpg
            # /py_generated
        )

# Silences some deprecation warnings that (hopefully) will be fixed in a future release of JUCE.
if (MSVC)
    target_compile_definitions(${PROJECT_NAME}
            PRIVATE
            _SILENCE_CXX23_ALIGNED_STORAGE_DEPRECATION_WARNING)
endif()

# Colour our warnings and errors
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options (-fdiagnostics-color=always)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options (-fcolor-diagnostics)
endif ()

# IDE Setup
# In Visual Studio this command provides a nice grouping of source files in "filters".
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/..)
# By default we don't want Xcode schemes to be made for modules, etc
set(CMAKE_XCODE_GENERATE_SCHEME OFF)
# Adds all the module sources so they appear correctly in the IDE
set_property(GLOBAL PROPERTY USE_FOLDERS YES)
# Create a /Modules directory in the IDE with the JUCE Module code
option(JUCE_ENABLE_MODULE_SOURCE_GROUPS "Show all module sources in IDE projects" ON)
# No, we don't want our source buried in extra nested folders
set_target_properties("${PROJECT_NAME}" PROPERTIES FOLDER "")
# The Xcode source tree should uhhh, still look like the source tree, yo
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/Source PREFIX "" FILES ${SourceFiles})
