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

using namespace ftxui;
using namespace std;
using namespace widgets;


int main() {
    track_info s1,s2,s3;

    s1.title = "The Bow I row";
    s1.album = "Summer salt";
    s1.artist = "Tame Impala";
    s1.mins = 1;
    s1.secs = 54;

    s2.title = "Witches";
    s2.album ="Witches";
    s2.artist = "Alice Phoebe Lou";
    s2.mins = 3;
    s2.secs = 2;

    s3.title = "Footsteps in the Dark";
    s3.album = "Covers By Cannons";
    s3.artist = "Cannons";
    s3.mins = 3;
    s3.secs = 32;


    vector<track_info> entries = {s1, s2, s3};

    

    auto screen = ScreenInteractive::TerminalOutput();
    int selected = 0;

    auto slist = songlist(&entries, &selected);

    //MenuOption option;
    //option.on_enter = screen.ExitLoopClosure();

    screen.Loop(slist);
    

    return EXIT_SUCCESS;
}