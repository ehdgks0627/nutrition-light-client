void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  pinMode(3, OUTPUT);
  analogWrite(3, 250);
}

void loop() {
  if(Serial.available())
  {
    Serial2.write(Serial.read());
  }
  if(Serial2.available())
  {
    Serial.write(Serial2.read());
  }
  /*
  if(Serial.available())
  {
    delay(100);
    String command = "";
    while (Serial.available())
    {
      command += Serial.read() - 0x30;
    }
    analogWrite(3, command.toInt());
    Serial.println(String(command.toInt()));
  }*/
}
