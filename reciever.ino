/*
  ESP-NOW Demo - Receive
  esp-now-demo-rcv.ino
  Reads data from Initiator
  
  DroneBot Workshop 2022
  https://dronebotworkshop.com
*/

// Include Libraries
#include <esp_now.h>
#include <WiFi.h>








uint8_t broadcastAddress[] = {0x08,0xD1,0xF9,0xED,0x0B,0x4C};
// Define a data structure
typedef struct struct_message {
  char a;
  int dist;
} struct_message;
esp_now_peer_info_t peerInfo;
// Create a structured object
struct_message myData;
// Callback function executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  //Serial.write(myData.a);
 // Serial.print("Data received: ");
  //Serial.println(len);
  //Serial.print("Character Value: ");
  Serial.println(myData.a);
  Serial.write(myData.a+48);
  //Serial.print("distance Value: ");
 // Serial.println(myData.dist);
 
 

  //Serial.println();
}

void setup() {
  // Set up Serial Monitor
  Serial.begin(9600);
  Serial2.begin(9600);
   
  WiFi.mode(WIFI_STA);
  

 
  
// Set ESP32 as a Wi-Fi Station
  
  // Initilize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
    // Register the send callback
 
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  // Register callback function
  esp_now_register_recv_cb(OnDataRecv);
}  


void loop() {
  
  if(Serial2.available())
  {
    //Serial.print("\n");
    Serial.write(Serial2.read());
  }
  //myData.dist = distance;



}
