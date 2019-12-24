// libraries
#include <MKRGSM.h>
#include <math.h>


#include "DHT.h"
#include <TinyGPS.h> 
#include "arduino_secrets.h" 
#define DHTPIN1 5     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT21
#define SerialGPS Serial1

const int z1_out = A6; /* connect z_out of module to A3 of UNO board */


int kipas1 = 3;


DHT dht1(DHTPIN1, DHTTYPE);

TinyGPS gps;

// Please enter your sensitive data in the Secret tab or arduino_secrets.h
// PIN Number
const char PINNUMBER[]     = SECRET_PINNUMBER;
// APN data
const char GPRS_APN[]      = SECRET_GPRS_APN;
const char GPRS_LOGIN[]    = SECRET_GPRS_LOGIN;
const char GPRS_PASSWORD[] = SECRET_GPRS_PASSWORD;

// initialize the library instance
GSMClient client;
GPRS gprs;
GSM gsmAccess;
GSM_SMS sms;
// URL, path and port (for example: example.org)
char server[] = "inoprex.com";
//char path[] = " ";
int port = 80; // port 80 is the default for HTTP
String apiKeyValue = "9F7j3bA5TAmPt";
String  postData;
bool stop_it = false;
bool stop_it2 = false;
void setup() {
  // initialize serial communications and wait for port to open:
  Serial.begin(9600);
 SerialGPS.begin(9600); //TEST GPS.
dht1.begin();

 pinMode(2, INPUT_PULLUP);
 pinMode(kipas1,OUTPUT);


//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for native USB port only
//  }

  Serial.println("Starting Arduino web client.");
  // connection state
  bool connected = false;

  // After starting the modem with GSM.begin()
  // attach the shield to the GPRS network with the APN, login and password
  while (!connected) {
    if ((gsmAccess.begin(PINNUMBER) == GSM_READY) &&
        (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD) == GPRS_READY)) {
      connected = true;
    } else {
      Serial.println("Not connected");
      delay(1000);
    }
  }
   while (!connected) {
    if (gsmAccess.begin(SECRET_PINNUMBER1) == GSM_READY) {
      connected = true;
    } else {
      Serial.println("Not connected");
      delay(1000);
    }
  }
    
 
}
boolean stop_main_loop(){
  if (stop_it == false){

    //Serial.println("halloasasasasa");
    //delay(1000);
  Serial.print("Enter a mobile number: ");
  const char remoteNum[] = driver ;  // telephone number to send sms
 // readSerial(remoteNum);
  Serial.println(remoteNum);

  // sms text
  Serial.print("Now, enter SMS content: ");
  char txtMsg[200]= "hati-hati mas";
 // readSerial(txtMsg);
  Serial.println("SENDING");
  Serial.println();
  Serial.println("Message:");
  Serial.println(txtMsg);

  // send the message
  sms.beginSMS(remoteNum);
  sms.print(txtMsg);
  sms.endSMS();
  Serial.println("\nCOMPLETE!\n");

    
  }

  stop_it = true;
  return stop_it;
}

boolean stop_main_loop2(){
  if (stop_it2 == false){

    //Serial.println("halloasasasasa");
    //delay(1000);
  Serial.print("Enter a mobile number: ");
  const char remoteNum[] = konsumen ;  // telephone number to send sms
 // readSerial(remoteNum);
  Serial.println(remoteNum);

  // sms text
  Serial.print("Now, enter SMS content: ");
  char txtMsg[200]= "hati-hati mas";
 // readSerial(txtMsg);
  Serial.println("SENDING");
  Serial.println();
  Serial.println("Message:");
  Serial.println(txtMsg);

  // send the message
  sms.beginSMS(remoteNum);
  sms.print(txtMsg);
  sms.endSMS();
  Serial.println("\nCOMPLETE!\n");

    
  }

  stop_it2 = true;
  return stop_it2;
}




