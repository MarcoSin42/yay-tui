#include <ftxui/component/screen_interactive.hpp>  // for Component,ScreenInteractive
#include <ftxui/component/component_base.hpp>  // for ComponentBase

#include <vector>
#include "music_structs.hpp"


#ifndef PLAYLIST_WIDGET_H
#define PLAYLIST_WIDGET_H
using namespace ftxui;

namespace widgets {
    Component playlist_widget(vector<track_info>* songs, int* selected);
}


#endif