#include <ESP8266WiFi.h>
#include <time.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp8266.h>
#include <DHTesp.h>




#define BLYNK_PRINT Serial1
#define BLYNK_TEMPLATE_ID "TMPL6-Oe0koGm"
#define BLYNK_TEMPLATE_NAME "DHT"
#define BLYNK_AUTH_TOKEN "zvIuRCn5WWY10eVKKTm9A4luaodwObIv"


char auth[] = "zvIuRCn5WWY10eVKKTm9A4luaodwObIv";
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "VanTuyen";
char pass[] = "0975617367";


#define DHTPIN D4
#define analogPin A0
#define RELAY D3
#define Button_Bom D5
#define button_led D7
#define Light D6
#define Led D0
#define OLED_RESET -1
// #define SCL D1
// #define SDA D2



boolean chedo_hoatdong =0;
boolean button_bomtuoiState = HIGH;
int doam_bomtuoi = 0;
int doam_tatbomtuoi =0;
WidgetLED ledConnect(V0);
unsigned long times = millis();
int timezone = 7 * 3600;
int dst = 0;

DHTesp dht;
BlynkTimer timer;

Adafruit_SSD1306 display(128, 64, &Wire, -1);

void setupOled(){
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // Clear the buffer.
  display.clearDisplay();
  display.display();
  
  Serial.begin(115200);

  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  display.setCursor(0,0);
  display.println("Welcom to");
  display.println("SMART GARDEN");

  WiFi.begin(ssid,pass);
 
  display.println("\nConnecting");

  display.display();

  while( WiFi.status() != WL_CONNECTED ){
      delay(500);
      display.print("."); 
      display.display();       
  }

  // Clear the buffer.
  display.clearDisplay();
  display.display();
  display.setCursor(0,0);
  

  display.print("IP:");
  display.println(WiFi.localIP() );

  display.display();

  configTime(timezone, dst, "pool.ntp.org","time.nist.gov");
  display.println("\nWaiting for NTP...");

  while(!time(nullptr)){
     Serial.print("*");
     
     delay(1000);
  }
  display.println("\nTime response....OK"); 
  display.display();  
  delay(1000);

  display.clearDisplay();
  display.display();

}

void setup(){
  setupOled();
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  delay(100);
  pinMode(RELAY, OUTPUT);
  pinMode(Button_Bom, INPUT_PULLUP);
  pinMode(button_led, INPUT_PULLUP);
  pinMode(Led, OUTPUT);
  digitalWrite(Led, LOW);
  digitalWrite(RELAY, LOW);
  dht.setup(DHTPIN, DHTesp::DHT11);
}
void loop (){
  // Set up led connect
  timer.run();
  Blynk.run();

  if (ledConnect.getValue()){
    ledConnect.off();
  }
  else{
    ledConnect.on(); 
  }
  
  //  Doc gia tri ngay thang nam va thoi gian
  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);

  Serial.print(p_tm->tm_mday);
  Serial.print("/");
  Serial.print(p_tm->tm_mon + 1);
  Serial.print("/");
  Serial.print(p_tm->tm_year + 1900);
    
  Serial.print(" ");
    
  Serial.print(p_tm->tm_hour);
  Serial.print(":");
  Serial.print(p_tm->tm_min);
  Serial.print(":");
  Serial.println(p_tm->tm_sec);

  // Doc gia tri nhiet do va do am
  float temp = dht.getTemperature();
  float hum = dht.getHumidity();
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print(" *C ");
  Serial.print("\t");
  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.println(" % ");
  Blynk.virtualWrite(V1, temp);
  Blynk.virtualWrite(V2, hum);

  // Doc gia tri do am dat

  int value = analogRead(analogPin);
  int phantram = map (value, 0, 1023, 0, 100);
  int doam_dat = 100 - phantram;
  Serial.print("Soil Moisture: ");
  Serial.println(doam_dat);
  Blynk.virtualWrite(V3, doam_dat);

