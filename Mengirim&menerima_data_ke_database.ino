/*
 * Membaca data dari Database
 * untuk menjalankan program ini, anda harus mempunyai dulu ID untuk terhubung
 * dengan cara registrasi terlebih dahulu di in-thishub.000webhostapp.com
 * 
 * jika ingin menggunakan host lain, seperti website anda pribadi atau di localhost, rubah string yang terdapat pada host
 * **********************************************************************************************************************
 * created : 20 june 2019
 * for more information, visit =>>> papermindvention.blogspot.com
 */

#include <ESP8266WiFi.h>

const char* ssid     = "Papermind"; // contoh nama ssid
const char* password = "sersan17"; // contoh password ssid

//const char* host = "192.168.43.54";
const char* host = "in-thishub.000webhostapp.com";
const char* myID = ""; //masukkan id kamu, berisi 32 karater

void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  delay(10);
    
  //WiFi.mode(WIFI_STA); //mode wifi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  //mulai koneksi
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(2, HIGH);
    delay(500);
    digitalWrite(2, LOW);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(2, HIGH);
  delay(3000);
  digitalWrite(2, LOW);
  WiFiClient client; // Use WiFiClient class to create TCP connections
}

void loop() {
  delay(10000);

  Serial.print("connecting to ");
  Serial.println(host);
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  // We now create a URI for the request
  String readLED = "/ESPinsert/ledStats/";
  readLED += myID;
  
  
  Serial.print("Requesting URL: ");
  Serial.println(readLED);

   // Perintah untuk membaca data dari server
   client.print(String("GET ") + readLED + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
               
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
  String line; // Read all the lines of the reply from server and print them to Serial
  while(client.available()) {
    line = client.readStringUntil('\n');
    Serial.print(line);
  }
  int ON = line.indexOf("#ON#");
  int OFF = line.indexOf("#OFF#");
  if (ON > OFF){
    Serial.println();
    Serial.println("LED ON");
    digitalWrite(2, HIGH);
  } else if (OFF > ON){
    Serial.println();
    Serial.println("LED OFF");
    digitalWrite(2, LOW);
  }

//proses mengirim data
if (client.connect(host, httpPort)){
  Serial.println("Prepare to sending data .....");
  float value = analogRead(A0);
  String sendSor = "/ESPinsert/updateTemp?";
  sendSor += "id=";
  sendSor += myID;
  sendSor += "&temp="; //digunakan untuk membaca data sensor, dan mengirimkannya ke WEB
  sendSor += value;
   // Perintah untuk mengirimkan data ke server
   delay(100);
   client.print(String("GET ") + sendSor + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");  
  Serial.println(value);
  client.stop();
  Serial.println("Connection closed..");
}
}
