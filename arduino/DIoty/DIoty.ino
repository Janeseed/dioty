// Change this to your unique names
#define PROTOTYPE_NAME "MyDevice"
#define SERVER_NAME "MyServer"
// Change this to your server PC IP
#define HOST_SERVER "143.248.250.173"

// Change this value to control data sending frequency
// e.g., 50: once every 50ms
const long interval = 50; 


#include <SocketIoClient.h>
#include <ArduinoJson.h>
SocketIoClient socket;

// Include libraries and declare global variables BELOW

int relay = 13; // 릴레이가 연결된 핀 번호
int switchState = true;
int onoff = 14;
int val = 0;

//wifi 리셋 위함
int rst = 0; //리셋 버튼 0번 핀에 배치
long longPressTime = 250; // 2.5이상 눌러야 reset

void setup() {
  Serial.begin(115200);      
  while(smartWiFi(PROTOTYPE_NAME));  
  launchWeb(1);
  socket.on(SERVER_NAME, event); //event 정의
  socket.begin(HOST_SERVER);
  
  // Pin setup HERE
  pinMode(relay, OUTPUT);
  pinMode(onoff, INPUT);
  pinMode(rst, INPUT_PULLUP);
  digitalWrite(relay, 0);

}


void loop() {  
  mdns_setup();  
  unsigned long currentMillis = millis();  
  unsigned long previousMillis = 0;
  Serial.print(digitalRead(rst));
  if (digitalRead(rst) == HIGH) {
    
    if (currentMillis - previousMillis >= longPressTime) {
      previousMillis = currentMillis;
      reset();
      DynamicJsonDocument data_to_web(1024);

      // configure the json array of data HERE
      // i.e., data_to_web["data_name"] = data    
      data_to_web["resetStatus"] = true;
      
      String jsonData;
      serializeJson(data_to_web, jsonData);    
      socket.emit(PROTOTYPE_NAME, jsonData.c_str());
    }
  }
    
  //onoff 스위치의 값 지정
  val = digitalRead(onoff);
  //physical switch가 off 상태이면 relay를 켜진 상태로 유지
  if(val == 0){
    digitalWrite(relay, 1);
  }
  
  socket.loop();  
  
  // Loop code HERE (NO DELAYS ALLOWED)    
  
}


// Receiving data from web
void event(const char * data, size_t length) {
  if (val == 1) {
    DynamicJsonDocument data_from_web(1024);
    deserializeJson(data_from_web, data);
  
    // Parse your received json data from here
    // i.e., data = data_from_web["data_name"];

    bool switchState = data_from_web["switch"];
  
    // Use your parsed data HERE (NO DELAYS ALLOWED)   
    // e.g.,
  
    digitalWrite(relay,switchState); //relay로 바꿔야 함.
  }
}
