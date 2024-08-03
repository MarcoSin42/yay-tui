#ifndef MUSIC_STRUCT_H
#define MUSIC_STRUCT_H
#include <string>
using namespace std;

struct playlist_info {
    string id;
    string title;
};

struct track_info {
    string id;
    string title;
    string artist;
    string album;
    uint8_t mins;
    uint8_t seconds;
};

#endif