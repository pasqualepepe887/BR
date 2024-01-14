//Created By Pasquale Pepe www.pasqualepepe.framer.website

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>

//ADXL345 PIN DI CONNESSIONI 
const uint8_t scl = 2; //D5  
const uint8_t sda = 0; //D6

//ADXL345 I2C Indirizzo 0x53(83)
#define Addr 0x53 

//Variabili Wi-Fi
const char* defaultSsid = "";  // SSID predefinito
const char* defaultPassword = "";  // Password predefinita
char ssid[32];  // Buffer per l'SSID
char password[32];  // Buffer per la Password

unsigned long timeout = 30000; // Timeout per la connessione al Wi-Fi di 30 secondi (in millisecondi)
unsigned long startTime;

int eepromAddr = 0; //Indirizzo per il salvataggio del SSD e Password su memoria EEPROM


ESP8266WebServer server(80); //Indirizzo del WebServer:80

IPAddress local_IP(192,168,1,80); //IP locale,gateway e subnet per la configurazione
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

IPAddress primaryDNS(8,8,8,8);


//adxl345_extract permette di ottenere i valori dell'accelerazione lungo i tre assi
void adxl345_extract()
{
  unsigned int data[6];

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select bandwidth rate register
  Wire.write(0x2C);
  // Normal mode, Output data rate = 100 Hz
  Wire.write(0x08);
  // Stop I2C transmission
  Wire.endTransmission();

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select power control register
  Wire.write(0x2D);
  // Auto-sleep disable
  Wire.write(0x08);
  // Stop I2C transmission
  Wire.endTransmission();

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select data format register
  Wire.write(0x31);
  // Self test disabled, 4-wire interface, Full resolution, Range = +/-2g
  Wire.write(0x08);
  // Stop I2C transmission
  Wire.endTransmission();
  delay(80);

  Wire.beginTransmission(0x53);
  Wire.write(0x32);
  Wire.endTransmission();
  Wire.requestFrom(0x53, 1);
  byte x0 = Wire.read();

  Wire.beginTransmission(0x53);
  Wire.write(0x33);
  Wire.endTransmission();
  Wire.requestFrom(0x53, 1);
  
  //Inizio la conversione da byte a float
  byte x1 = Wire.read();
  x1 = x1 & 0x03;

  uint16_t x = (x1 << 8) + x0;
  int16_t xf = x;
  if (xf > 511)
  {
    xf = xf - 1024;
  }
  float xa = xf * 0.004;

  delay(100);

  Wire.beginTransmission(0x53);
  Wire.write(0x34);
  Wire.endTransmission();
  Wire.requestFrom(0x53, 1);
  byte y0 = Wire.read();

  Wire.beginTransmission(0x53);
  Wire.write(0x35);
  Wire.endTransmission();
  Wire.requestFrom(0x53, 1);
  //Inizio la conversione da byte a float
  byte y1 = Wire.read();
  y1 = y1 & 0x03;

  uint16_t y = (y1 << 8) + y0;
  int16_t yf = y;
  if (yf > 511)
  {
    yf = yf - 1024;
  }
  float ya = yf * 0.004;


  Wire.beginTransmission(0x53);
  Wire.write(0x36);
  Wire.endTransmission();
  Wire.requestFrom(0x53, 1);
  byte z0 = Wire.read();

  Wire.beginTransmission(0x53);
  Wire.write(0x37);
  Wire.endTransmission();
  Wire.requestFrom(0x53, 1);
  //Inizio la conversione da byte a float
  byte z1 = Wire.read();
  z1 = z1 & 0x03;

  uint16_t z = (z1 << 8) + z0;
  int16_t zf = z;
  if (zf > 511)
  {
    zf = zf - 1024;
  }
  float za = zf * 0.004;

  delay(80);


  server.send(200, "text/plain", String(xa) + "/" + String(ya) + "/" + String(za) + "/");  //Invio dei valori dell'accelerazione al WebServer



}

//readCredentials permette di leggere se presenti l'SSID e la Password salvati in memoria
void readCredentials() {
  EEPROM.get(eepromAddr, ssid);
  eepromAddr += sizeof(ssid);
  EEPROM.get(eepromAddr, password);
  eepromAddr += sizeof(password);
}


//resetEEPROM permette di cancellare tutti i dati salvati nella memoria EEPROM
void resetEEPROM() {
  for (int i = 0; i < 512; i++) {
    EEPROM.write(i, 0); // Scrive il valore 0 in ogni posizione
  }
  EEPROM.commit(); // Salva le modifiche
}

//saveCredentials permette di salvare in memoria l'SSID e la Password inserite dall'utente
void saveCredentials() {
  EEPROM.put(0, ssid);
  EEPROM.put(sizeof(ssid), password);
  EEPROM.commit();
}

