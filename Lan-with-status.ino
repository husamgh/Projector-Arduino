// This project is the basic script to design and implement low cost 
// controllling system for the proejectors using Hex commands through 
// TCP 7142 port  
//https://www.industrialshields.com/blog/arduino-industrial-1/post/how-to-create-a-tcp-client-on-an-arduino-based-plc-55
#include <Ethernet.h>
#include <SPI.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = {169,254,170,60};
byte projector[] = {169,254,170,65}; // the projector IP
byte variable[21];
byte index=0;

EthernetClient client;
 
byte commandOn[]={0x02,0x00,0x00,0x00,0x00,0x02};// turn on command
byte commandOff[]={0x02,0x01,0x00,0x00,0x00,0x03};// turn off command
byte StatusReq []={0x00,0x85,0x00,0x00,0x01,0x01,0x87};//running status command

void setup()
{
  Ethernet.begin(mac, ip);
   
  Serial.begin(9600);
  delay(500);
    
  Serial.println("connecting...");
client.connect(projector,7142);
  if (client.connect(projector, 7142)) {
   //while(client.connect(projector,7142)){
    Serial.println("connected  and my local IP");
    Serial.println(Ethernet.localIP());
   // Serial.println(client.read(),HEX);
    Serial.println("connected");
    Serial.println("sending Command..........");
  //client.write(commandOn,sizeof(commandOn));//turn on the projector
//client.write(commandOff,sizeof(commandOff));//turn off the projector
  client.write(StatusReq,sizeof(StatusReq));// get the on/ff status
    delay(100);
   for(int i=0;i<21;i++){
      variable[i] = client.read();}
    for(int i=0;i<21;i++){ // to understand the response, refer to page 61 on the control manual
      Serial.print(variable[i],HEX);
       Serial.print(";");
      }  
      Serial.println(" ");
    //Serial.println(client.read());
   // c = client.read();
     //readStr +=c;
     //Serial.println(readStr);
    
  } else {
    Serial.println("connection failed");
  }
 
  client.stop();
  client.flush();  
  Serial.println("Done.....");
}

void loop(){}

  
