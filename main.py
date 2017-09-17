import RPi.GPIO as gpio
import requests
import json

class LightManager:
    HOST = "http://ss5h.namsu.xyz:9940"
    deviceID = "12345678"
    itemPK = -1
    lightColor = (128, 64, 200)
    lightStrength = 50
    PIN = {"W_sensor": 29, "LED_3000K": 31, "LED_4500K": 32, "LED_6000K": 33, "LED_10000K": 35, "LED_20000K": 36, "LED_30000K": 37, "consumption": 38, "temperature": 40}
    PWM = {}
    token = ""
    session = None

    def SyncState(self):
        try:
            data = {"deviceID": self.deviceID, "consumption": self.ReadSensor("consumption"), "temperature": self.ReadSensor("temperature")}
            response = self.session.post(self.HOST + "/devices/state/", data=data)
            if response.status_code != 200:
                print("[-] ServerError...")
            else:
                body = json.loads(response.text)
                print(body)
                #TODO
        except requests.exceptions.ConnectionError:
            print("[-] NetworkError...")


    def ReadSensor(self, command):
        return 1.3
        if command not in self.PIN:
            print("%s is not in PIN MAP"%(command))
            return
        if command == "consumption":
            pass
        elif command == "temperature":
            pass
        #TODO
        pass


    def ControlDevice(self, item):
        def ControlLED(LED_3000, LED_4500, LED_6000, LED_10000, LED_20000, LED_30000):
            pass
        item["amount"]
        item["time"]
        pass


    def RegisterDevice(self):
        response = self.session.post(self.HOST + "/devices/register/", data={"deviceID": self.deviceID})
        if response.status_code == 200 or response.status_code == 400:
            print("[+] Register Success")
        else:
            print("[-] ServerError...")

    def GetCpuTemperature(self):
        process = Popen(['vcgencmd', 'measure_temp'], stdout=PIPE)
        output, _error = process.communicate()
        return float(output[output.index('=') + 1:output.rindex("'")])"'")])


    def __init__(self, mode=gpio.BOARD):
        self.session = requests.Session()
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
        return "<LightManager@{}>".format(self.deviceID)


    def __del__(self):
        gpio.cleanup()


def main():
    try:
        manager = LightManager()
        manager.RegisterDevice()
        manager.SyncState()
    except KeyboardInterrupt:
        pass

if __name__ == "__main__":
    main()
