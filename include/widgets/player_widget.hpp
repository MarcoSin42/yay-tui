#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>  // for Component,ScreenInteractive
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>

#include <string>
#include "mpv_controller.hpp"

#ifndef PLAYER_WIDGET_H
#define PLAYER_WIDGET_H

namespace {
    class PlayerWidgetBase : public ftxui::ComponentBase {
    private:
        ftxui::Component prevBtn;
        ftxui::Component togglePlayPauseBtn;
        ftxui::Component nextBtn;

        ftxui::Component volDownBtn;
        ftxui::Component volUpBtn;
        ftxui::Component muteBtn;

        ftxui::Component volControls;
        ftxui::Component mediaControls;
        ftxui::Component controls;

        std::string currentArtist;


    
    public:
        PlayerWidgetBase(); 
        ftxui::Element Render() override;
        bool OnEvent(ftxui::Event) override;

    private:

        std::string getMuteStatusIcon(bool);
        std::string getPlayStatusIcon(bool);
    

};
};


namespace widgets {


ftxui::Component PlayerWidget();


}
#endif