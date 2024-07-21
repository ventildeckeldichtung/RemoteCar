#include <WiFi.h>
#include <esp_now.h>
#define LEFTFORWARD 12
#define LEFTREVERSE 13
#define RIGHTFORWARD 27
#define RIGHTREVERSE 14
#define ENA 32
#define ENB 33
#define LEDLEFT 15
#define LEDRIGTH 16


typedef struct struct_message {
    int xVal;
    int yVal;
    int bVal;
    boolean leftLED;
    boolean rightLED;
} struct_message;

struct_message myData;
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  //Serial.println("incomingData");
  commands();
}
void setup(){
  pinMode(LEFTFORWARD, OUTPUT);
  pinMode(LEFTREVERSE, OUTPUT);
  pinMode(RIGHTFORWARD, OUTPUT);
  pinMode(RIGHTREVERSE, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(LEDLEFT, OUTPUT);
  pinMode(LEDRIGHT, OUTPUT);
  Serial.begin(115200);
  Serial.println();
  WiFi.mode(WIFI_STA);
  Serial.println(WiFi.macAddress());
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
   //Register the Callback Function
    esp_now_register_recv_cb(OnDataRecv);
    
}
void commands() { 
  //Serial.print(myData.xVal); Serial.print(" "); Serial.print(myData.yVal); Serial.println("  ");
  if((myData.xVal > 115 && myData.xVal < 123) && (myData.yVal > 115 && myData.yVal < 123) ) {
    
    stop();
  } else if ((myData.xVal > 125 ) && (myData.yVal > 115 && myData.yVal < 123)) {
    forward();
  } else if ((myData.xVal < 110 ) && (myData.yVal > 115 && myData.yVal < 123)) {
    reverse();
  } else if ((myData.xVal > 115 && myData.xVal < 123) && (myData.yVal > 124 )) { 
    right();
  } else if ((myData.xVal > 115 && myData.xVal < 123) && (myData.yVal < 110 )) {
    left();
  }
}
void loop(){


}
void forward() {
    digitalWrite(RIGHTREVERSE, LOW);
    analogWrite(ENB,0);

    digitalWrite(LEFTREVERSE, LOW);
    analogWrite(ENA, 0);

    digitalWrite(RIGHTFORWARD, HIGH);
    analogWrite(ENB,200);

    digitalWrite(LEFTFORWARD, HIGH);
    analogWrite(ENA, 200);
}
void reverse() {
    digitalWrite(RIGHTFORWARD, LOW);
    analogWrite(ENB,0);

    digitalWrite(LEFTFORWARD, LOW);
    analogWrite(ENA, 0);

    digitalWrite(RIGHTREVERSE, HIGH);
    analogWrite(ENB,200);

    digitalWrite(LEFTREVERSE, HIGH);
    analogWrite(ENA, 200);
}
void left() {
    Serial.println("LEFT");
    digitalWrite(RIGHTREVERSE, LOW);
    analogWrite(ENB,0);

    digitalWrite(LEFTREVERSE, LOW);
    analogWrite(ENA, 0);

    digitalWrite(RIGHTFORWARD, LOW);
    analogWrite(ENB,0);

    digitalWrite(LEFTFORWARD, HIGH);
    analogWrite(ENA, 200);
}
void right() {
    Serial.println("RIGHT");
    digitalWrite(RIGHTREVERSE, LOW);
    analogWrite(ENB,0);

    digitalWrite(LEFTREVERSE, LOW);
    analogWrite(ENA, 0);

    digitalWrite(RIGHTFORWARD, HIGH);
    analogWrite(ENB,200);

    digitalWrite(LEFTFORWARD, LOW);
    analogWrite(ENA, 0);
}
void stop() {
    Serial.println("STOP");
    digitalWrite(RIGHTREVERSE, LOW);
    analogWrite(ENB,0);

    digitalWrite(LEFTREVERSE, LOW);
    analogWrite(ENA, 0);
    digitalWrite(RIGHTFORWARD, LOW);
    analogWrite(ENB,0);

    digitalWrite(LEFTFORWARD, LOW);
    analogWrite(ENA, 0);  
}
