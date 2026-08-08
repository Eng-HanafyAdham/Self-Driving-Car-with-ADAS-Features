/*************ESP-NOW & Fire-BASE****************/
// Include Libraries
#include <esp_now.h>
#include <WiFi.h>

const char* ssid     = "your_network_name";
const char* password = "your_network_password";


// MAC Address of responder - edit as required
uint8_t broadcastAddress[] = {0x08,0xD1,0xF9,0xEC,0xF2,0x74};

// Define a data structure
typedef struct struct_message {
  char a;
  int dist;
} struct_message;

// Create a structured object
struct_message myData;
 
// Peer info
esp_now_peer_info_t peerInfo;
 
// Callback function called when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
/*************************/

int i = 0;
unsigned long int j =0;

const int EnableL = 13;
const int HighL = 12;       // LEFT SIDE MOTOR
const int LowL = 14;

const int EnableR = 25;
const int HighR =27;       //RIGHT SIDE MOTOR
const int LowR = 26;

const int D0 = 17;       //ESP32 pin 17    LSB
const int D1 = 16;       //ESP32 pin 16
const int D2 = 4;        //ESP32 pin 4
const int D3 = 2;        //ESP32 pin 2    MSB

int a, b, c, d, data;

//ultrasonic pin
const int trigPin = 18;
const int echoPin = 19;
long duration, distance;


void setup() {
 pinMode(D0, INPUT_PULLUP);
pinMode(D1, INPUT_PULLUP);
pinMode(D2, INPUT_PULLUP);
pinMode(D3, INPUT_PULLUP);

pinMode(EnableL, OUTPUT);
pinMode(HighL, OUTPUT);
pinMode(LowL, OUTPUT);


pinMode(EnableR, OUTPUT);
pinMode(HighR, OUTPUT);
pinMode(LowR, OUTPUT);


pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);

/************ESP-NOW*****************/
  // Set up Serial Monitor
  Serial.begin(115200);
 
  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
 
  // Initilize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
 
  // Register the send callback
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

/*****************************/
}

void Data()
{
   a = digitalRead(D0);
   b = digitalRead(D1);
   c = digitalRead(D2);
   d = digitalRead(D3);

   data = 8*d+4*c+2*b+a;
   Serial.printf("%i",data);
}

void Forward()
{
  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);
  analogWrite(EnableL,255);

  digitalWrite(HighR, LOW);
  digitalWrite(LowR, HIGH);
  analogWrite(EnableR,255);
  
}


void Backward()
{
  digitalWrite(HighL, HIGH);
  digitalWrite(LowL, LOW);
  analogWrite(EnableL,255);

  digitalWrite(HighR, HIGH);
  digitalWrite(LowR, LOW);
  analogWrite(EnableR,255);
  
}

void Stop()
{
  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);
  analogWrite(EnableL,0);

  digitalWrite(HighR, LOW);
  digitalWrite(LowR, HIGH);
  analogWrite(EnableR,0);
  
}

void Left1()
{
  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);
  analogWrite(EnableL,160);

  digitalWrite(HighR, LOW);
  digitalWrite(LowR, HIGH);
  analogWrite(EnableR,255);
  
}

void Left2()
{
  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);
  analogWrite(EnableL,90);

  digitalWrite(HighR, LOW);
  digitalWrite(LowR, HIGH);
  analogWrite(EnableR,255);
  
}


void Left3()
{
  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);
  analogWrite(EnableL,50);

  digitalWrite(HighR, LOW);
  digitalWrite(LowR, HIGH);
  analogWrite(EnableR,255);
  
}

void Right1()
{
  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);
  analogWrite(EnableL,255);

  digitalWrite(HighR, LOW);
  digitalWrite(LowR, HIGH);
  analogWrite(EnableR,160);  //200
  
}
void Right2()
{
  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);
  analogWrite(EnableL,255);

  digitalWrite(HighR, LOW);
  digitalWrite(LowR, HIGH);
  analogWrite(EnableR,90);   //160
  
}

void Right3()
{
  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);
  analogWrite(EnableL,255);

  digitalWrite(HighR, LOW);
  digitalWrite(LowR, HIGH);
  analogWrite(EnableR,50);   //100
  
}

void UTurn()
{
  analogWrite(EnableL, 0);
  analogWrite(EnableR, 0);
  delay(400);

  analogWrite(EnableL, 250);
  analogWrite(EnableR, 250);    //forward
  //delay(1000);
  delay(500);

  analogWrite(EnableL, 0);
  analogWrite(EnableR, 0);
  delay(400);

  digitalWrite(HighL, HIGH);
  digitalWrite(LowL, LOW);
  digitalWrite(HighR, LOW);   //   left
  digitalWrite(LowR, HIGH);
  analogWrite(EnableL, 255);
  analogWrite(EnableR, 255);
  //delay(700);
  delay(350);

  analogWrite(EnableL, 0);
  analogWrite(EnableR, 0);
  delay(400);

  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);
  digitalWrite(HighR, LOW);   // forward
  digitalWrite(LowR, HIGH);
  analogWrite(EnableL, 255);
  analogWrite(EnableR, 255);
  //delay(900);
  delay(450);

  analogWrite(EnableL, 0);
  analogWrite(EnableR, 0);
  delay(400);

  digitalWrite(HighL, HIGH);
  digitalWrite(LowL, LOW);
  digitalWrite(HighR, LOW);    //left
  digitalWrite(LowR, HIGH);
  analogWrite(EnableL, 255);
  analogWrite(EnableR, 255);
  //delay(700);
  delay(350);


  analogWrite(EnableL, 0);
  analogWrite(EnableR, 0);
  delay(1000);



  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);
  digitalWrite(HighR, LOW);
  digitalWrite(LowL, HIGH);
  analogWrite(EnableL, 150);
  analogWrite(EnableR, 150);
  delay(300);
}


