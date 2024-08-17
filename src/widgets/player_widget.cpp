//#include "player_widget.hpp"

#include <format>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>

#include <string>

#include "widgets/player_widget.hpp"
#include "mpv_controller.hpp"

using namespace std;
using namespace ftxui;
namespace widgets {

string PlayerWidgetBase::getMuteStatusIcon(bool isMuted) {
    if (isMuted)
        return "   ";

    return "   ";
}

string PlayerWidgetBase::getPlayStatusIcon(bool isPaused) {
    if (isPaused)
        return "  ";

    return "  ";
}


PlayerWidgetBase::PlayerWidgetBase() {
    //cout << "Instantiate\n";
    prevBtn = Button("  ", [&] {controller.playlistPrev();});
    togglePlayPauseBtn = Button(PlayerWidgetBase::getPlayStatusIcon(controller.isPaused()), [&] {controller.togglePause();});
    nextBtn = Button("  ", [&] {controller.playlistNext();});

    mediaControls = Container::Horizontal({
        prevBtn,
        togglePlayPauseBtn,
        nextBtn,
    });

    volDownBtn = Button("  ", [&] {controller.volDown(5);});
    volUpBtn = Button("   ", [&] {controller.volUp(5);});
    muteBtn = Button(PlayerWidgetBase::getMuteStatusIcon(controller.isMuted()), [&] {controller.toggleMute();});

    volControls = Container::Horizontal({
        volDownBtn,
        volUpBtn,
        muteBtn,
    });

    controls = Container::Vertical({
        mediaControls, 
        volControls
    });

    title = text(
        format("Title: {:25} | By: {:15}", "No media playing", "N/A")
    );
    
};

Element PlayerWidgetBase::Render() {
    //return text("Placeholder");
    auto progress = [&]() -> float {return controller.getPercentPos() / 100;};

    title = text(
        format("Title: {:35} | By: {:25}", controller.getMediaTitle(), controller.getCurrentArtist())
    ) | borderRounded;

    int raw_playback_s = (int) controller.getTimeElapsed_s();
    int playback_ss = raw_playback_s % 60;
    int playback_mm = raw_playback_s / 60;

    int raw_dur_s = (int) controller.getDuration_s();
    int dur_ss = raw_dur_s % 60;
    int dur_mm = raw_dur_s / 60;
    
    playback_time = text(
        format("{:2}:{:02} / {:2}:{:02} ",
            playback_mm, playback_ss,
            dur_mm, dur_ss
        )
    );


    volume = text(format(" {:3}% ", controller.getVol())) | borderRounded;

    Element gridbox = ftxui::gridbox({
        {
            prevBtn->Render(),
            togglePlayPauseBtn->Render(),
            nextBtn->Render()
        },
        {
            volDownBtn->Render(),
            volUpBtn->Render(),
            muteBtn->Render()
        }
    });

    progress_bar =  gaugeRight(progress()) | borderRounded | xflex_grow; 

    return hbox({
        gridbox, 
        vbox({
            title | xflex_grow,
            hbox({ volume, progress_bar, playback_time | borderRounded}) | xflex_grow
        }) | xflex_grow
    }) | xflex_grow;
}

bool PlayerWidgetBase::OnEvent(ftxui::Event event) {
    controls->OnEvent(event);

    return false;
}






ftxui::Component PlayerWidget() {
    return ftxui::Make<PlayerWidgetBase>();
}

}


