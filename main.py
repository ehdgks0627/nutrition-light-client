import RPi.GPIO as gpio
import requests
import json


class LightManager:
    HOST = "http://naver.com"
    deviceID = "1234"
    itemPK = -1
    lightColor = (128, 64, 200)
    lightStrength = 50
    PIN_W_sensor = 28

    def updateState(self):
        try:
            data = {"deviceID": self.deviceID}
            response = requests.post(self.HOST, data=data)
            if response.status_code != 200:
                print("[-] ServerError...")
            else:
                try:
                    body = json.loads(response.text)
                    if body["state"] == "OK":
                        #TODO
                        pass
                    else:
                        print("[-] FormatError...")
                except:
                    print("[-] FormatError...")
        except requests.exceptions.ConnectionError:
            print("[-] NetworkError...")

    def getW(self):
        #TODO
        pass

    def __init__(self, mode=gpio.BCM):
        gpio.setmode(mode)
        gpio.setup(self.PIN_W_sensor, gpio.IN)

    def __str__(self):
        return "LightManager@%s"%(deviceID)

    def __del__(self):
        gpio.cleanup()


def main():
    try:
        manager = LightManager()
        manager.updateState()
    except KeyboardInterrupt:
        pass

if __name__ == "__main__":
    main()