// Cac che do theo doi anh sang
  // Bat den tu dong
  // Doc gia tri anh sang
  Serial.println(chedo_hoatdong);
  if (chedo_hoatdong ==1){
    int anhsang = digitalRead(Light);
    delay(2000);
    Serial.println("Light Level: " + String (anhsang));
    if (anhsang == 1){
      digitalWrite(Led, HIGH);
      Serial.println("Light On");
      Blynk.virtualWrite(V6, HIGH);
      delay(500);
    }
    else{
      digitalWrite(Led, LOW);
      Blynk.virtualWrite(V6, LOW);
      Serial.println("Light Off");
      delay(500);
    }
  }
  else{
    if (digitalRead(button_led) == LOW){
      digitalWrite(Led, HIGH);
      Blynk.virtualWrite(V6, digitalRead(Led));
      delay(1000);
    }
    else {
      digitalWrite(Led, LOW);
      Blynk.virtualWrite(V6, digitalRead(Led));
      delay(1000);
    }
  }

// Cac che do bơm tuoi
  // Che do tu dong
  Serial.println("Button State Status: " + String (digitalRead(button_bomtuoiState)));
  if (chedo_hoatdong == 1){
    if (doam_dat < doam_bomtuoi){
      digitalWrite(RELAY, LOW);
      Blynk.virtualWrite(V5, digitalRead(RELAY));
      Serial.println("Automatic: The water pump is turned on !");
      delay(1000);
    }
    else{
      if (doam_dat >= doam_tatbomtuoi){
        digitalWrite(RELAY, HIGH);
        Blynk.virtualWrite(V5, digitalRead(RELAY));
        Serial.println("Automatic: The water pump is turned off !");
        delay(1000);
      }
    }
  }
  times = millis();

  Serial.println("Button Status: " + String (digitalRead(Button_Bom)));
  
    // Che do dieu khien bang tay
  delay(1000);
  if (digitalRead(Button_Bom) == LOW){
    if (button_bomtuoiState == HIGH){
      button_bomtuoiState = LOW;
      chedo_hoatdong =0;
      Blynk.virtualWrite(V4, HIGH);
      digitalWrite(RELAY, !(digitalRead(RELAY)));
      Blynk.virtualWrite(V5, (digitalRead(RELAY)));
      Serial.println("Manual: The water pump is turned on !");
      delay(10000);
    }
    
    else{
      button_bomtuoiState = HIGH;
    }
  }

  // In
  display.clearDisplay();
  
  // Hien thi gio va phut
  display.setTextSize(2);
  display.setCursor(25,50);
  display.print(p_tm->tm_hour);
  display.print(":");
  if( p_tm->tm_min <10)
    display.print("0"); 
  display.print(p_tm->tm_min);
    
    
  // Hien thi giay
  display.setTextSize(1.8);
  display.setCursor(84,55);
  display.print(".");
  if( p_tm->tm_sec <10)
    display.print("0"); 
  display.print(p_tm->tm_sec);

  // Hien thi ngay thang nam va nhiet do
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print(p_tm->tm_mday);
  display.print("/");
  display.print(p_tm->tm_mon + 1);
  display.print("/");
  display.print(p_tm->tm_year + 1900);
  display.print("\nMode: ");
  if(chedo_hoatdong == 1){
    display.print("Automatic");
  }
  else{
    display.print("Manual");
  }
  display.print("\nTemperture: ");
  display.print(temp);
  display.print(" *C");
  display.print("\nHumidity: ");
  display.print(hum);
  display.print(" %");
  display.print("\nSoil Moisture: ");
  display.print(doam_dat);
  display.print(" %");
  display.setCursor(53, 0);
  display.print("  SG8");
  display.display();
  delay(1000); // update every 1 sec
}

// Ket noi du lieu voi Blynk

// Dong nhat du lieu voi Blynk
BLYNK_CONNECTED(){
  Blynk.syncVirtual(V4,V5,V6,V7,V8);
}

BLYNK_WRITE(V4){
  chedo_hoatdong = param.asInt();
  Serial.println("Mode: " + String(chedo_hoatdong));
}

BLYNK_WRITE(V5){
  if (chedo_hoatdong == 0){
    int relayState = param.asInt();
    digitalWrite(RELAY, relayState);
    Serial.println("Manual: The water pump is turned " + String(relayState));
  }
}

BLYNK_WRITE(V6){
  int lightState = param.asInt();
  digitalWrite(Led, lightState);
  Serial.println("Light: " + String(lightState));
}

BLYNK_WRITE(V7){
  doam_bomtuoi = param.asInt();
  Serial.println("Humidity before pumping : " + String(doam_bomtuoi));
}

BLYNK_WRITE(V8){
  doam_tatbomtuoi = param.asInt();
  Serial.println("Humidity when the pump is turned off :" + String(doam_tatbomtuoi));
}
