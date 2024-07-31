cmake_minimum_required (VERSION 3.24)

project(yay-tui
  LANGUAGES CXX
  VERSION 1.0.0
  DESCRIPTION "A terminal-user interface for YouTube Music, because a browser is too much for a music player."
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- Fetch FTXUI --------------------------------------------------------------
include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
set(FETCHCONTENT_QUIET OFF)

FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/arthursonzogni/ftxui.git
  GIT_TAG        v5.0.0
  GIT_PROGRESS   TRUE
  GIT_SHALLOW    TRUE
  EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(ftxui)
# ------------------------------------------------------------------------------

find_package(Python3 REQUIRED Development)


add_executable(yay-tui src/main.cpp)
target_include_directories(yay-tui PRIVATE src PRIVATE include)

add_library(widgets
  include/widgets/song_widget.hpp
  include/widgets/song_list.hpp
  src/widgets/song_widget.cpp
  src/widgets/song_list.cpp
)

add_library(python_wrapper
  include/python_wrappers/python_wrappers.hpp
  src/python_wrappers/python_wrappers.cpp
)

target_include_directories(widgets PRIVATE src/widgets PRIVATE include)
target_include_directories(python_wrapper PRIVATE src/python_wrappers PRIVATE include)

target_link_libraries(python_wrapper Python3::Python)

target_link_libraries(widgets
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component  # Not needed for this example.
)

target_link_libraries(yay-tui
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component  # Not needed for this example.
  PUBLIC widgets
)

