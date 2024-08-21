#include "mpv_controller.hpp"


#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <chrono>
#include <ctime>
#include <format>
#include <mpv/client.h>
#include <stdexcept>
#include <string>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <spawn.h>

#include <sys/wait.h>

struct mpv_opt {
    const char *option;
    const char *setting;
} typedef MpvOption;

using std::string, std::runtime_error, std::format;


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


    MpvOption options[] = {
        {"video", "no"},
        {"try_ytdl_first", "no"},
        {"ytdl_path", "/usr/bin/yt-dlp"},
        {"volume", "60"},
        {"ytdl-format", "worstvideo+bestaudio"},
        {"profile", "low-latency"},
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
    string result = "No media playing";
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
    bool recently_loaded = (now_ms - m_last_loaded_ms) < 10000; // for our purposes, <10000ms is 'recent' yt-dl is slow

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
    //std::lock_guard<std::mutex> lock_guard(m_file_available);

    int rv = mpv_command_string(m_Handle, format("loadfile {}", fileOrUrl).c_str());
    if (rv < 0)
        throw runtime_error(format("Unable to load: {} | rv = {}", fileOrUrl, rv));

    using namespace std::chrono;
    // Time since unix epoch in milliseconds; Required for a dumb hack
    m_last_loaded_ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    
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
    mpv_command_string(m_Handle, "playlist-next");

    using namespace std::chrono;
    m_last_loaded_ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void MpvController::playlistPrev() {
    mpv_command_string(m_Handle, "playlist-prev");

    using namespace std::chrono;
    m_last_loaded_ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
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
    mpv_command_string(m_Handle, format("add volume {}", percent).c_str());
}

void MpvController::volDown(int percent) {
    mpv_command_string(m_Handle, format("add volume -{}", percent).c_str());

}


float MpvController::getVol() {
    char *vol_str = mpv_get_property_string(m_Handle, "volume");
    if (!vol_str)
        //throw runtime_error("Unable to get volume");
        return  0.0f;

    float result = atof(vol_str);
    mpv_free(vol_str);

    return result;
}

void MpvController::toggleMute() {
    mpv_command_string(m_Handle, "cycle mute");
}

bool MpvController::isMuted() {
    char *mute_status = mpv_get_property_string(m_Handle, "mute");
    if (!mute_status)
        throw runtime_error("Unable to get mute status");

    bool muted = strcmp(mute_status, "no");
    mpv_free(mute_status);

    return muted;
}

float MpvController::getDuration_s() {
    char *dur_s = mpv_get_property_string(m_Handle, "duration");
    float result = 0.0f;
    if (!dur_s)
        return result;

    result = atof(dur_s);
    mpv_free(dur_s);

    return result;
}

string MpvController::getDuration_hh_mm_ss() {
    char *dur_hh_mm_ss = mpv_get_property_osd_string(m_Handle, "duration");
    string result = "00:00:00";
    if (!dur_hh_mm_ss)
        return result;

    result = string(dur_hh_mm_ss);
    mpv_free(dur_hh_mm_ss);

    return result;
}

bool MpvController::isPaused() {
    char *pause_status = mpv_get_property_string(m_Handle, "paused");
    if (!pause_status)
        return false;
        //throw runtime_error("Unable to get pause status");

    bool paused = strcmp("pause", "yes");
    mpv_free(pause_status);

    return paused;
}

// Unfortunately MPV does not support artist information
void MpvController::setCurrentArtist(string artist) {m_currentArtist = artist;}
string MpvController::getCurrentArtist() {return m_currentArtist;}

inline string getFileName(string videoID) {
    return "/tmp/" + videoID + ".mp4";
}

inline bool file_exists(string file) {
    struct stat buffer;
    return (stat(getFileName(file).c_str(), &buffer) == 0);
}

inline string getYTURL(string videoID) {
    return ytBaseURL + videoID;
}
void MpvController::stream_yt_dlp(string videoID)
{
	int pid;
    int file_fd = open(getFileName(videoID).c_str(), O_WRONLY | O_CREAT | O_TRUNC | O_SYNC | O_CLOEXEC, 0666);

    posix_spawn_file_actions_t action;
    posix_spawn_file_actions_init(&action);
    posix_spawn_file_actions_addopen(&action, STDERR_FILENO, "/dev/null", O_WRONLY | O_APPEND , 0); // ERRORS?  WE DON'T NEED NO STINKING ERRORS!
    posix_spawn_file_actions_adddup2(&action, file_fd, STDOUT_FILENO);
    posix_spawn_file_actions_addclose(&action, STDIN_FILENO);

    // This is fairly ugly and I'm not happy with it, but I see no other way
    char *argv[] = {(char*)"yt-dlp",(char *)0, (char*)"--quiet", (char*)"-o", (char*)"-", (char*)0};
    argv[1] = (char *)malloc(getYTURL(videoID).size() + 1);
    strcpy(argv[1], getYTURL(videoID).c_str());

    if (posix_spawnp(&pid, argv[0], &action, NULL, argv, NULL)) {
        perror("spawn");
        throw runtime_error("Failed to spawn posix process");
    }

    posix_spawn_file_actions_destroy(&action);
    free(argv[1]);
}

void MpvController::stream(string videoID) {
    if (file_exists(videoID)) {
        loadFile(getFileName(videoID));
        return;
    }
    /** 
    THERE IS AN IMPLICIT GUARANTEE THAT YOUTUBE WILL SEND A VALID VIDEO ID
    IF YOU PUT RANDOM STUFF THAT ISN'T DIRECTLY FROM THE YT-API, THERE IS NO GUARANTEE THIS WILL WORK! 
    AND WILL POTENTIALLY CRASH THIS PROGRAM.  THIS IS NOT ROBUST.

    TODO: FIX and make robust
    */
    
    stream_yt_dlp(videoID);
    loadFile(format("appending://{}", getFileName(videoID)));
    
}

void MpvController::setTitle(string title) {
    mpv_set_property_string(m_Handle, "force-media-title", title.c_str());
}



