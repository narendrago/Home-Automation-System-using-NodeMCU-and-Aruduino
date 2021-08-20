#include <ESP8266WiFi.h>

const char* ssid="*******";
const char* password="********";

WiFiServer server(5000);

String header;

String output5State="off";
String output4State="off";

const int output5=5;
const int output4=4;

unsigned long currentTime=millis();

unsigned long previousTime=0;

const long timeoutTime=2000;

void setup(){
  Serial.begin(115200);

  pinMode(output5,OUTPUT);
  pinMode(output4,OUTPUT);

  digitalWrite(output5,LOW);
  digitalWrite(output4,LOW);

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid,password);

  while(WiFi.status()!=WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Wi-Fi connected");
  Serial.println("IP Address:");
  Serial.println(WiFi.localIP());
  server.begin();
}
void loop()
{
  WiFiClient client=server.available();

  if(client)
  {
    Serial.println("New Client.");
    String currentLine="";
    currentTime=millis();
    previousTime=currentTime;
    while(client.connected() && currentTime-previousTime<=timeoutTime){
      currentTime=millis();
      if(client.available())
      {
        char c=client.read();
        Serial.write(c);
        header+=c;
        if(c=='\n'){
          if(currentLine.length()==0){
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection:close");
            client.println();

            if(header.indexOf("GET /5/on")>=0){
              Serial.println("GPIO 5 on");
              output5State="on";
              digitalWrite(output5,HIGH);
            }else if(header.indexOf("GET /5/off")>=0){
              Serial.println("GPIO 5 off");
              output5State="off";
              digitalWrite(output5,LOW);
            }else if(header.indexOf("GET /4/on")>=0){
              Serial.println("GPIO 4 on");
              output4State="on";
              digitalWrite(output4,HIGH);
            }else if(header.indexOf("GET /4/off")>=0){
              Serial.println("GPIO 4 off");
              output4State="off";
              digitalWrite(output4,LOW);
            }
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<style>html{font-family:Helvetica; display:inline-block; margin:0 px auto; text-align:center;}");
            client.println(".button1{background-color:#195B6A; border:none; color:white; padding:16px 40px; font-size:30px; margin:2px; cursor:pointer;}");
            client.println(".button2{background-color:#195B6A; border:none; color:white; padding:16px 40px; font-size:30px; margin:2px; cursor:pointer;}");
            client.println("</style></head>");

            client.println("<body><h1>Home Automation System</h1>");
            client.println("<h2>CSE2006 Team Number:4</h2>");
            client.println("<p>LED-1 State:"+output5State+"</p>");
            if(output5State=="off"){
              client.println("<p><a href=\"/5/on\"><button class=\"button1\">ON</button></a></p>");
            }else{
              client.println("<p><a href=\"/5/off\"><button class=\"button2\">OFF</button></a></p>");
            }

            client.println("<p>LED-2 State:"+output4State+"</p>");
            if(output4State=="off"){
              client.println("<p><a href=\"/4/on\"><button class=\"button1\">ON</button></a></p>");
            }else{
              client.println("<p><a href=\"/4/off\"><button class=\"button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");

            client.println();
            break;
            
          }else{
            currentLine="";
          }
        }else if(c!='\r'){
          currentLine+=c;
        }
      }
    }
    header="";
    client.stop();
    Serial.println("Client Disconnected");
    Serial.println("");
  }
}