void loop() {
  // if there are incoming bytes available
  // from the server, read them and print them:

  int z1_adc_value; 
  double z1_g_value;
  z1_adc_value = analogRead(z1_out);
  int swic = digitalRead(2);
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h1 = dht1.readHumidity();
  // Read temperature as Celsius (the default)
  float t1 = dht1.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f1 = dht1.readTemperature(true);


  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h1) || isnan(t1) || isnan(f1)) {
 //   Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }



  // Compute heat index in Fahrenheit (the default)
  float hif1 = dht1.computeHeatIndex(f1, h1);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic1 = dht1.computeHeatIndex(t1, h1, false);


  if (t1 >20){
    digitalWrite(kipas1,HIGH);
  delay(1000);
  
  }

  if ( z1_adc_value > 800){
  stop_it = stop_main_loop();  
  }
  
  
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;
 
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (SerialGPS.available())
    {
      char c = SerialGPS.read();
      // Serial.write(c); // hilangkan koment jika mau melihat data yang dikirim dari modul GPS
      if (gps.encode(c)) // Parsing semua data
        newData = true;
    }
  }
    float flat, flon,sat;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
   // Serial.print("LAT=");
    (flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    //Serial.print(" LON=");
    (flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
     sat = (gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites()); 
 

  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
  
 if ( swic == 0){
 Serial.println("connecting...");
  postData = "api_key=" + apiKeyValue + "&latitude=" + String(flat, 6)
                          + "&longitude=" + String(flon, 6) + "&dhtA=" + String(t1)
                          + "&adxlA=" + String(z1_adc_value) + "&iduser=" + String(4536) + "&status=" + String("jalan") + "";

  // if you get a connection, report back via serial:
  if (client.connect(server, port)) {
     Serial.println("postdata");
  client.println("POST /skripsi/bridge.php HTTP/1.1");
  client.println("Host:  inoprex.com");
  //client.println("User-Agent: Arduino/1.0");
  client.println("Connection: close");
  client.println("Content-Type: application/x-www-form-urlencoded;");
  client.print("Content-Length: ");
  client.println(postData.length());
  client.println();
  client.println(postData);
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
 }
 if ( swic == 1){
 Serial.println("connecting...");
  postData = "api_key=" + apiKeyValue + "&latitude=" + String(flat, 6)
                          + "&longitude=" + String(flon, 6) + "&dhtA=" + String(t1)
                          + "&adxlA=" + String(z1_adc_value) + "&iduser=" + String(4536) + "&status=" + String("kembali") + "";

  // if you get a connection, report back via serial:
  if (client.connect(server, port)) {
     Serial.println("postdata");
  client.println("POST /skripsi/bridge.php HTTP/1.1");
  client.println("Host:  inoprex.com");
  //client.println("User-Agent: Arduino/1.0");
  client.println("Connection: close");
  client.println("Content-Type: application/x-www-form-urlencoded;");
  client.print("Content-Length: ");
  client.println(postData.length());
  client.println();
  client.println(postData);
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
   stop_it2 = stop_main_loop2();  
  }

 }
/*
 postData = "api_key=" + apiKeyValue + "&latitude=" + String(flat, 6)
                          + "&longitude=" + String(flon, 6) + "&dhtA=" + String("t1")
                          + "&adxlA=" + String("z1_adc_value") + "&iduser=" + String("4536") + "&status=" + String("kembali") + "";

  // if you get a connection, report back via serial:
  if (client.connect(server, port)) {
     Serial.println("postdata");
  client.println("POST /skripsi/bridge.php HTTP/1.1");
  client.println("Host:  inoprex.com");
  //client.println("User-Agent: Arduino/1.0");
  client.println("Connection: close");
  client.println("Content-Type: application/x-www-form-urlencoded;");
  client.print("Content-Length: ");
  client.println(postData.length());
  client.println();
  client.println(postData);
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
   stop_it2 = stop_main_loop2();  
  }
 */  
}
