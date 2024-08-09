#include <iostream>
#include <vector>
#include <string>

#include <ftxui/screen/screen.hpp>
#include <ftxui/component/screen_interactive.hpp> // For Component
#include <ftxui/component/component.hpp>           // for Menu, Tab, Horizontal, 
#include <ftxui/component/component_options.hpp>   // for MenuOption

#include <Python.h>

#include "music_structs.hpp"
#include "playlist_widget.hpp"
#include "python_wrappers.hpp"


using namespace ftxui;
using namespace std;
using namespace pyapi;
using namespace widgets;

int main() {
    setenv("PYTHONPATH",".", 1);
    Py_Initialize();

    vector<playlist_info> playlists = get_playlists();
    vector<string> playlists_titles;
    vector<vector<track_info>> playlist_songs;

    cout << "Playlist size: " << playlists.size() << endl;
    for (playlist_info & plist : playlists) {
        playlist_songs.push_back(get_songs(plist.id));
        playlists_titles.push_back(plist.title);
    }

    Components playlist_widgets;
    int selected_song = 0;
    int selected_playlist = 0;
    for (vector<track_info> & tracks : playlist_songs) {
        playlist_widgets.push_back(playlist_widget(&tracks, &selected_song));
    }

    auto playlist_selector = Menu(&playlists_titles, &selected_playlist);

    auto song_selector = Container::Tab(playlist_widgets, &selected_playlist);

    
    auto component = Container::Horizontal({
      playlist_selector,
      song_selector,
    });

    auto focus_y = [&]() -> float {
        return (float)selected_song / (float)playlist_songs[selected_playlist].size();
    };
    
    // Add some separator to decorate the whole component:
    auto component_renderer = Renderer(component, [&] {
        return hbox({
                playlist_selector->Render(),
                separator(),
                song_selector->Render() | focusPositionRelative(0, focus_y()) | frame,
            }) |
            border;
    });

    auto screen = ScreenInteractive::FitComponent();
    screen.Loop(component_renderer);
    

    return EXIT_SUCCESS;
}