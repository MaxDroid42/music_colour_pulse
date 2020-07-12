import sounddevice as sd
import numpy as np
import serial
import time
import random
import requests

#IP address or hostname of e.g. esp8266
serverIP="HOST"
r,g,b=255,20,50    #pink
#r,g,b=255,157,0    #orange
#r,g,b=0,251,255   #light-blue
#reduce traffic, if sound is off or constantly at full volume
lastVolume=-1.0     #initial value is negativ so if initial volume is 0 one request will be sent

def sendVolume(indata, outdata, frames, time, status):
    global lastVolume
    volumeNorm=np.linalg.norm(indata)*0.3               #"sensitivity"
    volumeNorm=volumeNorm if volumeNorm < 1 else 1      #Clipping at full volume / max volume
    volumeNorm=volumeNorm if volumeNorm > 0.008 else 0  #Threshold for silence
    if lastVolume == volumeNorm:
        return
    lastVolume=volumeNorm
    print(volumeNorm)
    payload={
        #colour-value * 4 to map rgb-values (0-255) to esp8266 analogWrite-values (0-1024)
        "r":r*4*volumeNorm,
        "g":g*4*volumeNorm,
        "b":b*4*volumeNorm
    }
    req=requests.post(f"http://{serverIP}/colour", params=payload)
    #print(req.status_code)

#continues loop reading out the audio-stream
with sd.Stream(callback=sendVolume):
    while True:
        response = input()
        if response in ('', 'q', 'Q'):
            break
