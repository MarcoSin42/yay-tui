#include "player_widget.hpp"
#include <format>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>
#include <string>

using namespace std;
using namespace ftxui;
namespace {
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
}

namespace widgets {

Component player_widget(MpvController &controller, string& current_artist) {
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
    float progress = controller.getPercentPos() / 100;

    return Renderer([&] {
        return vbox({
            // Top Row
            hbox({
                controls->Render(),
                vbox({
                    gaugeRight(progress), // Top row
                    title
                })
            }),
    });
    });
}

}