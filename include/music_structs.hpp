#ifndef MUSIC_STRUCT_H
#define MUSIC_STRUCT_H
#include <string>
using namespace std;

struct playlist_info {
    string title;
    string id;

};

struct track_info {
    string id;
    string title;
    string artist;
    string album;
    int mins;
    int secs;

    ~track_info(){}
};

#endif