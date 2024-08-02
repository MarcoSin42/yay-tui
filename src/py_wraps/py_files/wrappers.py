from ytmusicapi import YTMusic
import os

def get_playlist():
    ytm = YTMusic("../config/oauth.json")    
    chan_ID = open("../config/channel_id", "r").read()
    
    result:list[str] = ytm.get_user(chan_ID)
    
    out = []
    for key in result['playlists']['results']:
        out.append(key['title'])
    return out


if __name__ == '__main__':
    print(get_playlist())