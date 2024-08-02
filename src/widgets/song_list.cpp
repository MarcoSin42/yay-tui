#include <vector>   // for vector, __alloc_traits<>::value_type
#include <string>

#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp> // For menu
#include <ftxui/component/component_options.hpp> // For MenuOptions
#include <ftxui/component/screen_interactive.hpp>  // for Component,ScreenInteractive
#include <ftxui/component/component_base.hpp>  // for ComponentBase

#include <format>

#include "widgets/song_list.hpp"
#include "widgets/song_widget.hpp"

using namespace ftxui;
using namespace std;

Component Window(std::string title, Component component) {
  return Renderer(component, [component, title] {  //
    return window(text(title), component->Render()) | flex;
  });
}

namespace widgets {
    Component songlist(vector<song>* songs, int* selected) {
        Components snames;
        for (song & s : *songs) {
            snames.push_back(MenuEntry(format(
                "{:<25} | {:<25} | {:<25} | {:^8}",
                s.name, s.album, s.artist, s.minutes
            )));
        }

        Component menu = Window(
            format("  {:^25} | {:^25} | {:^25} | {:^8}", "Song", "Album","Artist", "Runtime"),
            Container::Vertical(snames, selected)
        );
        return menu;
    }
}