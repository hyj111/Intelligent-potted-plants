int relayPin = 16; //继电器引脚
int incomedate;
 
void setup()
 {
    pinMode(relayPin, OUTPUT);
    //设置引脚13为输出接口，Arduino 上我们用到的I/O 口都要进行类似这样的定义。  
    Serial.begin(9600); 
    //设置波特率为9600，这里要跟软件设置相一致。当接入特定设备时，我们也要跟其他设备的波特率达到一致。  
}
 
void loop() 
{
  if (Serial.available() > 0)//串口接收到数据
  {
    incomedate = Serial.read();//获取串口接收到的数据
    if (incomedate == 'L')
    {
      Serial.println("pump start!");
      digitalWrite(relayPin, LOW);
    } 
    else if (incomedate == 'H')
    {
      Serial.println("pump stop!");
      digitalWrite(relayPin, HIGH);
    }
  }
}
