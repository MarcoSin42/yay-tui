#ifndef MPV_CONTROLLER_H
#define MPV_CONTROLLER_H
#include <ctime>
#include <mpv/client.h>
#include <string>


class MpvController
{
    
    private:
        mpv_handle *m_Handle;
        std::string m_currentArtist;

        // Required for a dumb fucking hack
        time_t m_last_loaded_ms;

        void checkError(int status);
        MpvController();

    public:
        static MpvController& getInstance();

        // Get information about the player.  Should not mutate the state at all.
        std::string getTitle();
        std::string getMediaTitle();
        bool isPlaying();
        float getPercentPos(); // Returns how much is left to play (0-100) as a percentage
        float getTimeElapsed_s(); // Returns playback time in seconds
        std::string getTimeElapsed_hh_mm_ss(); // Returns playback in hh:mm:ss format
        float getDuration_s(); // Gets the estimated length of the file
        std::string getDuration_hh_mm_ss(); // Gets the length of the file in hh:mm:ss format as a string
        float getVol(); // Gets the volume range(0-130); you may want to clamp it to 100.
        bool isMuted(); 
        bool isPaused();
        std::string getCurrentArtist();

        void loadFile(std::string fileOrUrl); // Either specify a path to file locally or it may query YouTube via YT-DLP.  

        // Playback controls
        void togglePause();
        void seek(int secs);
        void seekTo(std::string timeStamp);
        void playlistNext();
        void playlistPrev();
        void stop();
        void setVol(int percent);
        void volUp(int percent);
        void volDown(int percent);
        void toggleMute();

        void setCurrentArtist(std::string artist);

        ~MpvController();

        // Singleton stuff
        MpvController(MpvController const&)            = delete;
        MpvController& operator=(MpvController const&) = delete;

};
#endif