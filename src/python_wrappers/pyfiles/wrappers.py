from ytmusicapi import YTMusic
import os


def get_playlist():
    ytm = YTMusic("config/oauth.json")    
    chan_ID = open("config/channel_id", "r").read()
    
    result = ytm.get_user(chan_ID)
    
    return