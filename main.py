import RPi.GPIO as gpio
import requests
import json


class LightManager:
    HOST = "http://ss5h.namsu.xyz:9940"
    deviceID = "12345"
    itemPK = -1
    lightColor = (128, 64, 200)
    lightStrength = 50
    PIN = {"W_sensor": 38, "LED_3000K": 29, "LED_4500K": 31, "LED_6000K": 33, "LED_10000K": 35, "LED_20000K": 36, "LED_30000K": 37, "consumption": 39, "temperature": 40}
    PWM = {}

    def syncState(self):
        try:
            data = {"device": self.deviceID, "consumption": self.readSensor("consumption"), "temperature": self.readSensor("temperature")}
            response = requests.post(self.HOST + "/devices/state", data=data)
            print(response.text)
            print(response.status_code)
            if response.status_code != 200:
                print("[-] ServerError...")
            else:
                body = json.loads(response.text)
                if body["state"] == "OK":
                    #TODO
                    pass
        except requests.exceptions.ConnectionError:
            print("[-] NetworkError...")

    def readSensor(self, command):
        if command not in self.PIN:
            print("%s is not in PIN MAP"%(command))
            return
        if command == "consumption":
            pass
        elif command == "temperature":
            pass
        #TODO
        pass

    def controlDevice(self, item):
        def controlLED(LED_3000, LED_4500, LED_6000, LED_10000, LED_20000, LED_30000):
            pass
        item["amount"]
        item["time"]
        pass

    def registerDevice(self):
        response = requests.post(self.HOST + "/devices/register/", data={"key": self.deviceID})
        if response.status_code != 200 or response.status_code != 400:
            print("[-] ServerError...")
        else:
            print("[+] Register Success")

    def __init__(self, mode=gpio.BOARD):
        gpio.setmode(mode)
        gpio.setup(self.PIN["W_sensor"], gpio.IN)
        gpio.setup(self.PIN["consumption"], gpio.IN)
        gpio.setup(self.PIN["temperature"], gpio.IN)
        for key, value in self.PIN.items():
            if key.startswith("LED_"):
                gpio.setup(value, gpio.OUT)
                self.PWM[key] = gpio.PWM(value, 100)
                self.PWM[key].start(50)

    def __str__(self):
        return "LightManager@%s"%(self.deviceID)

    def __del__(self):
        gpio.cleanup()


def main():
    try:
        manager = LightManager()
        manager.registerDevice()
        manager.syncState()
    except KeyboardInterrupt:
        pass

if __name__ == "__main__":
    main()
