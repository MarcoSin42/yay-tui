#include <ftxui/component/screen_interactive.hpp>  // for Component,ScreenInteractive
#include <ftxui/component/component_base.hpp>  // for ComponentBase

#include <vector>
#include "music_structs.hpp"


#ifndef PLAYLIST_SELCTOR_WIDGET_H
#define PLAYLIST_SELCTOR_WIDGET_H
using namespace ftxui;
using namespace std;

namespace widgets {
    Component playlist_selector_widget(vector<playlist_info>* playlists, int* selected);
}


#endif

