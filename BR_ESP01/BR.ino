//Created By Pasquale Pepe www.pasqualepepe.framer.website

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>

// ADXL345 I2C address is 0x53(83)
#define Addr 0x53


const uint8_t scl = 2; //D5 //Accesso a ADXL345 
const uint8_t sda = 0; //D6


const char* defaultSsid = "";  // SSID predefinito
const char* defaultPassword = "";  // Password predefinita
char ssid[32];  // Buffer per l'SSID
char password[32];  // Buffer per la password
int eepromAddr = 0;

unsigned long timeout = 30000; // Timeout di 30 secondi (in millisecondi)
unsigned long startTime;

float xAccl, yAccl, zAccl;

ESP8266WebServer server(80);

IPAddress local_IP(192,168,1,80);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

IPAddress primaryDNS(8,8,8,8);

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
byte x1 = Wire.read();
x1 = x1 & 0x03;

uint16_t x = (x1 << 8) + x0;
int16_t xf = x;
if(xf > 511)
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
byte y1 = Wire.read();
y1 = y1 & 0x03;

uint16_t y = (y1 << 8) + y0;
int16_t yf = y;
if(yf > 511)
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
byte z1 = Wire.read();
z1 = z1 & 0x03;

uint16_t z = (z1 << 8) + z0;
int16_t zf = z;
if(zf > 511)
{
zf = zf - 1024;
}
float za = zf * 0.004;
 
delay(80);


 server.send(200, "text/plain", String(xa) +"/"+ String(ya)+"/"+ String(za) +"/"); 



}

void readCredentials() {
  EEPROM.get(eepromAddr, ssid);
  eepromAddr += sizeof(ssid);
  EEPROM.get(eepromAddr, password);
  eepromAddr += sizeof(password);
}
void resetEEPROM() {
  for (int i = 0; i < 512; i++) {
    EEPROM.write(i, 0); // Scrive il valore 0 in ogni posizione
  }
  EEPROM.commit(); // Salva le modifiche
}


void saveCredentials() {
  EEPROM.put(0, ssid);
  EEPROM.put(sizeof(ssid), password);
  EEPROM.commit();
}
void handleRoot() {
  // Pagina web per la configurazione delle credenziali

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
void configureWiFi() {
  Serial.println("Modalità configurazione Wi-Fi. Connetti il dispositivo al tuo AP.");
  Serial.println();

  // Crea un access point per la configurazione
  WiFi.softAP("ConfiguraWiFi", "password");
  IPAddress staticIP_c(192, 168, 1, 10);  // L'indirizzo IP statico che desideri utilizzare
  IPAddress gateway_c(192, 168, 1, 1);    // L'indirizzo IP del gateway
  IPAddress subnet_c(255, 255, 255, 0);   // La subnet mask

  WiFi.config(staticIP_c, gateway_c, subnet_c);
  // Imposta le pagine web per la configurazione
  server.on("/", HTTP_GET, handleRoot);
  server.on("/save", HTTP_POST, handleSave);
  server.on("/date", HTTP_POST, []() {
  adxl345_extract();
  });

    server.on("/format", HTTP_POST, []() {
        // Qui esegui la funzione void per spegnere il LED o eseguire altre azioni.
         resetEEPROM();
        server.send(200, "text/html", "Credenziali Cancellata. Riavvia il BR");
        ESP.restart();
      
    });

  server.begin();
}



void connectToWiFi() {
  Serial.println("Connessione a Wi-Fi...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
   if (millis() - startTime > timeout) {
      // Timeout raggiunto, esegui l'azione desiderata, ad esempio il riavvio
      Serial.println("Timeout di connessione scaduto. Riavvio in corso...");
     configureWiFi();
    }
    
  }
  IPAddress staticIP(192, 168, 1, 45);  // L'indirizzo IP statico che desideri utilizzare
  IPAddress gateway(192, 168, 1, 1);    // L'indirizzo IP del gateway
  IPAddress subnet(255, 255, 255, 0);   // La subnet mask

  WiFi.config(staticIP, gateway, subnet);
  

  Serial.println("Connessione stabilita.");
 ArduinoOTA.setHostname("BR");
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }


    Serial.println("Start updating " + type);
  });
  ArduinoOTA.begin();
}

void handleSave() {
  // Salva le nuove credenziali
  String newSsid = server.arg("ssid");
  String newPassword = server.arg("password");

  if (newSsid.length() > 0 && newPassword.length() > 0) {
    strcpy(ssid, newSsid.c_str());
    strcpy(password, newPassword.c_str());

    saveCredentials();
    connectToWiFi();

    // Reindirizza alla pagina di successo
    server.sendHeader("Location", "/");
    server.send(303);
  } else {
    // Se i campi sono vuoti, mostra un messaggio di errore
    server.send(400, "text/plain", "Errore: Assicurati di inserire SSID e Password.");
  }
}







void setup()
{
  // Initialise I2C communication as MASTER
  Wire.begin(sda, scl);
  // Initialise serial communication, set baud rate = 115200
  Serial.begin(115200);
  EEPROM.begin(512);

   readCredentials();
   
  // Connect to WiFi network


    WiFi.mode(WIFI_STA);

  // Se non ci sono credenziali salvate, imposta il dispositivo in modalità configurazione
  if (strlen(ssid) == 0 || strlen(password) == 0) {
    configureWiFi();
  } else {
    // Connessione Wi-Fi utilizzando le credenziali salvate
    connectToWiFi();
  }
  server.on("/", HTTP_GET, handleRoot);
  server.on("/save", HTTP_POST, handleSave);
  server.on("/date", HTTP_POST, []() {
  adxl345_extract();
  });
  
    server.on("/format", HTTP_POST, []() {
        // Qui esegui la funzione void per spegnere il LED o eseguire altre azioni.
         resetEEPROM();
        server.send(200, "text/html", "Credenziali Cancellata. Riavvia il BR");
        ESP.restart();
      
    });
  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{
  ArduinoOTA.handle();
  server.handleClient();
}
