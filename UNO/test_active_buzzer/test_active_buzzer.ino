#define VIN 9

void setup() {
    pinMode(VIN, OUTPUT);
    digitalWrite(VIN, LOW);
    Serial.begin(9600);
}

void loop() {
   Serial.println("looping...");
 unsigned char i;
   float delay1 = 1; 
   float delay2 = 2; 
 while(1)
 {
   //output an frequency
   Serial.println(delay1);
   for(i=0;i<250;i++)
   {
    digitalWrite(VIN,HIGH);
    delay(delay1);//wait for 1ms
    digitalWrite(VIN,LOW);
    delay(delay1);//wait for 1ms
    }
    //output another frequency
   Serial.println(delay2);
     for(i=0;i<250;i++)
      {
        digitalWrite(VIN,HIGH);
        delay(delay2);//wait for 1ms
        digitalWrite(VIN,LOW);
        delay(delay2);//wait for 1ms
      }
  }
}
