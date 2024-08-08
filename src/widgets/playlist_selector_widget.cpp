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

}

namespace widgets {
    Component playlist_selector_widget(vector<playlist_info>* playlists, int* selected) {
        Components pnames;
        for (playlist_info & plist : *playlists) {
            pnames.push_back(MenuEntry(format(
                "{0:{1}.{1}}",
                plist.title, TITLE_SPACING
            )));
        }

        Component menu = Window(
            format("  {0:^{1}}", 
                "Song", TITLE_SPACING
            ),
            Container::Vertical(pnames, selected)
        );
        return menu;
    }
}