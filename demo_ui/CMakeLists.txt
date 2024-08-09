message("Building Demo")

if(NOT YAY_TUI_BUILD_EXAMPLES)
  return()
endif()

set(EXAMPLES_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


function(example name)
  add_executable(yay_tui_demo_${name} ${name}.cpp)
  target_include_directories(yay_tui_demo_${name} 
    PRIVATE ../include
    PRIVATE ../include/py_wraps
    PRIVATE ../include/widgets
  )
  
  target_link_libraries(yay_tui_demo_${name}
    PRIVATE widgets
    PRIVATE pyapi
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component  # Not needed for this example.
  )
  file(RELATIVE_PATH dir ${EXAMPLES_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
  set_property(GLOBAL APPEND PROPERTY YAY_TUI::DEMO ${dir}/${name})
  target_compile_features(yay_tui_demo_${name} PRIVATE cxx_std_20)
endfunction(example)


example(playlist_selector)
example(end_2_end_playlist)
example(music_selector)

message(${CMAKE_SOURCE_DIR})
# Copy configuration files and python files
add_custom_command(
  TARGET yay_tui_demo_end_2_end_playlist POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/config
    ${CMAKE_SOURCE_DIR}/build/config
)

add_custom_command(
  TARGET yay_tui_demo_end_2_end_playlist POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/src/py_wraps/wrappers.py
    ${CMAKE_SOURCE_DIR}/build/demo_ui/wrappers.py
)