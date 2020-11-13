// This project is the basic script to design and implement low cost 
// controllling system for the proejectors using Hex commands through 
// TCP 7142 port  
//https://www.industrialshields.com/blog/arduino-industrial-1/post/how-to-create-a-tcp-client-on-an-arduino-based-plc-55
#include <Ethernet.h>
#include <SPI.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = {192,168,0,1};//arduino's IP
byte projector[] = {192,168,0,10}; // the projector IP
byte variable[22];//21,because the status reply is 21 bytes

int k=0,Reset=5;
int x,y;// x,acts as a flag for the yellow led & y acts as a flag for the red led

EthernetClient client; 
byte commandOn[]={0x02,0x00,0x00,0x00,0x00,0x02};// turn on command
byte commandOff[]={0x02,0x01,0x00,0x00,0x00,0x03};// turn off command
byte StatusReq []={0x00,0x85,0x00,0x00,0x01,0x01,0x87};//running status command
void setup()
{
Serial.begin(9600);
digitalWrite(Reset, HIGH); 
Ethernet.begin(mac, ip); 
pinMode(Reset, OUTPUT);
pinMode(2,INPUT);
pinMode(3,OUTPUT);
pinMode(4,OUTPUT);

delay(200);
Serial.println(" ");
Serial.println("connecting to the projector...");
client.connect(projector,7142);
if (client.connect(projector, 7142)) {
    Serial.println("connected  and my local IP");
    Serial.println(Ethernet.localIP());
    Serial.println("connected to the projector");

    } 
attachInterrupt(digitalPinToInterrupt(2),Intrrupt, RISING);
}
//=============================loop============================
void loop(){
  if(!client.connected()){
    Serial.println("connection failed, Reconnecting....");
    client.connect(projector, 7142);
    if (client.connect(projector, 7142)) {
      Serial.println("finally connected");
    }
  }
  delay(500);
 }
//=============Functions Declaration======================
//---------------------the interrupt function----------------------------------------
void Intrrupt(){// this function is called whenever the button is pressed
// delay(3000);
Active_Status();
  //TurnOn();
  //TurnOff;
}
//---------------Activity Process--------------------------
int  Active_Status(){ //here to check if the projector is on standby or active
//delay(4000);
control();

  }
//------------------------------------------------------------------------
void control(){ // it controls what happens after pressing the button
  getStatus();
  byte p=variable[7];
  Serial.println(" ");
  if(p==0){
   Serial.println("The projector is on standby....");
  delay(900);
  // TurnOff();
   return y=0;

    }
  if(p==1){
  Serial.println("the projector is Actively running...... ");
  delay(900);
 // TurnOn();
  return;
  }
}
//--------------Turn off the Projector------------------------------------------------
int TurnOff(){
client.write(commandOff,sizeof(commandOff));//turn off the projector
delay(100);
for(int i=0;i<8;i++){
      variable[i] = client.read();}
for(int i=0;i<8;i++){ // to understand the response, refer to page 15 on the control manual
      Serial.print(variable[i],HEX);
      Serial.print(";");
      }
digitalWrite(3,LOW);
digitalWrite(4,HIGH);
delay(1000);
digitalWrite(Reset, LOW);
return y=0;
  }
//-----------------Turn on the projector----------------------------------------------
int TurnOn(){
client.write(commandOn,sizeof(commandOn));//turn off the projector
 //delay(100);
for(int i=0;i<8;i++){
     variable[i] = client.read();}
for(int i=0;i<8;i++){ // to understand the response, refer to page 15 on the control manual
      Serial.print(variable[i],HEX);
       Serial.print(";");
       }
digitalWrite(4,LOW);
digitalWrite(3,HIGH);
return y=1;
  }
//---------------Get the status of the projector----------------------------------------------------------
byte getStatus(){
client.write(StatusReq,sizeof(StatusReq));// get the on/ff status
 for(int i=0;i<22;i++){
      variable[i] = client.read();}
    Serial.println(" ");
    for(int i=0;i<22;i++){ // to understand the response, refer to page 15 on the control manual
      Serial.print(variable[i],HEX);
       Serial.print(";");
     
  }
 return variable;
  }
/*
//--------Process Status------------------------------------
void Process_Status(){ //check if the projector is in the process of (turning-on/turning-off)
     getStatus();
 byte l=variable[9];
  Serial.println(" ");
  if((l)==0){
    Serial.println("The projector is available....");
    digitalWrite(4,LOW);
   x=0;
    }
  else{
  Serial.println("the projector is busy and currently it doesn't accept any command...... ");
  digitalWrite(4,HIGH);
  x=1;
 }}  
 */ 
