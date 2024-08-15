#include <ftxui/component/component.hpp> // For Vertical
#include <ftxui/dom/elements.hpp> // For gauge
#include <ftxui/component/screen_interactive.hpp> // For screeninteractive

#include <cstdio>
#include <ftxui/dom/node.hpp>

#include "mpv_controller.hpp"

#include <mpv/client.h>
#include <string>
#include <unistd.h>

using namespace ftxui;



int main(int argc, char *argv[])
{
    MpvController& controller = MpvController::getInstance();
    ScreenInteractive screen = ScreenInteractive::FitComponent();

    if (argc != 2) {
        controller.loadFile("https://www.youtube.com/watch?v=dQw4w9WgXcQ"); // Load our test mp4
    } else {
        controller.loadFile(argv[1]); // Load our user's file
    }

    std::atomic<bool> refresh_ui_continue = true;
    std::thread refresh_ui([&] {
        while (refresh_ui_continue) {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(0.05s);

        screen.Post(Event::Custom);

        if (!controller.isPlaying())
            screen.ExitLoopClosure()();
        }
    });

    using namespace std;
    Component media_controls = Container::Horizontal({
        Button("-5s", [&] {controller.seek(-5);}),
        Button("Prev", [&] {controller.playlistPrev();}),
        Button("Play/Pause", [&] { controller.togglePause();}),
        Button("Next", [&] {controller.playlistNext();}),
        Button("+5s", [&] {controller.seek(+5);}),
        Button("Stop", [&] {controller.stop();}),
        Button("Vol +5%", [&] {controller.volUp(5);}),
        Button("Vol -5%", [&] {controller.volDown(5);}),
        Button("Mute", [&] {controller.toggleMute();}),
    }) | center;

    Component component = Renderer(media_controls,[&] {

        float progress_percent = controller.getPercentPos();
        string time_elapsed = controller.getTimeElapsed_hh_mm_ss();
        float seconds_elapsed = controller.getTimeElapsed_s();
        float volume = controller.getVol();
        bool ismuted = controller.isMuted();

        float dur_s = controller.getDuration_s();
        string dur_hh_mm_ss = controller.getDuration_hh_mm_ss();

        return vbox({
            text("Title: " + controller.getMediaTitle()),
            gaugeRight(progress_percent / 100) | border,
            text(format("{:04.2f}% | {} / {} | {:5.2f} / {:0.2f} | Vol: {:3.0f} | muted: {:5}",
             progress_percent, time_elapsed, dur_hh_mm_ss, seconds_elapsed, dur_s, volume, ismuted)),
             separator(),
            //media_controls->Render()
        }) | border;
    });
 
    screen.Loop(component);
    refresh_ui_continue = false;
    refresh_ui.join();
    
    return 0;
}