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

#include "widgets/song_list.hpp"

using namespace ftxui;
using namespace std;
using namespace widgets;



int main() {
    song s1,s2,s3;

    s1.name = "The Bow I row";
    s1.album = "Summer salt";
    s1.artist = "Tame Impala";
    s1.minutes = 0;

    s2.name = "Witches";
    s2.album ="Witches";
    s2.artist = "Alice Phoebe Lou";
    s2.minutes = 42;

    s3.name = "Footsteps in the Dark";
    s3.album = "Covers By Cannons";
    s3.artist = "Cannons";
    s3.minutes = 3;


    vector<song> entries = {s1, s2, s3};

    

    auto screen = ScreenInteractive::TerminalOutput();
    int selected = 0;

    auto slist = songlist(&entries, &selected);

    //MenuOption option;
    //option.on_enter = screen.ExitLoopClosure();

    screen.Loop(slist);
    

    return EXIT_SUCCESS;
}