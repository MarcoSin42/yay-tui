#include <ftxui/screen/screen.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>

#include "mpv_controller.hpp"
#include "widgets/player_widget.hpp"

using namespace std;
using namespace ftxui;
string getMuteStatusIcon(bool isMuted) {
    if (isMuted)
        return "   ";

    return "   ";
}

string getPlayStatusIcon(bool isPaused) {
    if (isPaused)
        return "  ";

    return "  ";
}

Component mycontrols() {
    MpvController &controller = MpvController::getInstance();

    Component media_controls = Container::Horizontal({
        Button("  ", [&] {controller.playlistPrev();}),
        Button(getPlayStatusIcon(controller.isPaused()), [&] {controller.togglePause();}),
        Button("  ", [&] {controller.playlistNext();}),
    });

    Component volume_controls = Container::Horizontal({
        Button("  ", [&] {controller.volUp(5);}),
        Button("   ", [&] {controller.volDown(-5);}),
        Button(getMuteStatusIcon(controller.isMuted()), [&] {controller.toggleMute();}),
    });

    Component controls = Container::Vertical({media_controls, volume_controls});

    return controls;
}


int main() {
    // Components
    ScreenInteractive screen = ScreenInteractive::FixedSize(100, 10);
    MpvController& controller = MpvController::getInstance();

    string current_artist("Test");
    Component widge = widgets::player_widget(current_artist);

    Component prevBtn = Button("  ", [&] {controller.playlistPrev();});
    Component togglePlayPauseBtn = Button(getPlayStatusIcon(controller.isPaused()), [&] {controller.togglePause();});
    Component nextBtn = Button("  ", [&] {controller.playlistNext();});

    Component playback_controls = Container::Horizontal({
        prevBtn,
        togglePlayPauseBtn,
        nextBtn,
    });

    Component volDownBtn = Button("  ", [&] {controller.volUp(5);});
    Component volUpBtn = Button("   ", [&] {controller.volDown(-5);});
    Component muteBtn = Button(getMuteStatusIcon(controller.isMuted()), [&] {controller.toggleMute();});

    Component volume_controls = Container::Horizontal({
        volDownBtn,
        volUpBtn,
        muteBtn,
    });

    Component controls = Container::Vertical({
        playback_controls, 
        volume_controls
    });
        
    // Components done

    int raw_playback_s = (int) controller.getTimeElapsed_s();
    int playback_ss = raw_playback_s % 60;
    int playback_mm = raw_playback_s / 60;

    int raw_dur_s = (int) controller.getDuration_s();
    int dur_ss = raw_dur_s % 60;
    int dur_mm = raw_dur_s / 60;
    
    Element playback_time = text(
        format("{:2}:{:02} / {:2}:{:02}",
            playback_mm, playback_ss,
            dur_mm, dur_ss
        )
    );

    Element title = text(
        format("Title: {:25} | By: {:15}", controller.getMediaTitle(), current_artist)
    );
    auto progress = [&]() -> float {return controller.getPercentPos() / 100;};


    screen.Loop(Renderer( controls,
    [&] {
        Element controlGrid = gridbox({
            {prevBtn->Render(), togglePlayPauseBtn->Render(), nextBtn->Render()}, // Top row
            {volDownBtn->Render(), volUpBtn->Render(), muteBtn->Render()} // Bottom Row
        });
        Element title = text(
            format("Title: {:25} | By: {:15}", controller.getMediaTitle(), current_artist)
        );

        auto progress = [&]() -> float {return (controller.getPercentPos() + 50) / 100;};

        return hbox({
            controlGrid,
            vbox({
                title,
                gaugeRight(progress()),
            })

            
        }) | border;
    }));
    return 0;
}