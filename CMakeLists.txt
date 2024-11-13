cmake_minimum_required(VERSION 3.22)

project(cool_synth VERSION 0.0.1)

find_package(JUCE CONFIG REQUIRED)
find_package(CUDA REQUIRED)
find_package(OpenGL REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(
  "-Wall"
  "$<$<CONFIG:Debug>:-O0;-g3;-ggdb3;>"
  "$<$<CONFIG:Release>:-O3;-mtune=native;-march=native;-DNDEBUG;>"
)

add_compile_options(
  "$<$<COMPILE_LANGUAGE:CUDA>:
    $<$<CONFIG:Debug>:-g;-G;-O0;>
    $<$<CONFIG:Release>:-O3;>>"
)
add_compile_options(
  "$<$<COMPILE_LANGUAGE:CUDA>:--generate-line-info;-O3;-gencode=arch=compute_86,code=sm_86;>"
)

#add_compile_options(-fsanitize=undefined)
#add_link_options(-fsanitize=address,undefined -static-libsan)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include_directories(src)
include_directories(${CUDA_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIRS})

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)
enable_testing()

juce_add_plugin(
  CoolSynthPlugin

  NEEDS_MIDI_INPUT TRUE
  NEEDS_MIDI_OUTPUT FALSE
  COPY_PLUGIN_AFTER_BUILD TRUE
  PLUGIN_MANUFACTURER_CODE Just
  PLUGIN_CODE Cool

  FORMATS VST3 Standalone
  PRODUCT_NAME "Cool Synth"
)

juce_generate_juce_header(CoolSynthPlugin)

set(cuda_files)
set(src_files
  src/gui/main.cpp
  src/gui/MainWindow.cpp
)
set(header_files
  src/gui/MainWindow.hpp
  src/audio/CoolSynthPlugin.hpp
)

target_sources(
  CoolSynthPlugin
  PRIVATE
  ${src_files}
  ${cuda_files}
  ${header_files}
)

target_link_libraries(
  CoolSynthPlugin
  PRIVATE
  juce::juce_core
  juce::juce_data_structures
  juce::juce_events
  juce::juce_graphics
  juce::juce_gui_basics
  juce::juce_gui_extra
  juce::juce_opengl
  PUBLIC
  juce::juce_recommended_config_flags
  juce::juce_recommended_lto_flags
  juce::juce_recommended_warning_flags
  ${OPENGL_LIBRARIES}
)

target_compile_definitions(
  CoolSynthPlugin
  PUBLIC
  JUCE_WEB_BROWSER=0
  JUCE_USE_CURL=0
  JUCE_VST3_CAN_REPLACE_VST2=0
)


