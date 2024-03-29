/*
 * Membaca data dari Database
 * untuk menjalankan program ini, anda harus mempunyai dulu ID untuk terhubung
 * dengan cara registrasi terlebih dahulu di https://in-thishub.000webhostapp.com
 * 
 * jika ingin menggunakan host lain, seperti website anda pribadi atau di localhost, rubah string yang terdapat pada host
 * **********************************************************************************************************************
 * created : 20 june 2019
 * for more information, visit =>>> papermindvention.blogspot.com
 */

#include <ESP8266WiFi.h>

const char* ssid     = "Your SSID"; //nama ssid
const char* password = "Passsword SSID"; // password ssid

const char* host = "https://in-thishub.000webhostapp.com";
const char* myID = "Your ID"; //masukkan id kamu, berisi 32 karater

void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT); //NOT logic, if val 1 = LED 0ff, else if val 0 = LED ON
  delay(10);
  digitalWrite(2, LOW);
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
  Serial.println(WiFi.localIP()); //print local IP
  digitalWrite(2, HIGH);
  delay(3000);
  digitalWrite(2, LOW);
}

void loop() {
  Serial.print("connecting to ");
  Serial.println(host);
  
  WiFiClient client; // Use WiFiClient class to create TCP connections
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  // We now create a URI for the request
  String sendSor = "/ESPinsert/ledStats/";
  sendSor += myID;
  
  Serial.print("Requesting URL: ");
  Serial.println(sendSor);

  // Perintah untuk mengirimkan data ke server
   client.print(String("GET ") + sendSor + " HTTP/1.1\r\n" +
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
  int ON = line.indexOf("#ON#");   // Cari text #ON# pada string
  int OFF = line.indexOf("#OFF#"); // Cari text #OFF# pada string
  if (ON > OFF){
    Serial.println();
    Serial.println("LED ON");
    digitalWrite(2, HIGH);
  } else if (OFF > ON){
    Serial.println();
    Serial.println("LED OFF");
    digitalWrite(2, LOW);
  }
 Serial.println(); 
 delay(10000);
}

