message("Building Demo")

if(NOT YAY_TUI_BUILD_EXAMPLES)
  return()
endif()

set(EXAMPLES_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message("CWD:" ${CMAKE_CURRENT_SOURCE_DIR})


function(example name)
  add_executable(yay_tui_demo${name} ${name}.cpp)
  target_include_directories(yay_tui_demo${name} PUBLIC ../src/widgets PUBLIC ../include)
  target_link_libraries(yay_tui_demo${name}
    PRIVATE widgets
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component  # Not needed for this example.
  )
  file(RELATIVE_PATH dir ${EXAMPLES_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
  set_property(GLOBAL APPEND PROPERTY YAY_TUI::DEMO ${dir}/${name})
  target_compile_features(yay_tui_demo${name} PRIVATE cxx_std_20)
endfunction(example)


example(playlist)