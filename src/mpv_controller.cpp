#include "mpv_controller.hpp"
#include <cstdlib>
#include <cstring>
#include <chrono>
#include <ctime>
#include <format>
#include <mpv/client.h>
#include <stdexcept>
#include <string>
#include <sys/stat.h>

struct mpv_opt {
    const char *option;
    const char *setting;
} typedef MpvOption;

void MpvController::checkError(int status) {
    if (status < 0) {
        printf("mpv API error: %s\n", mpv_error_string(status));
        exit(1);
    }
}

MpvController::MpvController() {
    setlocale(LC_NUMERIC, "C");
    m_Handle = mpv_create();
    if (!m_Handle)
        throw runtime_error("Unable to create mpv handle");

    //checkError(mpv_set_option_string(m_Handle, "input-default-bindings", "yes"));
    //int val = 1;
    //checkError(mpv_set_option(m_Handle, "osc", MPV_FORMAT_FLAG, &val));
    MpvOption options[] = {
        {"video", "no"},
        {"try_ytdl_first", "yes"},
        {"ytdl_path", "/usr/bin/yt-dlp"},
        {"volume", "60"},
        {"ytdl-format", "bestaudio"},
        {"ytdl", "yes"},
        {"keep-open","yes"},
        {"input-vo-keyboard", "yes"}
    };

    for (MpvOption & an_option : options) {
        //printf("Option: %10s | set-to : %10s\n", an_option.option, an_option.setting);
        mpv_set_option_string(m_Handle, an_option.option, an_option.setting);
    }

    if (mpv_initialize(m_Handle) < 0)
        throw runtime_error("Unable to initialize mpv");
}

MpvController& MpvController::getInstance() {
    static MpvController the_one;

    return the_one;
}


MpvController::~MpvController() {
    mpv_terminate_destroy(m_Handle);
}

string MpvController::getTitle() {
    string result = "";
    char *title_c = mpv_get_property_string(m_Handle, "filename");
    if (!title_c)
        return result;

    result = string(title_c);
    mpv_free(title_c);

    return result;
}

string MpvController::getMediaTitle() {
    string result = "";
    char *title_c = mpv_get_property_string(m_Handle, "media-title");
    if (!title_c)
        return result;

    result = string(title_c);
    mpv_free(title_c);

    return result;
}

bool MpvController::isPlaying() {
    char *eof_status = mpv_get_property_string(m_Handle, "eof-reached");

    /*
    This is a dumb hack.
    ------------------
    If you call isPlaying() immediately or close to immediately after loading
    a file.  isPlaying() will return false.  This is to prevent that.
    */
    using namespace std::chrono;
    time_t now_ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    bool recently_loaded = (now_ms - last_loaded_ms) < 10000; // for our purposes, <100ms is 'recent' 

    if (!eof_status) 
        return recently_loaded;

    bool is_playing = !strcmp(eof_status, "no");
    mpv_free(eof_status);

    return is_playing;
}

// Between 0-100
float MpvController::getPercentPos() {
    char *percent_str = mpv_get_property_string(m_Handle, "percent-pos");
    float result = 0.0f;
    if (!percent_str)
        return result;

    result = atof(percent_str);
    mpv_free(percent_str);

    return result;
}

// Returns seconds elapsed
float MpvController::getTimeElapsed_s() {
    char *time_pos = mpv_get_property_string(m_Handle, "time-pos");
    float result = 0.0f;
    if (!time_pos)
        return result;

    result = atof(time_pos);
    mpv_free(time_pos);

    return result;
}

// Returns playback time in human readable format
string MpvController::getTimeElapsed_hh_mm_ss() {
    char *time_pos = mpv_get_property_osd_string(m_Handle, "time-pos");
    string result = "00:00:00";
    if (!time_pos)
        return result;

    result = string(time_pos);
    mpv_free(time_pos);

    return result;
}

void MpvController::loadFile(string fileOrUrl) {
    int rv = mpv_command_string(m_Handle, format("loadfile {}", fileOrUrl).c_str());

    if (rv < 0)
        throw runtime_error(format("Unable to load: {} | rv = {}", fileOrUrl, rv));

    using namespace std::chrono;
    // Time since unix epoch in milliseconds; Required for a dumb hack
    last_loaded_ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    
}

void MpvController::togglePause() {
    int rv = mpv_command_string(m_Handle, "cycle pause");
    if (rv < 0)
        throw runtime_error("For some inexplicable reason you can't play nor pause");
}

void MpvController::seek(int secs) {
    int rv = mpv_command_string(m_Handle, format("seek {}", secs).c_str());
    
    if (rv < 0)
        throw runtime_error("MPV was unable to relatively seek");
}

void MpvController::seekTo(string timeStamp) {
    int rv = mpv_command_string(m_Handle, format("seek {} absolute", timeStamp).c_str());
    
    if (rv < 0)
        throw runtime_error("MPV was unable to absolutely seek");
}


void MpvController::playlistNext() {
    int rv = mpv_command_string(m_Handle, "playlist-next");
    /*
    if (rv < 0)
        throw runtime_error("Unable to goto next in playlist");
    */

    using namespace std::chrono;
    last_loaded_ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void MpvController::playlistPrev() {
    int rv = mpv_command_string(m_Handle, "playlist-prev");
    /*
    if (rv < 0)
        throw runtime_error("Unable to goto previous in playlist");
    */

    using namespace std::chrono;
    last_loaded_ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}


void MpvController::stop() {
    int rv = mpv_command_string(m_Handle, "stop");

    if (rv < 0)
        throw runtime_error("Unable to stop");
}

void MpvController::setVol(int percent) {
    int rv = mpv_command_string(m_Handle, format("set volume {}", percent).c_str());

    if (rv < 0)
        throw runtime_error("Unable to set volume");
}

void MpvController::volUp(int percent) {
    int rv = mpv_command_string(m_Handle, format("add volume {}", percent).c_str());

    if (rv < 0)
        throw runtime_error("Unable to increment volume");
}

void MpvController::volDown(int percent) {
    int rv = mpv_command_string(m_Handle, format("add volume -{}", percent).c_str());

    if (rv < 0)
        throw runtime_error("Unable to decrement volume");
}




