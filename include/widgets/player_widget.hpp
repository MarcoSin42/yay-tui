#include <ftxui/component/screen_interactive.hpp>  // for Component,ScreenInteractive
#include <string>
#include "mpv_controller.hpp"

#ifndef PLAYER_WIDGET_H
#define PLAYER_WIDGET_H

namespace widgets {
    ftxui::Component player_widget(std::string& current_artist);
}

#endif