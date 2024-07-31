#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/string.hpp>
#include <ftxui/dom/node.hpp>
#include "song_list.hpp"

#ifndef SONGWIDGET_H
#define SONGWIDGET_H
using namespace ftxui;
using namespace std;

namespace widgets {
Element songwidget(string name, string artist, string album);
Element songwidget(song s);



class Song
{
private:
    string name;
    string artist;
    string album;
public:
    Song(string name, string artist, string album);
    string ToString();
};

}


#endif