#include <esp_now.h>
#include <WiFi.h>

// MAC addresses of the two ESP32 devices
uint8_t receiverMacAddress[] = {0xa8, 0x42, 0xe3, 0x45, 0x94, 0x68};
esp_now_peer_info_t peerInfo;

// Callback function to handle received data
void OnDataRecv(const uint8_t *macAddr, const uint8_t *data, int len) {
  // Handle received data
  
  // Print received message
  Serial.print("Received message: ");
  Serial.println((char*)data);
}

void fillRandomBytes(uint8_t *array, size_t length) {
  for (size_t i = 0; i < length - 1; i++) { // Leave space for the null terminator
    array[i] = random(0, 10) + '0'; // Generates a random digit and converts it to its ASCII representation
  }
  array[length - 1] = '\0'; // Null-terminate the array
}

void setup() {
  // Initialize Serial and Wi-Fi
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

    // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Register peer
  memcpy(peerInfo.peer_addr, receiverMacAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // Send message via ESP-NOW
  uint8_t senderData[9];
  fillRandomBytes(senderData,8);
  Serial.println((char*)senderData);
  esp_err_t result = esp_now_send(receiverMacAddress, senderData, sizeof(senderData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
 delay(10000);
}