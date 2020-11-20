// This project is the basic script to design and implement low cost 
// controllling system for the proejectors using Hex commands through 
// TCP 7142 port  
//https://www.industrialshields.com/blog/arduino-industrial-1/post/how-to-create-a-tcp-client-on-an-arduino-based-plc-55
#include <Ethernet.h>
#include <SPI.h>

byte mac[] = {0xDE,0xAD,0xBE,0xEF,0xFE,0xED};
byte ip[] = {192,168,0,1};
byte projector[] = {192,168,0,10}; // the projector IP
byte variable[22];//21,because the status reply is 21 bytes
const byte green =5;
const byte red =6;
int k,Reset=7;
static unsigned long last_interrupt_time = 0;
 unsigned long interrupt_time;
EthernetClient client; 
byte commandOn[]={0x02,0x00,0x00,0x00,0x00,0x02};// turn on command
byte commandOff[]={0x02,0x01,0x00,0x00,0x00,0x03};// turn off command
byte StatusReq []={0x00,0x85,0x00,0x00,0x01,0x01,0x87};//running status command
void setup(){
 digitalWrite(Reset, HIGH); 
 Serial.begin(9600);
 pinMode(Reset, OUTPUT);
 Ethernet.begin(mac, ip); 
 pinMode(2,INPUT_PULLUP); //on button
 pinMode(3,INPUT_PULLUP);//off button
 pinMode(6,OUTPUT);
 pinMode(5,OUTPUT);
 Serial.println("connecting to the projector...");
 client.connect(projector,7142);
 if (client.connect(projector, 7142)) {
    Serial.println("connected  and my local IP");
    Serial.println(Ethernet.localIP());
    Serial.println("connected");
    }
attachInterrupt(digitalPinToInterrupt(3),Intrrupt0ff, CHANGE);
attachInterrupt(digitalPinToInterrupt(2),Intrrupton, CHANGE);

}
void loop(){ 
  interrupt_time = millis();
 if(!client.connected()){
  Serial.println("connection failed, Reconnecting....");
  client.connect(projector, 7142);
  if (client.connect(projector, 7142)) {
   Serial.println("finally connected");
   }
  }
delay(300); 
}
//============================================================
//=============Function Declarations======================
//---------------------the interrupt function----------------------------------------
void  Intrrupt0ff(){// this function is called whenever the button is pressed
 //unsigned long interrupt_time = millis();
 // If interrupts come faster than 20000ms, assume it's a bounce and ignore
 if (interrupt_time - last_interrupt_time > 2000)
 {
 Serial.println("turning off the projector");
 digitalWrite(green,LOW);
 digitalWrite(red,HIGH);
 TurnOff();
}
 else{
  Serial.println("wait RED.............");
  }
 last_interrupt_time = interrupt_time;
 return;
}
void Intrrupton(){
  // unsigned long interrupt_time = millis();
   if (interrupt_time - last_interrupt_time > 2000)
 {
 digitalWrite(red,LOW);
 digitalWrite(green,HIGH);
 Serial.println("turning on the projector");
 TurnOn();
}
 else{
  Serial.println("wait GREEN...");}
//}
last_interrupt_time = interrupt_time;
 return;
}

//----Turn off the Projector------------------------------------------------
void TurnOff(){
 if(client){
  Serial.println("Sending command....");
  client.write(commandOff,sizeof(commandOff));//turn off the projector
  client.flush();
  while (client.available()) {
   Serial.print(client.read());
   Serial.print(";");
   }
    }
 return;
  }
//-----------------Turn on the projector----------------------------------------------
  void TurnOn(){
   if(client){
    Serial.println("Sending on command....");
    client.write(commandOn,sizeof(commandOn));//turn off the projector
    client.flush();
    while (client.available()) {
     Serial.print(client.read());
     Serial.print(";");
     }
   }
   return;
  }
