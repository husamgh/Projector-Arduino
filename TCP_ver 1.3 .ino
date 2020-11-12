// This project is the basic script to design and implement low cost 
// controllling system for the proejectors using Hex commands through 
// TCP 7142 port  
//https://www.industrialshields.com/blog/arduino-industrial-1/post/how-to-create-a-tcp-client-on-an-arduino-based-plc-55
#include <Ethernet.h>
#include <SPI.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = {192,168,0,1};
byte projector[] = {192,168,0,10}; // the projector IP
byte variable[22];//21,because the status reply is 21 bytes

int k=0,Reset=5;
int x=0;// acts as a flag for the yellow led
int y=0;// acts as a flag for the red led
EthernetClient client; 
byte commandOn[]={0x02,0x00,0x00,0x00,0x00,0x02};// turn on command
byte commandOff[]={0x02,0x01,0x00,0x00,0x00,0x03};// turn off command
byte StatusReq []={0x00,0x85,0x00,0x00,0x01,0x01,0x87};//running status command
void setup()
{
digitalWrite(Reset, HIGH); 
Ethernet.begin(mac, ip); 
Serial.begin(9600);
pinMode(Reset, OUTPUT);
pinMode(2,INPUT);
pinMode(3,OUTPUT);
pinMode(4,OUTPUT);

delay(200);
Serial.println("connecting to the projector...");
client.connect(projector,7142);
if (client.connect(projector, 7142)) {
    Serial.println("connected  and my local IP");
    Serial.println(Ethernet.localIP());
    Serial.println("connected");

    } 
  else {
    Serial.println("connection failed, Reconnecting....");
    client.connect(projector, 7142);
    if (client.connect(projector, 7142)) {
      Serial.println("finally connected");
    }
  }
attachInterrupt(digitalPinToInterrupt(2),Intrrupt, RISING);
}

void loop(){
 // if(digitalRead(2)==HIGH){
   // delay(200);
   // ProjStatus();
   //TurnOn();
   //TurnOff();
  //getStatus();
  //Serial.println(" ");
  //Serial.println(k=k+1);
 }
//============================================================
//=============Function Declarations======================
//---------------------the interrupt function----------------------------------------
void  Intrrupt(){// this function is called whenever the button is pressed
//Process_Status();
// ProjStatus();
   //TurnOn();
  // TurnOff();
  //getStatus();
Active_Status();
delay(500);
if(y==1){
  Serial.println("turning off the projector...");
  TurnOff();
  }
  if(y==0){
  Serial.println("turning on the projector...");
  TurnOn();
  }
  
}
//---------------Activity Process--------------------------
void Active_Status(){ //here to check if the projector is on standby or active
  getStatus();
 // delay(1000);
  byte p=variable[7];
  Serial.println(" ");
  if(p==0){
    Serial.println("The projector is on standby....");
    digitalWrite(3,LOW);
    y=0;
   
    }
  if(p==1){
  Serial.println("the projector is Actively running...... ");
  digitalWrite(3,HIGH);
  y=1;
  }
}
//--------------Turn off the Projector------------------------------------------------
void TurnOff(){
client.write(commandOff,sizeof(commandOff));//turn off the projector
 delay(100);
 for(int i=0;i<8;i++){
      variable[i] = client.read();}
   for(int i=0;i<8;i++){ // to understand the response, refer to page 15 on the control manual
      Serial.print(variable[i],HEX);
      Serial.print(";");}
       y=0;
    digitalWrite(3,LOW);
    delay(10000);
    digitalWrite(Reset, LOW);
  }
//-----------------Turn on the projector----------------------------------------------
  void TurnOn(){
client.write(commandOn,sizeof(commandOn));//turn off the projector
 delay(100);
for(int i=0;i<8;i++){
     variable[i] = client.read();}
    for(int i=0;i<8;i++){ // to understand the response, refer to page 15 on the control manual
      Serial.print(variable[i],HEX);
       Serial.print(";");}
         y=1;
    digitalWrite(3,HIGH);
  }
//---------------Get the status of the projector----------------------------------------------------------
void getStatus(){
client.write(StatusReq,sizeof(StatusReq));// get the on/ff status
    //delay(600);
 for(int i=0;i<22;i++){
      variable[i] = client.read();}
       Serial.println(" ");
    for(int i=0;i<22;i++){ // to understand the response, refer to page 15 on the control manual
      Serial.print(variable[i],HEX);
       Serial.print(";");
       client.flush();
  }
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
