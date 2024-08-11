#ifndef MPV_CONTROLLER_H
#define MPV_CONTROLLER_H
#include <ctime>
#include <mpv/client.h>
#include <string>

using namespace std;

class MpvController
{
    
    private:
        mpv_handle *m_Handle;

        // Required for a dumb fucking hack
        time_t last_loaded_ms;

        void checkError(int status);
        MpvController();

    public:
        static MpvController& getInstance();

        // Get information about the player.  Should not mutate the state at all.
        string getTitle();
        bool isPlaying();
        float getPercentPos();
        float getTimeElapsed_s(); // Returns playback time in seconds

        void loadFile(string fileOrUrl);

        // Playback controls
        void togglePause();
        void seek(int secs);
        void seekTo(string timeStamp);
        void playlistNext();
        void playlistPrev();
        void stop();
        void setVol(int percent);
        void volUp(int percent);
        void volDown(int percent);

        ~MpvController();

        // Singleton stuff
        MpvController(MpvController const&)            = delete;
        MpvController& operator=(MpvController const&) = delete;

};
#endif