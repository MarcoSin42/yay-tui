#include <iostream>
#include <vector>
#include <string>

#include <ftxui/screen/screen.hpp>
#include <ftxui/component/screen_interactive.hpp> // For Component
#include <ftxui/component/component.hpp>           // for Menu
#include <ftxui/component/component_options.hpp>   // for MenuOption

#include <Python.h>

#include "music_structs.hpp"
#include "widgets/playlist_selector_widget.hpp"
#include "py_wraps/python_wrappers.hpp"


using namespace ftxui;
using namespace std;
using namespace widgets;
using namespace pyapi;

int main() {
    setenv("PYTHONPATH",".", 1);
    Py_Initialize();

    vector<playlist_info> entries = get_playlists();

    auto screen = ScreenInteractive::TerminalOutput();
    int selected = 0;
    auto menu = playlist_selector_widget(&entries, &selected);
    
    menu |= CatchEvent([&](Event event) {
        bool ret = (ftxui::Event::Character('\n') == event);
        if (ret) 
            screen.ExitLoopClosure()();
        return ret;
    });

    screen.Loop(menu);
    

    return EXIT_SUCCESS;
}