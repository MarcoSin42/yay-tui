#include <vector>
#include <string>

#include <ftxui/screen/screen.hpp>
#include <ftxui/component/screen_interactive.hpp> // For Component
#include <ftxui/component/component.hpp>           // for Menu
#include <ftxui/component/component_options.hpp>   // for MenuOption

#include <Python.h>

#include "music_structs.hpp"
#include "widgets.hpp"
#include "python_wrappers.hpp"

using namespace ftxui;
using namespace std;
using namespace widgets;
using namespace pyapi;

/*
Tests My Python API
*/

int main() {
    setenv("PYTHONPATH", ".", 1);
    Py_Initialize();

    string playlist_id = "PLFk-2zvjcx0Cs-wFipOSzSwPbFSO4Q_KA";
    vector<track_info> songs = get_songs(playlist_id);

    int selected = 0;
    auto song_menu = playlist_widget(&songs, &selected);

    ScreenInteractive screen = ScreenInteractive::TerminalOutput();

    song_menu |= CatchEvent([&](Event event) {
        bool ret = (ftxui::Event::Character('\n') == event);
        if (ret) 
            screen.ExitLoopClosure()();

        return ret;
    });

    screen.Loop(song_menu);
    cout << "Selected: " << songs[selected].title << endl;

    Py_FinalizeEx();

    return EXIT_SUCCESS;
}