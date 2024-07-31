from ytmusicapi import YTMusic
import os

if __name__ == '__main__':
    ytm = YTMusic("config/oauth.json")    
    chan_ID = open("config/channel_id", "r").read()
    
    
    result = ytm.get_user(chan_ID)
    
    
    #home = ytm.get_user_playlists(chan_ID, result["playlists"]["params"])
    
    #print(chan_ID)
    
    for key in result['playlists']['results']:
        print("="*20)
        print(key)
        print("-"*20)
        
        print(key['title'])
        print('\n')
    
    
    #print(home)