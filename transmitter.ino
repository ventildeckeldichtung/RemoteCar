#include <esp_now.h>
#include <WiFi.h>


#define VRX_PIN  34
#define VRY_PIN  32 
#define SW_PIN   27
#define LED_PIN 13
#define BUTTON_GREEN 17
#define BUTTON_RED 4


int xValue = 0;
int yValue = 0;
int bValue = 0; 
int flag = 0;


// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x10,0x06,0x1C,0x80,0xAA,0x1C};  //34:85:18:26:D7:34   ESP32-C3 Device1
//uint8_t broadcastAddress[] = {0x34,0x85,0x18,0x26,0xC6,0x88};  //34:85:18:26:C6:88   ESP32-C3 Device2

//uint8_t broadcastAddress[] = {0xC8,0xC9,0xA3,0xC9,0x14,0xEC};  //C8:C9:A3:C9:14:EC   ESP32-Wroom


// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  int xVal;
  int yVal;
  int bVal;
  boolean redbtn;
  boolean greenbtn;
   
} struct_message;

// Create a struct_message called myData & peer
struct_message myData;
esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  //Serial.print("\r\nLast Packet Send Status:\t");
  //Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  //status == ESP_NOW_SEND_SUCCESS ? digitalWrite(LED_PIN, HIGH) : digitalWrite(LED_PIN, LOW);
}
 
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  //button.setDebounceTime(100);
  pinMode(34, OUTPUT);
  pinMode(VRX_PIN, INPUT);
  pinMode(VRY_PIN, INPUT);
  pinMode(SW_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_GREEN, INPUT);
  pinMode(BUTTON_RED, INPUT);
  myData.xVal = 0;
  myData.yVal = 0;
  myData.bVal = 0;
  myData.greenbtn = LOW;
  myData.redbtn = LOW;
  digitalWrite(34, HIGH);
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {
  // Make values to send
  //button.loop(); // MUST call the loop() function first
  
  int currentValx = analogRead(VRX_PIN);
  int currentValy = analogRead(VRY_PIN);
  int buttunval = analogRead(SW_PIN);

  
  // read analog X and Y analog values
  myData.xVal = map(analogRead(VRX_PIN),0,4095,0,255);
  myData.yVal = map(analogRead(VRY_PIN),0,4095,0,255);
  myData.bVal = map(analogRead(SW_PIN), 0,4095,0,255);
  myData.greenbtn = digitalRead(BUTTON_GREEN);
  myData.redbtn = digitalRead(BUTTON_RED);
  int deltaX = abs(currentValx - myData.xVal);
  int deltaY = abs(currentValy - myData.yVal);

  Serial.print("X Value: ");Serial.print(myData.xVal);Serial.print("   Y Value: ");Serial.print(myData.yVal);Serial.println(myData.bVal);

  
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
    digitalWrite(LED_PIN, HIGH);
  }
  else {
    Serial.println("Error sending the data");
    digitalWrite(LED_PIN, LOW);
  }
  
  delay(50);

}
