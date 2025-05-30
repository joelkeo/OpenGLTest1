cmake_minimum_required(VERSION 3.22)

# MacOS properties
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.13" CACHE STRING "Minimum OS X deployment target")
# Windows MultiThreaded debug behavior based on CONFIG
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# basic project info (including setting PROJECT_NAME)
project(OpenGLTest1 VERSION 1.0.0 LANGUAGES C CXX)

# include embedded JUCE dependency
add_subdirectory("libs/JUCE")

# IDE src organization
set_property(GLOBAL PROPERTY USE_FOLDERS YES)
# IDE JUCE organization
option(JUCE_ENABLE_MODULE_SOURCE_GROUPS "Enable Module Source Groups" ON)

# keep it compiler independent
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 20)

# generate PDB/dSYM files for the Release build
# setting system & CONFIG dependent compiler options
if(MSVC)
    add_compile_options("$<$<NOT:$<CONFIG:Debug>>:/Zi>")
    add_link_options("$<$<NOT:$<CONFIG:Debug>>:/DEBUG>")
    add_link_options("$<$<NOT:$<CONFIG:Debug>>:/OPT:REF>")
    add_link_options("$<$<NOT:$<CONFIG:Debug>>:/OPT:ICF>")
else()
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS[variant=Release] "YES")
    set(CMAKE_XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT[variant=Release] "dwarf-with-dsym")
    set(CMAKE_XCODE_ATTRIBUTE_DEPLOYMENT_POSTPROCESSING[variant=Release] "YES")
endif()

if(MSVC)
    add_compile_options("/W4")
endif()

juce_add_plugin(${PROJECT_NAME}
        COMPANY_NAME "InfernoAudio"
        PLUGIN_MANUFACTURER_CODE Infe
        PLUGIN_CODE Opn1
        BUNDLE_ID "com.InfernoAudio.OpenGLTest1"
        IS_SYNTH FALSE
        NEEDS_MIDI_INPUT FALSE
        NEEDS_MIDI_OUTPUT FALSE
        IS_MIDI_EFFECT FALSE
        EDITOR_WANTS_KEYBOARD_FOCUS FALSE
        COPY_PLUGIN_AFTER_BUILD TRUE
        FORMATS AU VST3 Standalone
        PRODUCT_NAME "OpenGLTest1"
        VST3_COPY_DIR "C:/Program Files/Common Files/VST3"
)

juce_generate_juce_header(${PROJECT_NAME})

# globbed sources
file(GLOB SOURCE_FILES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

# juce module hookup stuff
target_sources(${PROJECT_NAME} PRIVATE ${SOURCE_FILES})

# basic juce options
target_compile_definitions(${PROJECT_NAME} PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_DISPLAY_SPLASH_SCREEN=0
        JUCE_REPORT_APP_USAGE=0
        JUCE_MODAL_LOOPS_PERMITTED=0
        DONT_SET_USING_JUCE_NAMESPACE=1
)

# included modules
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
        juce::juce_opengl
        PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
)