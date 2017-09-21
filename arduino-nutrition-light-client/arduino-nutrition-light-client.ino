#define MATH_PI          3.141592653589793
#define MATH_E           2.718281828459045

#define PIN_LED_3000K    1
#define PIN_LED_4500K    2
#define PIN_LED_6000K    3
#define PIN_LED_10000K   4
#define PIN_LED_20000K   5
#define PIN_LED_30000K   6
#define PIN_Consumption  7
#define PIN_Temperature  8

#define HOST             "http://ss5h.namsu.xyz:9940"
#define deviceID         "12345678910"

#define WIFI_SSID         "AD"
#define WIFI_PASS         "1234"

double CalLED(double temperature, double amount, double x)
{
  return ((5.0*amount/2.0)/math.sqrt(2.0*math.pi))*pow(math.e,(-((x-temperature)**2.0)/300000.0))
}

double double_map(double x, double in_min, double in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

double map_temperature_to_range(double temperature)
{
  if(temperature < 3000)
  {
    return 0;
  }
  else if(3000 <= temperature && temperature < 4500)
  {
    return double_map(temperature, 3000, 4500, 0, 1000);
  }
  else if(4500 <= temperature && temperature < 6000)
  {
    return double_map(temperature, 4500, 4500, 1000, 2000);
  }
  else if(6000 <= temperature && temperature < 10000)
  {
    return double_map(temperature, 6000, 10000, 2000, 3000);
  }
  else if(10000 <= temperature && temperature < 20000)
  {
    return double_map(temperature, 10000, 20000, 3000, 4000);
  }
  else if(20000 <= temperature && temperature < 30000)
  {
    return double_map(temperature, 20000, 30000, 4000, 5000);
  }
  else if(30000 <= temperature)
  {
    return 5000;
  } 
}

void setup() {
  pinMode(PIN_LED_3000K, OUTPUT);
  pinMode(PIN_LED_4500K, OUTPUT);
  pinMode(PIN_LED_6000K, OUTPUT);
  pinMode(PIN_LED_10000K, OUTPUT);
  pinMode(PIN_LED_20000K, OUTPUT);
  pinMode(PIN_LED_30000K, OUTPUT);
  pinMode(PIN_Consumption, INPUT);
  pinMode(PIN_Temperature, INPUT);
  //RegisterDevice
  //POST HOST + "/devices/register/", data = "deviceID":deviceID

}

void syncState()
{
  
}

void loop() {
  // put your main code here, to run repeatedly:
  double W = analogRead(PIN_Consumption);
  double T = analogRead(PIN_Temperature);
  syncState();
  delay(5000);
}
