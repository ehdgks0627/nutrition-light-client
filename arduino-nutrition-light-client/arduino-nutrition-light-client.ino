#include <ArduinoJson.h>

//#define DEBUG
//#define DEBUG_CALL
#define SERIAL_RX_BUFFER_SIZE 256
#define SERIAL_TX_BUFFER_SIZE 256
#define SERIAL_BUFFER_SIZE 256

#define MATH_PI          3.141592653589793
#define MATH_E           2.718281828459045

#define PIN_LED_3000K    2
#define PIN_LED_4500K    3
#define PIN_LED_6000K    4
#define PIN_LED_10000K   5
#define PIN_LED_20000K   6
#define PIN_LED_30000K   7
#define PIN_Consumption  A0
#define PIN_Temperature  A1

#define HOST             String("kafuuchino.moe")
#define PORT             String("9940")
#define deviceID         String("12345678910")

#define WIFI_SSID        String("sprout")
#define WIFI_PASS        String("9223372036854775808")

//library function start

String recv_until(char c)
{
  #ifdef DEBUG_CALL
    Serial.println("recv_until('" + String(c) + "')");
  #endif
  String result;
  char r;
  while (true)
  {
    if ((r = Serial2.read()) == c)
    {
      #ifdef DEBUG_CALL
        Serial.println("recv_until('" + String(c) + "') end");
      #endif
      return result;
    }
    else
    {
      result += r;
    }
  }
  #ifdef DEBUG_CALL
    Serial.println("recv_until('" + String(c) + "') end");
  #endif
}

String recv_until(String c)
{
  #ifdef DEBUG_CALL
    Serial.println("recv_until('" + c + "')");
  #endif
  String result;
  int depth = 0;
  char r;

  while (true)
  {
    if (Serial2.available())
    {
      r = Serial2.read();
      /*Serial.print(int(r));
      Serial.println(" - " + String(r));*/
      if (r == c[depth])
      {
        depth += 1;
        if (depth == c.length())
        {
          #ifdef DEBUG_CALL
            Serial.println("recv_until('" + c + "') end");
          #endif
          return result;
        }
      }
      else
      {
        depth = 0;
        result += r;
      }
    }
  }
  #ifdef DEBUG_CALL
    Serial.println("recv_until('" + c + "') end");
  #endif
}

bool find_string(char* cmd, int msec, char* string)
{
  #ifdef DEBUG_CALL
    Serial.println("find_string('" + String(cmd) + ", " + String(msec) + ", '" + String(string) + "')");
  #endif
  unsigned int finish = millis() + msec;
  Serial2.println(cmd);
  while (millis() < finish)
    if (Serial2.find(string))
    {
      Serial.print("[+] ");
      Serial.print(cmd);
      Serial.println(" OK");
      #ifdef DEBUG_CALL
        Serial.println("find_string('" + String(cmd) + ", " + String(msec) + ", '" + String(string) + "') end");
      #endif
      return true;
    }
  Serial.print("[-] ");
  Serial.print(cmd);
  Serial.println(" FAILURE");
  #ifdef DEBUG_CALL
    Serial.println("find_string('" + String(cmd) + ", " + String(msec) + ", '" + String(string) + "') end");
  #endif
  return false;
}

void send_command(String cmd, int msec = 100)
{
  #ifdef DEBUG_CALL
    Serial.println("send_command('" + cmd + "', " + String(msec) + ")");
  #endif
  Serial2.println(cmd);
  delay(msec);
  #ifdef DEBUG_CALL
    Serial.println("send_command('" + cmd + "', " + String(msec) + ") end");
  #endif
}

void flushing()
{
  #ifdef DEBUG_CALL
    Serial.println("flushing()");
  #endif
  while (Serial2.available())
    Serial2.read();
  #ifdef DEBUG_CALL
    Serial.println("flushing() end");
  #endif
}
//library function end

