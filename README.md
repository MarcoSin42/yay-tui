Yet Another YouTube Music Terminal User Interface

Because a browser is too much for a music player.
# Setting up ytmusicapi

You can install ytmusicapi using the following command:

    pip install ytmusicapi


Note, if you're using Linux you may need to add `/home/<Your Username>/.local/bin` to your `$PATH`. so you can run `ytmusicapi`.  

Then, you need to obtain an API key so you can use the YouTube API.  If you used your default Python installation you can run: 

    ytmusicapi oauth

You should have `oauth.json` in your current working directory.

Now move it to the `/config` folder in the `/yay-tui` directory.  


# Building and running

Make the `/build` directory if it doesn't already exist in `/yay-tui`.

**Make sure** you have `oauth.json` in your config directory, more specifically `/yay-tui/config`.

Then, run: `./run.sh`.

# Demo 

https://github.com/user-attachments/assets/775eb66f-8ea5-4e06-bf7e-5a8d235f8213

# Controls

FTXUI uses vim-like bindings, that is.  


`left := h`

`down := j`

`up := k`

`left := l`


To play use the cursor and place it over the song you want and press `Enter`.



# Requirements
The following must be installed:

- [yt-dlp](https://github.com/yt-dlp/yt-dlp)
- [mpv](https://mpv.io/)
- [ytmusicapi](https://github.com/sigma67/ytmusicapi)
- Some sort of nerd-font.  Note, you may have to configure your editor in order to view it, for example, I had to add my nerd font in vscode so I could see it in my editor.


# TODO 

- Rewrite parts of the YTMusicapi in pure C++
- Add playlist support
- Add caching
- Improve UI
- Create installer
