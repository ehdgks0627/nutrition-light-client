import RPi.GPIO as gpio
import requests
import json


class LightManager:
    HOST = "http://naver.com"
    deviceID = "1234"
    itemPK = -1
    lightColor = (128, 64, 200)
    lightStrength = 50
    PIN = {"W_sensor": 28, "LED_3000K": 1, "LED_4500K": 2, "LED_6000K": 3, "LED_10000K": 4, "LED_20000K": 5, "LED_30000K": 6}
    PWM = {}
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
        W = GPIO.input(self.PIN["W_sensor"]) #TODO
        return W

    def driveLED(self):
        #TODO
        pass

    def __init__(self, mode=gpio.BCM):
        gpio.setmode(mode)
        gpio.setup(self.PIN["W_sensor"], gpio.IN)
        for key, value in self.PIN.items():
            if key.startswith("LED_"):
                gpio.setup(value, gpio.OUT)
                self.PWM[key] = gpio.PWM(value, 100)
                self.PWM[key].start(50)

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
