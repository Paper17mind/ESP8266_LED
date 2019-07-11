/*
 *  This sketch demonstrates how to scan WiFi networks. 
 *  The API is almost the same as with the WiFi Shield library, 
 *  the most obvious difference being the different file you need to include:
 *
 *  Modified by :papermindvention 11 july 2019
 */
#include <ESP8266WiFi.h>
char ValStr[75];
char* ssid;
String ins;

void setup() {
  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  Serial.println("Setup done");
  scan();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED){
    Serial.println("Disconnect, You are now Offline");
    delay(1000);
    Serial.println("try to connecting......");
    scan();
  }
}

void scan(){
  Serial.println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      if (WiFi.encryptionType(i) == ENC_TYPE_NONE){
        ins += WiFi.SSID(i);
        for (int i=0; i< ins.length(); i++){ ValStr[i] = ins.charAt(i); }
        ssid = strtok(ValStr,".");
        WiFi.begin(ssid);
        Serial.print("Connecting To : ");
        Serial.println(ssid);  
        while (WiFi.status() != WL_CONNECTED) {
          Serial.print('.');
          delay(500);
        }
        Serial.println("WiFi connected");  
          Serial.println("IP address: ");
          Serial.println(WiFi.localIP());
      ins = "";
      break;
      }
      delay(10);
    }
  }
  Serial.println("");

  // Wait a bit before scanning again
  delay(5000);
}
