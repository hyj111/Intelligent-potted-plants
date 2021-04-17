 1 /**
  2  *
  3  */
  4 #include <TimerOne.h>
  5 #include <HttpPacket.h>
  6 #include <ArduinoJson.h>
  7 /**'
  8  * ESP8266 采用串口3通讯
  9  */
 10 #define ESP8266Serial Serial3
 11 #define mySerial Serial
 12 
 13 #define RES_MAX_LENGTH 500
 14 
 15 #define SEND_SUCCESS 1
 16 #define SEND_FAIL 0
 17 
 18 /**
 19 * 链接的AP的账户密码
 20 */
 21 char ssid[] = "zlei";
 22 char pass[] = "18829237005";
 23 
 24 /**
 25  * Onenet 互联网信息
 26  */
 27 char oneNetServer[] = "api.heclouds.com";       //不需要修改
 28 char deviceId[] = "10005241";
 29 char API_KEY[] = "qB40IZOP6Rx58EZ7ow16mA=VkRQ=";
 30 char sensorID1[] = "waterLevel";
 31 
 32 
 33 // ESP8266于Arduino交互的数据缓存
 34 char ESP8266Response[RES_MAX_LENGTH];
 35 
 36 // 交互数据缓存的世纪存储长度
 37 int resLength = 0;
 38 
 39 // 请求时间计算的最小分度值
 40 const unsigned long interval = 1000;
 41 
 42 // 计时器，以毫秒为最小计数单位
 43 unsigned long curTime = 0;
 44 
 45 HttpPacketHead packet;
 46 void setup() {
 47   ESP8266Serial.begin(115200);
 48   mySerial.begin(9600);
 49 
 50   Timer1.initialize(interval);
 51   Timer1.attachInterrupt(timeClock);
 52   intiESP8266();
 53 
 54 }
 55 
 56 void loop() {
 57   postDataToServer(API_KEY,deviceId,sensorID1,1.00); // 发送数据（数据为模拟数据）
 58   delay(10000);
 59 }
 60 
 61 /**
 62 * 制造重启命令
 63 */
 64 void(* resetFunc) (void) = 0;
 65 
 66 /**
 67  * ESP8266 板子进行初始化
 68  * 1. 发送 AT 指令判断供电，TTL和WIFI模块是否正常 返回OK表示正常
 69  * 2. 发送 AT+CWMODE=? 指令 选择模式 1.Station模式，2. AP 模式
 70  * 3. 发送 AT+CWJAP="ssid","password" 来连接AP
 71  */
 72 void intiESP8266(){
 73   
 74   // AT 指令， 返回OK表明供电，TTL和WIFI模块通讯正常
 75   if(sendATCommead("AT\r\n","OK", 1000, 2) == SEND_SUCCESS );
 76   else ESP8266_Error(1);
 77   delay(100);
 78   // AT+CWMODE=? 选择模式 1.Station模式，2. AP 模式
 79   if(sendATCommead("AT+CWMODE=1\r\n","OK",1000,3) == SEND_SUCCESS );
 80   else ESP8266_Error(2);
 81 
 82   // AT+CWJAP="sid","pass" 加入AP
 83   char cwjap[50];
 84   strcat(cwjap,"AT+CWJAP=\"");
 85   strcat(cwjap,ssid);
 86   strcat(cwjap,"\",\"");
 87   strcat(cwjap,pass);
 88   strcat(cwjap,"\"\r\n");
 89   if(sendATCommead(cwjap,"OK",2000,4) == SEND_SUCCESS );
 90   else ESP8266_Error(3);
 91 }
 92 
 93 /**
 94  * ESP8266 错误信息处理
 95  * @param type 错误信息类型
 96  */
 97 void ESP8266_Error(int type){
 98   mySerial.println("ERROR");
 99   mySerial.println(type);
100   while(1){
101     if(sendATCommead("AT\r\n","OK", 1000, 2) == SEND_SUCCESS){
102       mySerial.print("\r\nRESET!!!!!!\r\n");
103       resetFunc();
104     }
105   }
106 }
107 
108 /**
109  * Arduino 向ESP8266中发送AT指令
110  * @param commoned 需要发送的指令
111  * @param response 发送之后接收到response之后表示成功
112  * @param timeout 在发送命令之后 timeout 毫秒之后重新发送消息
113  * @param reTry 重新发送的次数
114  */
115 unsigned int sendATCommead(char *commoned, char *response, int timeout, unsigned char reTry){
116 
117   // 多次重复发送消息，直到接收到消息。
118   for(unsigned char tryTime = 0; tryTime < reTry; tryTime++){
119     clearResponse(); // 清理接收缓存
120     mySerial.print("send commoned ");
121     mySerial.print(commoned);
122     mySerial.print(" to ESP8266.\n");
123     ESP8266Serial.write(commoned); // 通过串口发送命令
124     curTime = 0;
125     
126     // 在延时范围内查看回复消息，如果回复消息成功，返回成功
127     while(curTime < timeout){
128       readResponse();
129       if(strstr(ESP8266Response, response) != NULL){
130           mySerial.println("Response is ");
131           mySerial.println("-------------- Response Start ----------------");
132           mySerial.println(ESP8266Response);
133           mySerial.println("-------------- Response end ------------------");
134           return SEND_SUCCESS;
135       }
136     }
137   }
138   // 发送失败
139   mySerial.print("response received, content is \n >>>> ");
140   mySerial.println(ESP8266Response);
141   return SEND_FAIL;
142 }
143 
144 /**
145  * 向服务器发送数据请求
146  * @param apiKey 访问网站的apiKey
147  * @param deviceID 设备ID
148  * @param sensorID 传感器ID
149  * @param data 发送的数据
150  */
151 void postDataToServer(char *apiKey, char *deviceID, char *sensorID, double data){
152   StaticJsonBuffer<400> jsonBuffer;
153 
154   JsonObject &value = jsonBuffer.createObject();
155   value["value"] = data;
156 
157   JsonObject &idDataPoints = jsonBuffer.createObject();
158   idDataPoints["id"] = sensorID;
159   JsonArray &dataPoints = idDataPoints.createNestedArray("dataPoints");
160   dataPoints.add(value);
161 
162   JsonObject &myJson = jsonBuffer.createObject();
163   JsonArray &dataStreams = myJson.createNestedArray("dataStreams");
164   dataStreams.add(idDataPoints);
165 
166   // 设置POST的请求表头
167   char postData[400];
168   int postLength = myJson.printTo(postData, sizeof(postData));
169   
170   packet.setHostAddress(oneNetServer);
171   packet.setDevId(deviceId);
172   packet.setAccessKey(API_KEY);
173 
174   packet.createCmdPacket(POST, TYPE_DATAPOINT, postData);
175 
176   int httpLength = strlen(packet.content) + postLength;
177   
178   char cmd[500];
179   // 链接服务器
180   memset(cmd, 0, 500);
181   sprintf(cmd, "AT+CIPSTART=\"TCP\",\"%s\",80\r\n",oneNetServer);
182   if(sendATCommead(cmd,"CONNECT", 3000, 5) == SEND_SUCCESS);
183   else ESP8266_Error(4);
184 
185   // 发送数据
186   memset(cmd,0,500);
187   sprintf(cmd, "AT+CIPSEND=%d\r\n",httpLength);
188   if(sendATCommead(cmd,">",3000,3) == SEND_SUCCESS);
189   else ESP8266_Error(5);
190 
191   memset(cmd,0,500);
192   strcpy(cmd,packet.content);
193   strcat(cmd,postData);
194   if(sendATCommead(cmd,"succ\"}",5000,5) == SEND_SUCCESS);
195   else ESP8266_Error(6);
196 
197   if(sendATCommead("AT+CIPCLOSE\r\n","CLOSED",3000,3) == SEND_SUCCESS);
198   else ESP8266_Error(7);
199 }
200 
201 /**
202  * @TODO 将ESP8266返回的数据返回到缓存数组中
203  * 1. 清理缓存数据
204  * 2. 将读取的数据存入全局数组中
205  * 3. 超出数组长度之后
206  */
207 void readResponse(){
208   clearResponse();
209   while(ESP8266Serial.available()){
210     ESP8266Response[resLength++] = ESP8266Serial.read();
211     if(resLength == RES_MAX_LENGTH) clearResponse();
212   }
213 }
214 
215 /**
216  * @todo 清理数据交互缓存区
217  */
218 void clearResponse(){
219   memset(ESP8266Response,0,RES_MAX_LENGTH);
220   resLength = 0;
221 }
222 
223 /**
224  * 计时器函数，单位为毫秒
225  */
226 void timeClock(){
227   curTime++;
228 }