// Pagina web per il Web-Server
void handleRoot() {

  String html = "<html><style>body{background-color:#808080;} .centered-form{display:flex;flex-direction:column;align-items:center;height:100vh} form{padding:20px;border-radius:5px;box-shadow:0 0 10px rgba(0,0,0,0.2);text-align:center;margin: 10px auto;font-size:15px;width:300px;} form input, form select, form textarea{display:block;margin: 10px auto;width:100%;}</style><body>";
  html += "<div class='centered-form'>";
  html += "<h1>BR</h1>";
  html += "<form method='POST' action='/save'>";
  html += "SSID: <input type='text' name='ssid' value='" + String(ssid) + "'><br>";
  html += "Password: <input type='password' name='password'><br>";
  html += "<input type='submit' value='Salva'></form>";

  html += "<form action='/format' method='post'>";
  html += "<input type='submit' value='Formatta Wi-Fi'></form>";

  html += "<form action='/date' method='post'>";
  html += "<input type='submit' value='Accedi ai Dati'></form>";

  html += "</div></body></html>";

  server.send(200, "text/html", html);

}

//connectToWiFi permette di connettersi al Wi-Fi con le credenziali salvate in memoria
void connectToWiFi() {
  Serial.println("Connessione a Wi-Fi...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
   if (millis() - startTime > timeout) {
      // Timeout raggiunto riporta il BR come AccessPoint
      Serial.println("Timeout di connessione scaduto. Riavvio in corso...");
     configureBRWiFi();
    }
    
  }
  IPAddress staticIP(192, 168, 1, 45);  // L'indirizzo IP statico per il WebServer
  IPAddress gateway(192, 168, 1, 1);    // L'indirizzo IP del gateway
  IPAddress subnet(255, 255, 255, 0);   // La subnet mask

  WiFi.config(staticIP, gateway, subnet);
  

  Serial.println("Connessione stabilita.");

}


// handleSave permette di salvare le nuove credenziali
void handleSave() {
  //ottiene il valore del nuovo SSID e Password dal Web-Server
  String newSsid = server.arg("ssid"); 
  String newPassword = server.arg("password");

  if (newSsid.length() > 0 && newPassword.length() > 0) {
    strcpy(ssid, newSsid.c_str());
    strcpy(password, newPassword.c_str());

    saveCredentials(); //Salva le credenziali in memoria
    connectToWiFi(); //Connettiti alle nuove credenziali

    // Reindirizza alla pagina di successo
    server.sendHeader("Location", "/");
    server.send(303);
  } else {
    // Se i campi sono vuoti, mostra un messaggio di errore
    server.send(400, "text/plain", "Errore: Assicurati di inserire SSID e Password.");
  }
}



//configureWiFi permette di di configurare il BR come access-point per permettere all'utente di inserire le proprie credenziali
void configureBRWiFi() {
  Serial.println("Modalità configurazione Wi-Fi. Connetti il dispositivo al tuo AP.");
  Serial.println();

  // Crea un access point per la configurazione
  WiFi.softAP("BR-AP", "password");
  IPAddress staticIP_c(192, 168, 1, 10);  // L'indirizzo IP statico che desideri utilizzare
  IPAddress gateway_c(192, 168, 1, 1);    // L'indirizzo IP del gateway
  IPAddress subnet_c(255, 255, 255, 0);   // La subnet mask

  WiFi.config(staticIP_c, gateway_c, subnet_c);
  // Imposta le pagine web per la configurazione
  server.on("/", HTTP_GET, handleRoot);
  server.on("/save", HTTP_POST, handleSave);
  server.on("/date", HTTP_GET, []() {
  adxl345_extract();
  });

  server.on("/format", HTTP_POST, []() {
        resetEEPROM();
        server.send(200, "text/html", "Credenziali Cancellata. Riavvio il BR");
        ESP.restart();
      
    });

  server.begin();
}




void setup()
{
  // Inizia la connessione I2C con l'ADXL345 come Master
  Wire.begin(sda, scl);
  //imposta il baud rate = 115200
  Serial.begin(115200);
  EEPROM.begin(512);

  readCredentials(); //Leggi le credenziali del Wi-Fi in memoria


  // Imposta il BR per la connessione al Wi-Fi
  WiFi.mode(WIFI_STA);

  // Se non ci sono credenziali salvate, imposta il dispositivo in modalità configurazione
  if (strlen(ssid) == 0 || strlen(password) == 0) {
    configureBRWiFi();
  } else {
    // Connessione Wi-Fi utilizzando le credenziali salvate
    connectToWiFi();
  }
  
  server.on("/", HTTP_GET, handleRoot);
  server.on("/save", HTTP_POST, handleSave);
  server.on("/data", HTTP_GET, []() {
    adxl345_extract();
  });

  server.on("/format", HTTP_POST, []() {
    resetEEPROM();
    server.send(200, "text/html", "Credenziali Cancellata. Riavvia il BR");
    ESP.restart();

  });
  server.begin();
  Serial.println("Web-Server Online");
}



void loop()
{
  server.handleClient(); //WebServer
}
