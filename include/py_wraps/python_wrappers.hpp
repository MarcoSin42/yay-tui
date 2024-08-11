#ifndef PYTHON_WRAPPER_H
#define PYTHON_WRAPPER_H
#include <vector>
#include <string>
#include "music_structs.hpp"

namespace pyapi {

vector<playlist_info> get_playlists();
vector<track_info> get_songs(std::string playlist_id);
}

#endif