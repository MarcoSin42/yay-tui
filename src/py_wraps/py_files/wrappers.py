from ytmusicapi import YTMusic
import os

def get_playlists():
    ytm = YTMusic("../config/oauth.json")    
    #chan_ID = open("../config/channel_id", "r").read()
    
    playlists = ytm.get_library_playlists()
    
    out = []
    for key in playlists:
        out.append((
            key['title'],
            key['playlistId']
            ))
    return out[:-1]


"""Gets a list of songs
returns a list containing a tuple of a tuple for the following form:
song_name, artist, song_id, album, mins, seconds
"""
def get_songs(playlist_id: str):
    ytm = YTMusic("../config/oauth.json")    
    #chan_ID = open("../config/channel_id", "r").read()
    
    tracks = ytm.get_playlist(playlist_id)['tracks']
    
    out = []
    for key in tracks:
        mins,secs = key['duration'].split(':')
        out.append((
            key['title'],
            key['artists'][0]['name'],
            key['videoId'],
            key['album']['name'],
            int(mins),
            int(secs)
        ))
    return out

if __name__ == '__main__':
    print(get_playlists())

    print(get_songs("PLFk-2zvjcx0Cs-wFipOSzSwPbFSO4Q_KA")[0])