double CalLED(double temperature, double amount, double x)
{
  return ((5.0*amount/2.0)/sqrt(2.0*MATH_PI))*pow(MATH_E,(-(pow((x-temperature), 2.0))/300000.0));
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

void RegisterDevice()
{
  String content = "deviceID=" + deviceID;
  String data = "POST /devices/register/ HTTP/1.1\r\n";
  data += "Host: " + HOST + "\r\n";
  data += "Content-Type: application/x-www-form-urlencoded\r\n";
  data += "Content-Length: " + String(content.length()) + "\r\n\r\n";
  data += content;
  
  char buf[512];
  sprintf(buf, "AT+CIPSEND=%d", data.length());
  Serial2.println("AT+CIPSTART=\"TCP\",\"" + HOST + "\"," + PORT);
  delay(3000);
  Serial2.println(buf);
  delay(1000);
  Serial2.println(data);
  recv_until("+IPD");
  recv_until("\r\n\r\n");
  String body = recv_until("\r\n");
  Serial.println(body);
  send_command("AT+CIPCLOSE");
  Serial.println("Register Success");
}

void printTime(int msec)
{
  int start = millis();
  while(start + msec > millis())
  {
    if(Serial2.available())
    {
      Serial.write(Serial2.read());
    }
  }
}

void SyncState(double T, double W)
{
  //POST HOST + "/devices/state/", data = "deviceID":deviceID consumption:1.3, temperature:4.6
  String content = "{\"deviceID\": \"" + deviceID + "\", \"consumption\": \"" + String(W) + "\",\"temperature\": \"" + String(T) +"\"}";
  String data = "POST /devices/state/ HTTP/1.1\r\n";
  data += "Host: " + HOST + "\r\n";
  data += "Content-Type: application/json\r\n";
  data += "Content-Length: " + String(content.length()) + "\r\n\r\n";
  data += content;
  
  char buf[512];
  StaticJsonBuffer<2048> jsonBuffer;
  sprintf(buf, "AT+CIPSEND=%d", data.length());
  Serial2.println("AT+CIPSTART=\"TCP\",\"" + HOST + "\"," + PORT);
  delay(3000);
  flushing();
  Serial2.println(buf);
  recv_until('>');
  Serial2.println(data);
  recv_until("+IPD");
  recv_until("+IPD");
  recv_until("\r\n\r\n");
  String body = recv_until("\r\n");
  JsonObject& root = jsonBuffer.parseObject(body);
  Serial.println(body);
  double temperature = (double)map_temperature_to_range((double)root["light"]["temperature"]);
  double amount = (double)(root["light"]["amount"])/2.0;
  double map_base = 0;
  int _3000k = (int)CalLED(temperature, amount, map_base + 0.0);
  int _4500k = (int)CalLED(temperature, amount, map_base + 1000.0);
  int _6000k = (int)CalLED(temperature, amount, map_base + 2000.0);
  int _10000k = (int)CalLED(temperature, amount, map_base + 3000.0);
  int _20000k = (int)CalLED(temperature, amount, map_base + 4000.0);
  int _30000k = (int)CalLED(temperature, amount, map_base + 5000.0);
  Serial.println(_3000k);
  Serial.println(_4500k);
  Serial.println(_6000k);
  Serial.println(_10000k);
  Serial.println(_20000k);
  Serial.println(_30000k);
  analogWrite(PIN_LED_3000K, 255 - _3000k);
  analogWrite(PIN_LED_4500K, 255 - _4500k);
  analogWrite(PIN_LED_6000K, 255 - _6000k);
  analogWrite(PIN_LED_10000K, 255 - _10000k);
  analogWrite(PIN_LED_20000K, 255 - _20000k);
  analogWrite(PIN_LED_30000K, 255 - _30000k);

  
  Serial.println("Synced");
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
  Serial.begin(115200);
  Serial2.begin(115200);
  Serial.println("start");
  find_string("AT", 100, "OK");
  send_command("AT+CWMODE=1");
  send_command("AT+CWQAP", 500);
  String command = "";
  command = "AT+CWJAP=\"";
  command += WIFI_SSID;
  command += "\",\"";
  command += WIFI_PASS;
  command += "\"";
  send_command(command, 11000);
  flushing();
  Serial.println("[+] IP : ");
  send_command("AT+CIFSR", 100);
  Serial.println(recv_until("OK"));
  RegisterDevice();
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);
  double T = (analogRead(PIN_Temperature) * 500.0) / 1024.0;
  double W = (analogRead(PIN_Consumption) * 500.0) / 9.31;
  SyncState(T, W);
}
