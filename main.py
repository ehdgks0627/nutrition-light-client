import RPi.GPIO as gpio
import requests
import json


class LightManager:
    HOST = "http://a.com"
    deviceID = "1234"
    itemPK = -1
    lightColor = (128, 64, 200)
    lightStrength = 50
    PIN = {"consumption": 28, "temperature": 29}

    def updateState(self):
        data = {"deviceID": self.deviceID, "consumption": readSensor("consumption"), "temperature": readSensor("temperature")}
        response = requests.pos(HOST + "/StateDevice", data=data)
        if response.status_code != 200:
            print("[-] ServerError...")
        else:
            body = json.loads(response.text)
            if body["state"] == "OK":
                #TODO
                #controlDevice(body["item"])
                pass
            else:
                print("[-] FormatError...")

    def readSensor(self, command):
        if command not in PIN:
            print("%s is not in PIN MAP"%(command))
            return
        if command == "consumption":
            pass
        elif command == "temperature":
            pass
        #TODO
        pass

    def controlDevice(self, item):
        item["amount"]
        item["temperature"]
        item["time"]
        pass

    def __init__(self, mode=gpio.BCM):
        gpio.setmode(mode)
        gpio.setup(PIN["W"], gipo.IN)

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
