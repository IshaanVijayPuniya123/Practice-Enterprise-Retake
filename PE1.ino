#define echoPin D6 // Echo Pin
#define trigPin D7 // Trigger Pin
#define BUTTON D5 //pushbutton
#define buzzer D4 // buzzer
#define ledPin  D3 // led manually controlled by webserver
unsigned char reading, state = LOW, previous = LOW;// added these characters to make 2 states of pushbutton
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>//library for our external app and esp8266 board wemos d1
#include <BlynkSimpleEsp8266.h>


 
long duration, distance; // Duration used to calculate distance
char auth[] = "QZhxCTcPg4DOvS0HKLpFp_yJU34LWdkT";
char ssid[] = "Pixel_2867";
char pass[] = "12345678"; //using personal hotspot for the project to stay on the same LAN.

BlynkTimer timer; 
WiFiServer server(80);


// This function  is used to trigger the sensor
void Ultrasonic(){
  digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
distance = duration/58.2;// We calculated the distance based on the speed of sound and since it was total distance of signal going and coming back, we divided it by 2.
Serial.println(distance);
delay(500);// placing a delay before the next reading.
}

void Practice(){
  
  if(distance>15){
        Blynk.email("ishaanpuniya@gmail.com", "DOOR UPDATE", "The door just opened");
       Blynk.notify("The Door has been opened");   //Notify used in case you missed the mail  
           
  }
  }
  void Bell(){
    
     reading = digitalRead(BUTTON);
  if (reading == HIGH && previous == LOW)
  {
    if (state == HIGH){
      state = LOW;
      Blynk.notify("Somebody is at your door"); }  //using notification as a doorbell
    else{
    state = HIGH;}
    
  }
  digitalWrite(buzzer, state); // buzzer is also used on second bell press in case you missed the notification
  previous = reading;
    
    
    }


   
 
void setup()
{
Serial.begin (9600);
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
Blynk.begin(auth, ssid, pass);
timer.setInterval(1000L, Practice);
pinMode(BUTTON, INPUT);
pinMode(buzzer,OUTPUT);
pinMode(ledPin, OUTPUT);
delay(10);
 
 //  SETTING UP INTERNET IN WIFI MODULE
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Starting the server
  server.begin();
  Serial.println("Server started");
 
  //  Collecting the IP address from here in order to access our Webserver to control the LED.
  Serial.print("Use this URL : ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

}


void loop()
{
Ultrasonic();
Blynk.run();
timer.run();
Practice();
Bell();  // calling all our functions we made earlier in the loop


 WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Waiting for data by the client
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // This will read the request sent by us through Webserver
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // Then we made the code to match the request in order to react to turn the LED on or off.
 
  int value = LOW;
  if (request.indexOf("/LED=ON") != -1) {
    digitalWrite(ledPin, HIGH);
    value = HIGH;
  } 
  if (request.indexOf("/LED=OFF") != -1){
    digitalWrite(ledPin, LOW);
    value = LOW;
  }
 
 
 
  // Then we get a reply back from the server 
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
 
  client.print("STATE OF LED PIN: ");
 
  if(value == HIGH) {
    client.print("On");  // used to detect whether the Light is on or off.
  } else {
    client.print("Off");
  }
  client.println("<br><br>");
  client.println("<br><br>");
  client.println("<b>Click <button><a href=\"/LED=ON\">here</a></button> turn the Lights on  </b><br>");
  client.println("<b>Click <button><a href=\"/LED=OFF\">here</a> </button>turn the Lights off</b><br>");
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disconnected"); // Disconnecting after turning the Lights on or off
  Serial.println("");
 

}
