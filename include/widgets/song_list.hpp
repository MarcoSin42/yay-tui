#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/string.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/component/screen_interactive.hpp>  // for Component,ScreenInteractive
#include <ftxui/component/component_base.hpp>  // for ComponentBase

#include <vector>
#include <string>


#ifndef SONGLIST_H
#define SONGLIST_H
using namespace ftxui;
using namespace std;

namespace widgets {
    struct song {
        string name;
        string artist;
        string album;
        uint8_t seconds;
        uint8_t minutes;
    };

    Component songlist(vector<song>* songs, int* selected);
}


#endif