void Object()
{

  analogWrite(EnableL, 0);
  analogWrite(EnableR, 0);            //stop
  delay(1000);

  digitalWrite(HighL, HIGH);
  digitalWrite(LowL, LOW);
  digitalWrite(HighR, LOW);
  digitalWrite(LowR, HIGH);        //left
  analogWrite(EnableL, 250);
  analogWrite(EnableR, 250);
  //delay(500);
  delay(250);

  analogWrite(EnableL, 0);
  analogWrite(EnableR, 0);            //stop
  delay(200);

  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);           //forward
  digitalWrite(HighR, LOW);
  digitalWrite(LowR, HIGH);
  analogWrite(EnableL, 255);
  analogWrite(EnableR, 255);
  //delay(1000);
  delay(500);

  analogWrite(EnableL, 0);           //stop
  analogWrite(EnableR, 0);
  delay(200);

  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);
  digitalWrite(HighR, HIGH);         //right
  digitalWrite(LowR, LOW);
  analogWrite(EnableL, 255);
  analogWrite(EnableR, 255);
  //delay(500);
  delay(250);

  analogWrite(EnableL, 0);               //stop
  analogWrite(EnableR, 0);
  delay(1000);

  
  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);
  digitalWrite(HighR, LOW);       // forward
  digitalWrite(LowR, HIGH);
  analogWrite(EnableL, 150);
  analogWrite(EnableR, 150);
  //delay(500);
  delay(250);

   i  = i+1;
}

void Lane_Change()
{

  analogWrite(EnableL, 0);
  analogWrite(EnableR, 0);            //stop
  delay(1000);

  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);
  digitalWrite(HighR, HIGH);
  digitalWrite(LowR, LOW);        //Right
  analogWrite(EnableL, 250);
  analogWrite(EnableR, 250);
  //delay(500);
  delay(250);

  analogWrite(EnableL, 0);
  analogWrite(EnableR, 0);            //stop
  delay(200);

  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);           //forward
  digitalWrite(HighR, LOW);
  digitalWrite(LowR, HIGH);
  analogWrite(EnableL, 255);
  analogWrite(EnableR, 255);
  //delay(800);
  delay(400);

  analogWrite(EnableL, 0);           //stop
  analogWrite(EnableR, 0);
  delay(200);

  digitalWrite(HighL, HIGH);
  digitalWrite(LowL, LOW);
  digitalWrite(HighR, LOW);         //LEFT
  digitalWrite(LowR, HIGH);
  analogWrite(EnableL, 255);
  analogWrite(EnableR, 255);
  //delay(500);
  delay(250);

  analogWrite(EnableL, 0);               //stop
  analogWrite(EnableR, 0);
  delay(1000);

  
  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);
  digitalWrite(HighR, LOW);       // forward
  digitalWrite(LowR, HIGH);
  analogWrite(EnableL, 150);
  analogWrite(EnableR, 150);
  delay(500);


}


void loop() 
{

    //ultrasonic
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;

  Data();
  
  myData.a=data;
  myData.dist=distance;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sending confirmed");
  }
  else {
    Serial.println("Sending error");
  }
  if(distance > 13)
  {

 
    if (j > 25000)
    {
      Lane_Change();
      i = 0;
      j = 0;
    }
  
  
  if(data==0)
   {
     Forward();
     if (i>0)
     {
      j = j+1;
     }
   }
   
  else if(data==1)
   {
     Right1();
          if (i>0)
     {
      j = j+1;
     }
   }
     
  else if(data==2)
   {
     Right2();
          if (i>0)
     {
      j = j+1;
     }
   }
     
  else if(data==3)
   {
     Right3();
          if (i>0)
     {
      j = j+1;
     }
   }
     
  else if(data==4)
   {
     Left1();
          if (i>0)
     {
      j = j+1;
     }
   }
    
  else if(data==5)
   {
     Left2();
          if (i>0)
     {
      j = j+1;
     }
   }
    
  else if(data==6)
   {
     Left3();
          if (i>0)
     {
      j = j+1;
     }
   }
     
  else if(data==7)//lane end
   {
     UTurn();
   }
  
  else if (data==8) //stop sign
   {
      analogWrite(EnableL, 0);
      analogWrite(EnableR, 0);
      delay(4000);

      analogWrite(EnableL, 150);
      analogWrite(EnableR, 150);
      delay(1000);
   }

      else if(data==9)// object detection
   {
     Object();
   }

         else if(data==10)//red traffic light 
   {
      analogWrite(EnableL, 0);
      analogWrite(EnableR, 0);
      delay(2000);
   }


     else if(data>10)
   {
     Stop();
   }


    }
    else
    {
      //emergency brake distance <13
      Stop();
    }


}
