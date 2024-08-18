#include <cstdlib>
#include <ftxui/component/event.hpp>
#include <string>
#include <thread>
#include <vector>

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/component/screen_interactive.hpp> // For Component
#include <ftxui/component/component.hpp>           // for Menu
#include <Python.h>

#include "widgets/song_widget.hpp"
#include "widgets/playlist_widget.hpp"
#include "widgets/player_widget.hpp"

#include "py_wraps/python_wrappers.hpp"
#include "mpv_controller.hpp"

using namespace ftxui;
using namespace std;
using namespace pyapi;
using namespace widgets;

int main() {
    setenv("PYTHONPATH",".", 1);
    Py_Initialize();

    Component playlist_selector;
    Component song_selector;
    Component player_widget;

    string cur_artist = "No Artist";
    MpvController &controller = MpvController::getInstance();
    std::atomic<bool> refresh_ui_continue = true;

    vector<playlist_info> playlists = get_playlists();
    vector<string> playlists_titles;
    vector<vector<track_info>> playlist_songs;

    ScreenInteractive screen = ScreenInteractive::Fullscreen();

    // Retrieve playlist info from youtube
    int selected_song = 0;
    int selected_playlist = 0;

    for (playlist_info & plist : playlists) {
        playlist_songs.push_back(get_songs(plist.id));
        playlists_titles.push_back(plist.title);
    }

    Components playlist_widgets;
    for (vector<track_info> & tracks : playlist_songs) {
        playlist_widgets.push_back(playlist_widget(&tracks, &selected_song));
    }
    // ^^^^ Retrieve playlist info from youtube ^^^^

    playlist_selector = Menu(&playlists_titles, &selected_playlist);
    song_selector = Container::Tab(playlist_widgets, &selected_playlist);
    player_widget = widgets::PlayerWidget();

    
    Component component = Container::Vertical({
    Container::Horizontal({playlist_selector, song_selector,}), // Top row
    player_widget // Bottom row
    });

    auto focus_y = [&]() -> float {
        return (float)selected_song / (float)playlist_songs[selected_playlist].size();
    };

    Component component_renderer = Renderer(component, [&] {
        return vbox({
            hbox({
                    playlist_selector->Render(),
                    separator(),
                    song_selector->Render() | focusPositionRelative(0, focus_y()) | frame,}) 
                        |  yflex_shrink | border,
            player_widget->Render() | xflex_grow | size(HEIGHT, ftxui::EQUAL, 6)
        });
    });

    component_renderer |= CatchEvent([&](Event event) {
        bool ret = false; 

        if (ftxui::Event::Character('\n') == event) {
            ret = true;
            controller.loadFile("https://www.youtube.com/watch?v=" + playlist_songs[selected_playlist][selected_song].id);
            controller.setCurrentArtist(playlist_songs[selected_playlist][selected_song].artist);
        }

        if (ftxui::Event::Escape == event) {
            ret = true;
            refresh_ui_continue = false;
            std::this_thread::sleep_for(0.2s);
            screen.ExitLoopClosure()();
        }

        return ret;
    });

    std::thread refresh_ui([&] {
        while (refresh_ui_continue) {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(0.05s);
            screen.Post(Event::Custom);
        }
        //std::terminate();
    });
    

    screen.Loop(component_renderer);
    refresh_ui.join();

}