#include <string>

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/string.hpp>
#include <ftxui/component/component_base.hpp>     // for ComponentBase
#include <ftxui/component/component_options.hpp>  // for MenuOption, MenuEntryOption,
#include <ftxui/component/component.hpp>  // for Make, Menu, MenuEntry, Toggle

#include <ftxui/dom/elements.hpp>

#include "widgets.hpp"

using namespace ftxui;
using namespace std;
namespace widgets {
    Element songwidget(string name, string artist, string album) {
        // sname  | artist | album
        auto content = hbox({
            text(name) | center,
            separator(),
            text(album)| center | flex,
            separator(),
            text(artist) | center | align_right,
        });

        return content;
    }


}