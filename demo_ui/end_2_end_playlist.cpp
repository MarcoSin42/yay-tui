#include <iostream>
#include <vector>
#include <string>
#include <format>

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/string.hpp>
#include <ftxui/component/screen_interactive.hpp> // For Component
#include <ftxui/component/component.hpp>           // for Menu
#include <ftxui/component/component_options.hpp>   // for MenuOption

#include "music_structs.hpp"
#include "widgets/playlist_widget.hpp"
#include "py_wraps/python_wrappers.hpp"

using namespace ftxui;
using namespace std;
using namespace widgets;


int main() {
    vector<track_info> entries = get_playlists();

    auto screen = ScreenInteractive::TerminalOutput();
    int selected = 0;

    auto slist = songlist(&entries, &selected);

    //MenuOption option;
    //option.on_enter = screen.ExitLoopClosure();

    screen.Loop(slist);
    

    return EXIT_SUCCESS;
}