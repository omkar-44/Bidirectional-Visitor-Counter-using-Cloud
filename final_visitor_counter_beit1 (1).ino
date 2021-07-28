#include <LiquidCrystal.h>
#include <IRremote.h>
int RECV_PIN = 10;
IRrecv irrecv(RECV_PIN);
decode_results results;
int in = 15;
int inpr = 16;
int out = 14;
int outpr = 17;
int ppl = 0;
int total=0;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
bool pi = 0;
bool po = 0;

String ssid     = "Simulator Wifi";  // SSID to connect to
String password = ""; // Our virtual wifi has no password (so dont do your banking stuff on this network)
String host     = "api.thingspeak.com"; // API
const int httpPort   = 80;
String uri     = "/update?api_key=XUZ7L6M328W1WAAQ&field1=";

int setupESP8266(void) {
  // Start our ESP8266 Serial Communication
  Serial.begin(115200);   // Serial connection over USB to computer
  Serial.println("AT");   // Serial connection on Tx / Rx port to ESP8266
  delay(10);        // Wait a little for the ESP to respond
  if (!Serial.find("OK")) return 1;
    
  // Connect to 123D Circuits Simulator Wifi
  Serial.println("AT+CWJAP=\"" + ssid + "\",\"" + password + "\"");
  delay(10);        // Wait a little for the ESP to respond
  if (!Serial.find("OK")) return 2;
  
  // Open TCP connection to the host:
  Serial.println("AT+CIPSTART=\"TCP\",\"" + host + "\"," + httpPort);
  delay(50);        // Wait a little for the ESP to respond
  if (!Serial.find("OK")) return 3;
  
  return 0;
}

void anydata(void) {

      int temp=  ppl;

  
  // Construct our HTTP call
  String httpPacket = "GET " + uri + String(temp) + " HTTP/1.1\r\nHost: " + host + "\r\n\r\n";
  int length = httpPacket.length();
  
  // Send our message length
  Serial.print("AT+CIPSEND=");
  Serial.println(length);
  delay(10); // Wait a little for the ESP to respond if (!Serial.find(">")) return -1;

  // Send our http request
  Serial.print(httpPacket);
  delay(10); // Wait a little for the ESP to respond
  if (!Serial.find("SEND OK\r\n")) return;
  
  
}




void setup() {
  pinMode(15, INPUT);
  pinMode(14, INPUT);
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(13, OUTPUT);
  
  
  setupESP8266();
  lcd.begin(16, 2);
  irrecv.enableIRIn();
  
}

void loop() {
  	    if (irrecv.decode(&results))
    {
          
      if (results.value == 0xFD00FF)
  	{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("WELCOME ");
    lcd.setCursor(3,2);
    lcd.print("AT MGM ");
    delay(700);
      }
    if (results.value == 0xFD40BF)
    {
    
  lcd.clear();
  digitalWrite(outpr, HIGH);
  digitalWrite(inpr, HIGH);
  pi = digitalRead(in);
  po = digitalRead(out);
  if (pi == 1){
    ppl--;
    delay(500);
  }
  else if (po == 1){
    ppl++ ;
    delay(500);
  }
  ppl = constrain(ppl, 0, 50);
  lcd.setCursor(0, 0);
        
  lcd.print("PEOPLE IN:");
  lcd.setCursor(11, 0);
  lcd.print(ppl);
  
  if (ppl >= 10){
     lcd.setCursor(0, 1);
     lcd.print("PLEASE WAIT");
     digitalWrite(13, HIGH);
     delay(700);
  }
  if (ppl <= 9){
     lcd.setCursor(0, 1);
     lcd.print("PLEASE VISIT");
     digitalWrite(13, LOW);
    delay(700);
    
  }
	anydata();
  delay(100);
      }
          // Receive the next value 
        }
     
}

 