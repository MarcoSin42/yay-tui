Yet Another YouTube Music Terminal User Interface

Because a browser is too much for a music player.

# Building

Simply run `build.sh` you may need to make the `build` directory.


# Requirements

You need YTmusicapi.  You can install it by executing the following:

    pip install ytmusicapi


Note, if you're using Linux you may need to add `/home/\<Your Username>/.local/bin` to your `$PATH.`

Then, you need to obtain an API key so you can use the YouTube API.  If you used your default python installation you can run: 

    ytmusicapi oauth

You should have `oauth.json` in your current working directory.

Move it to the `/config` folder in the `/yay-tui` directory.  

# TODO 

- Rewrite parts of the YTMusicapi in pure C++
