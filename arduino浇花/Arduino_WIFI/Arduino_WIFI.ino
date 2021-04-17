/**
*
*/
#include <TimerOne.h>
#include <HttpPacket.h>
#include <ArduinoJson.h>
/**'
* ESP8266 采用串口3通讯
*/
#define ESP8266Serial Serial3
#define mySerial Serial

#define RES_MAX_LENGTH 500

#define SEND_SUCCESS 1
#define SEND_FAIL 0

/**
* 链接的AP的账户密码
*/
char ssid[] = "zlei";
char pass[] = "18829237005";

/**
* Onenet 互联网信息
*/
char oneNetServer[] = "api.heclouds.com";       //不需要修改
char deviceId[] = "10005241";
char API_KEY[] = "qB40IZOP6Rx58EZ7ow16mA=VkRQ=";
char sensorID1[] = "waterLevel";


// ESP8266于Arduino交互的数据缓存
char ESP8266Response[RES_MAX_LENGTH];

// 交互数据缓存的世纪存储长度
int resLength = 0;

// 请求时间计算的最小分度值
const unsigned long interval = 1000;

// 计时器，以毫秒为最小计数单位
unsigned long curTime = 0;

HttpPacketHead packet;
void setup() {
ESP8266Serial.begin(115200);
mySerial.begin(9600);

Timer1.initialize(interval);
Timer1.attachInterrupt(timeClock);
intiESP8266();

}

void loop() {
postDataToServer(API_KEY,deviceId,sensorID1,1.00); // 发送数据（数据为模拟数据）
delay(10000);
}

/**
* 制造重启命令
*/
void(* resetFunc) (void) = 0;

/**
* ESP8266 板子进行初始化
* 1. 发送 AT 指令判断供电，TTL和WIFI模块是否正常 返回OK表示正常
* 2. 发送 AT+CWMODE=? 指令 选择模式 1.Station模式，2. AP 模式
* 3. 发送 AT+CWJAP="ssid","password" 来连接AP
*/
void intiESP8266(){

// AT 指令， 返回OK表明供电，TTL和WIFI模块通讯正常
if(sendATCommead("AT\r\n","OK", 1000, 2) == SEND_SUCCESS );
else ESP8266_Error(1);
delay(100);
// AT+CWMODE=? 选择模式 1.Station模式，2. AP 模式
if(sendATCommead("AT+CWMODE=1\r\n","OK",1000,3) == SEND_SUCCESS );
else ESP8266_Error(2);

// AT+CWJAP="sid","pass" 加入AP
char cwjap[50];
strcat(cwjap,"AT+CWJAP=\"");
strcat(cwjap,ssid);
strcat(cwjap,"\",\"");
strcat(cwjap,pass);
strcat(cwjap,"\"\r\n");
if(sendATCommead(cwjap,"OK",2000,4) == SEND_SUCCESS );
else ESP8266_Error(3);
}

/**
* ESP8266 错误信息处理
* @param type 错误信息类型
*/
void ESP8266_Error(int type){
mySerial.println("ERROR");
mySerial.println(type);
while(1){
if(sendATCommead("AT\r\n","OK", 1000, 2) == SEND_SUCCESS){
mySerial.print("\r\nRESET!!!!!!\r\n");
resetFunc();
}
}
}

/**
* Arduino 向ESP8266中发送AT指令
* @param commoned 需要发送的指令
* @param response 发送之后接收到response之后表示成功
* @param timeout 在发送命令之后 timeout 毫秒之后重新发送消息
* @param reTry 重新发送的次数
*/
unsigned int sendATCommead(char *commoned, char *response, int timeout, unsigned char reTry){

// 多次重复发送消息，直到接收到消息。
for(unsigned char tryTime = 0; tryTime < reTry; tryTime++){
clearResponse(); // 清理接收缓存
mySerial.print("send commoned ");
mySerial.print(commoned);
mySerial.print(" to ESP8266.\n");
ESP8266Serial.write(commoned); // 通过串口发送命令
curTime = 0;

// 在延时范围内查看回复消息，如果回复消息成功，返回成功
while(curTime < timeout){
readResponse();
if(strstr(ESP8266Response, response) != NULL){
mySerial.println("Response is ");
mySerial.println("-------------- Response Start ----------------");
mySerial.println(ESP8266Response);
mySerial.println("-------------- Response end ------------------");
return SEND_SUCCESS;
}
}
}
// 发送失败
mySerial.print("response received, content is \n >>>> ");
mySerial.println(ESP8266Response);
return SEND_FAIL;
}

/**
* 向服务器发送数据请求
* @param apiKey 访问网站的apiKey
* @param deviceID 设备ID
* @param sensorID 传感器ID
* @param data 发送的数据
*/
void postDataToServer(char *apiKey, char *deviceID, char *sensorID, double data){
StaticJsonBuffer<400> jsonBuffer;

JsonObject &value = jsonBuffer.createObject();
value["value"] = data;

JsonObject &idDataPoints = jsonBuffer.createObject();
idDataPoints["id"] = sensorID;
JsonArray &dataPoints = idDataPoints.createNestedArray("dataPoints");
dataPoints.add(value);

JsonObject &myJson = jsonBuffer.createObject();
JsonArray &dataStreams = myJson.createNestedArray("dataStreams");
dataStreams.add(idDataPoints);

// 设置POST的请求表头
char postData[400];
int postLength = myJson.printTo(postData, sizeof(postData));

packet.setHostAddress(oneNetServer);
packet.setDevId(deviceId);
packet.setAccessKey(API_KEY);

packet.createCmdPacket(POST, TYPE_DATAPOINT, postData);

int httpLength = strlen(packet.content) + postLength;

char cmd[500];
// 链接服务器
memset(cmd, 0, 500);
sprintf(cmd, "AT+CIPSTART=\"TCP\",\"%s\",80\r\n",oneNetServer);
if(sendATCommead(cmd,"CONNECT", 3000, 5) == SEND_SUCCESS);
else ESP8266_Error(4);

// 发送数据
memset(cmd,0,500);
sprintf(cmd, "AT+CIPSEND=%d\r\n",httpLength);
if(sendATCommead(cmd,">",3000,3) == SEND_SUCCESS);
else ESP8266_Error(5);

memset(cmd,0,500);
strcpy(cmd,packet.content);
strcat(cmd,postData);
if(sendATCommead(cmd,"succ\"}",5000,5) == SEND_SUCCESS);
else ESP8266_Error(6);

if(sendATCommead("AT+CIPCLOSE\r\n","CLOSED",3000,3) == SEND_SUCCESS);
else ESP8266_Error(7);
}

/**
* @TODO 将ESP8266返回的数据返回到缓存数组中
* 1. 清理缓存数据
* 2. 将读取的数据存入全局数组中
* 3. 超出数组长度之后
*/
void readResponse(){
clearResponse();
while(ESP8266Serial.available()){
ESP8266Response[resLength++] = ESP8266Serial.read();
if(resLength == RES_MAX_LENGTH) clearResponse();
}
}

/**
* @todo 清理数据交互缓存区
*/
void clearResponse(){
memset(ESP8266Response,0,RES_MAX_LENGTH);
resLength = 0;
}

/**
* 计时器函数，单位为毫秒
*/
void timeClock(){
curTime++;
}
