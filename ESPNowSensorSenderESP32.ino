#include <esp_now.h>
#include <WiFi.h>

#define ID 1
#define TYPE 1 // Bodenfeuchte
// define TYPE 2 // Lufttemperatur/Luftfeuchte/Helligkeit
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  60       /* Time ESP32 will go to sleep (in seconds) */

uint8_t receiverAddress[] = {0xEC, 0xFA, 0xBC, 0x8A, 0xB3, 0xD0};
uint8_t senderAddress[] = {0,0,0,0,0,0};

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

typedef struct struct_inMessage {
  int data1=0;
  int data2=0;
  int data3=0;
  int data4=0;
  int data5=0;
  int data6=0;
  int data7=0;
  int data8=0;
} struct_inMessage;

struct_inMessage outMessage;
struct_inMessage inMessage;

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("\r\nLast Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&senderAddress, mac, sizeof(senderAddress));
  if (senderAddress == receiverAddress){
    memcpy(&inMessage, incomingData, sizeof(inMessage));
    Serial.print(inMessage.data1);
    Serial.print(inMessage.data2);
    Serial.print(inMessage.data3);
    Serial.print(inMessage.data4);
    Serial.print(inMessage.data5);
    Serial.print(inMessage.data6);
    Serial.print(inMessage.data7);
    Serial.print(inMessage.data8);
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
  esp_now_register_recv_cb(OnDataRecv);

}

void loop() {
  
  esp_now_send(0, (uint8_t *) &outMessage, sizeof(outMessage));
  
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  delay(5000);
  esp_deep_sleep_start();
}
