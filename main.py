import RPi.GPIO as gpio
import requests
import json


class LightManager:
    HOST = "http://a.com"
    deviceID = "1234"
    itemPK = -1
    lightColor = (128, 64, 200)
    lightStrength = 50
    PIN_W = 28
    PIN_

    def updateState(self):
        data = {"deviceID": self.deviceID}
        response = requests.pos(HOST, data=data)
        if response.status_code != 200:
            print("[-] ServerError...")
        else:
            body = json.loads(response.text)
            if body["state"] == "OK":
                #TODO
                pass
            else:
                print("[-] FormatError...")

    def getW(self):
        #TODO
        pass

    def __init__(self, mode=gpio.BCM):
        gpio.setmode(mode)
        gpio.setup(pin_W, gipo.IN)

    def __str__(self):
        return "LightManager@%s"%(deviceID)

    def __del__(self):
        gpio.cleanup()


def main():
    try:
        manager = LightManager()
    except KeyboardInterrupt:
        pass

if __name__ == "__main__":
    main()
