#include <ftxui/component/screen_interactive.hpp>  // for Component,ScreenInteractive
#include <ftxui/component/component_base.hpp>  // for ComponentBase

#include <vector>
#include <string>
#include "music_structs.hpp"


#ifndef SONGLIST_H
#define SONGLIST_H
using namespace ftxui;
using namespace std;

namespace widgets {
    Component songlist(vector<track_info>* songs, int* selected);
}


#endif