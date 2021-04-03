#include <ArduinoJson.h>
#include <HttpPacket.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <SimpleDHT.h>
#include <dht11.h>
HttpPacketHead packet;
#define myPeriodic 5
#define Relay 16  // 控制继电器
#define Moisture A0 // 获取土壤湿度

dht11 DHT11;
const char ssid[] = "Redmi_B530";     //修改为自己的路由器用户名
const char password[] = "he2068375"; //修改为自己的路由器密码
#define mqtt_server "183.230.40.39"
#define mqtt_port 6002
char OneNetServer[] = "api.heclouds.com";       //不需要修改
#define mqtt_ClientIdentifier "694205832"
#define mqtt_UserName "409370"
#define mqtt_UserPassword "O=C30hI6pbbe=uZV1p=sxj0TM40="
long lastMsg = 0;
char msg_buf[200];
char sensor_id1[] = "TEMP";
char sensor_id2[] = "HUMI";
char sensor_id3[] = "SOILHUMI";
char dataTemplete[] = "{\"Temp\":$d,\"Hum\":$d},\"switch\":$d}";
char msgJson[75];
int switch0 = 1;//开关状态
WiFiClient espClient;
#define DHT11PIN 2
PubSubClient client(espClient);
void callback(char *topic, byte *payload, unsigned int length)
{
  
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }
    Serial.println();

    // 平台发来命令“1”时开灯
    if ((char)payload[0] == '1')
    { 
      Serial.print("11111111111111111111111111111111111111111111111111111111");
    switch0 = 1;
    
    }
    else
    {
       switch0 = 0;
       Serial.print("222222222222222222222222222222222");
    }
    
}
void reconnect (){
  while(!client.connected()){
    if(client.connect(mqtt_ClientIdentifier,mqtt_UserName,mqtt_UserPassword)){
      }else {
    Serial.print(client.state());
    delay(30000);
        }
    } 
  }


void postDataToOneNet(char* API_VALUE_temp, char* mqtt_ClientIdentifier_temp, char* sensor_id_temp, float thisData)
{  WiFiClient client;
  StaticJsonBuffer<250> jsonBuffer;
  JsonObject& myJson = jsonBuffer.createObject();
  JsonArray& datastreams= myJson.createNestedArray("datastreams");
  JsonObject& id_datapoints = datastreams.createNestedObject();
  id_datapoints["id"] = sensor_id_temp;
  JsonArray& datapoints = id_datapoints.createNestedArray("datapoints");
  JsonObject& value = datapoints.createNestedObject();
    value["value"] =thisData;
  char p[180];
  myJson.printTo(p, sizeof(p));
  packet.setHostAddress(OneNetServer);
  packet.setDevId(mqtt_ClientIdentifier_temp);   //mqtt_ClientIdentifier
  packet.setAccessKey(API_VALUE_temp);  //mqtt_UserPassword 
  /*create the http message about add datapoint */
  packet.createCmdPacket(POST, TYPE_DATAPOINT, p);
  if (strlen(packet.content))
  Serial.print(packet.content);
  Serial.println(p);
  char cmd[400];
  memset(cmd, 0, 400);  
  strcpy(cmd, packet.content);
  strcat(cmd, p);
  if (client.connect(OneNetServer, 80)) {
    client.print(cmd);
    delay(1000);
  }//end if
  //  Serial.println(cmd);
  client.stop();
}

void getData(){
    // 温湿度
  double temp;
  double hum;
  double SH=0;
  SH = analogRead(Moisture);
  Serial.print('turan');
  Serial.println(SH);
//  digitalWrite(4, switch0);

  int chk = DHT11.read(DHT11PIN);
  switch (chk)
  {
    case DHTLIB_OK: 
                ; 
                break;
    case DHTLIB_ERROR_CHECKSUM: 
            
                break;
    case DHTLIB_ERROR_TIMEOUT: 
               
                break;
    default: 
              
                break;
  }
  temp=DHT11.temperature;
  hum=DHT11.humidity;
  //发送数据到Onenet
  postDataToOneNet(mqtt_UserPassword , mqtt_ClientIdentifier, sensor_id1, temp);
  postDataToOneNet(mqtt_UserPassword , mqtt_ClientIdentifier, sensor_id2, hum);
  postDataToOneNet(mqtt_UserPassword , mqtt_ClientIdentifier, sensor_id3, SH);
  postDataToOneNet(mqtt_UserPassword , mqtt_ClientIdentifier, "SWITCH", switch0);
  }
void setup() {
  pinMode(4, OUTPUT);
  Serial.begin(115200);
  client.setCallback(callback);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");  
  }
   Serial.print(WiFi.localIP());  
  client.setServer(mqtt_server,mqtt_port);
  client.publish("$dp",(uint8_t*)msg_buf, 3+ strlen(msgJson));
}
void loop() {
  if(!client.connected()){
      Serial.print("zxxxzxxzxxzxxzxx");  
    reconnect();
    }
  client.loop();
  getData();

}
  
  
