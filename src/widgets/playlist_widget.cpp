#include <vector>   
#include <string>
#include <format>

#include <ftxui/component/component.hpp> // For menu
#include <ftxui/component/component_options.hpp> // For MenuOptions
#include <ftxui/component/component_base.hpp>  // for ComponentBase

#include "playlist_widget.hpp"
#include "song_widget.hpp"

using namespace ftxui;
using namespace std;

namespace {

Component Window(std::string title, Component component) {
  return Renderer(component, [component, title] {  //
    return window(text(title), component->Render()) | flex;
  });
}

const int TITLE_SPACING = 40;
const int ALBUM_SPACING = 25;
const int ARTIST_SPACING = 25;
const int RUNTIME_SPACING = 6;

}

namespace widgets {
    Component songlist(vector<track_info>* tracks, int* selected) {
        Components snames;
        for (track_info & track : *tracks) {
            snames.push_back(MenuEntry(format(
                "{0:{1}.{1}} | {2:{3}.{3}} | {4:{5}.{5}} | {6:{7}.{7}}",
                track.title, TITLE_SPACING,
                track.album, ALBUM_SPACING,
                track.artist, ARTIST_SPACING,
                format("{}:{:02}", track.mins, track.secs), RUNTIME_SPACING
            )));
        }

        Component menu = Window(
            format("  {0:^{1}} | {2:^{3}} | {4:^{5}} | {6:^{7}}", 
                "Song", TITLE_SPACING,
                "Album", ALBUM_SPACING,
                "Artist", ARTIST_SPACING,
                "Runtime", RUNTIME_SPACING
            ),
            Container::Vertical(snames, selected)
        );
        return menu;
    }
}