#include <esp_now.h>
#include <WiFi.h>

#define ID 1
#define TYPE 1 // Bodenfeuchte
// define TYPE 2 // Lufttemperatur/Luftfeuchte/Helligkeit
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  600       /* Time ESP32 will go to sleep (in seconds) */

uint8_t receiverAddress[] = {0xEC, 0xFA, 0xBC, 0x8A, 0xB3, 0xD0};
esp_now_peer_info_t peerInfo;


typedef struct struct_outMessage {
  int id = ID; // must be unique for each sender board
  int type = TYPE;
  int data1=0;
  int data2=0;
  int data3=0;
  int data4=0;
  int data5=0;
  int data6=0;
  int data7=0;
  int data8=0;
} struct_outMessage;

struct_outMessage outMessage;

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("\r\nLast Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}
void initESP(){
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  //WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  } 
  // Register peer
  memcpy(peerInfo.peer_addr, receiverAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

void setup() {
  Serial.begin(115200);
  initESP();
  

}

void loop() {
  
  esp_now_send(0, (uint8_t *) &outMessage, sizeof(outMessage));
  delay(3000);
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
}
