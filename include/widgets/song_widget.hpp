#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/string.hpp>
#include <ftxui/dom/node.hpp>
#include "music_structs.hpp"

#ifndef SONGWIDGET_H
#define SONGWIDGET_H
using namespace ftxui;

namespace widgets {
Element songwidget(std::string name, std::string artist, std::string album);
Element songwidget(track_info s);
}


#endif