#include <Metro.h>
#include <FlexCAN.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
//#define SSD1306_128_64;
Adafruit_MPU6050 mpu;
const String aa = String(char(229)); // å

#define OLED_DC     6
#define OLED_CS     10
#define OLED_RESET  5
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);

static CAN_message_t msg,msg1,rxmsg;
volatile uint32_t count = 0;
IntervalTimer TX_timer;
String CANStr(""); 
volatile uint32_t can_msg_count = 0;

void setup() {
Can0.begin(500000);  
Can1.begin(500000); 
 
  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
  
  delay(1000);
  Serial.println(F("DISPLAY CAN-BUS DETAILS AND IMU Z-VALUE"));
  display.setTextSize(0);
  delay(1000);
  display.clearDisplay();
  
  msg.buf[0] = 1;
  msg.buf[1] = 2;
  msg.buf[2] = 0;
  msg.buf[3] = 0;
  msg.buf[4] = 0;
  msg.buf[5] = 0;
  msg.buf[6] = 0;
  msg.len = 8;
  msg.id = 0x21;
  msg.flags.extended = 0; 
  msg.flags.remote = 0;
  
  msg1.buf[0] = 1;
  msg1.buf[1] = 2;
  msg1.buf[2] = 0;
  msg1.buf[3] = 0;
  msg1.buf[4] = 0;
  msg1.buf[5] = 0;
  msg1.buf[6] = 0;
  msg1.len = 8;
  msg1.id = 0x22;
  msg1.flags.extended = 0; 
  msg1.flags.remote = 0;
  
  TX_timer.begin(tx_CAN, 1000000);   

   Serial.begin(115200);
  while (!Serial)
    delay(10);

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: +-8G\n");

  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: +- 500 deg/s\n");

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: 21 Hz\n");

  Serial.println("");
  delay(100);
}

void tx_CAN(void)
{
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  msg.len = 8;
  msg.id = 0x21;
  msg.buf[0] = 1;
  msg.buf[1] = 3;
  msg.buf[2] = 0;
  msg.buf[3] = 0;
  msg.buf[4] = 0;
  msg.buf[5] = 0;
  msg.buf[6] = 0;
  msg.buf[7] = 0;

  msg1.len = 8;
  msg1.id = 0x22;
  msg1.buf[0] = a.acceleration.x;
  msg1.buf[1] = a.acceleration.y;
  msg1.buf[2] = a.acceleration.z;
  msg1.buf[3] = g.gyro.x;
  msg1.buf[4] = g.gyro.y;
  msg1.buf[5] = g.gyro.z;
  msg1.buf[6] = 0;
  msg1.buf[7] = 0;
  Can0.write(msg);
  Can0.write(msg1);
  Can1.write(msg);
  Can1.write(msg1);
  can_msg_count++;
}

void loop() {

sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  Serial.print("Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");
  
  Serial.println("");
  delay(500);

while(Can0.read(rxmsg))  { 
     String CANStr(""); 
     for (int i=0; i < 8; i++) {     

         CANStr += String(rxmsg.buf[i],HEX);
         CANStr += (" ") ;
     }
     Serial.print(rxmsg.id,HEX); 
     Serial.print(' '); 
     Serial.println(CANStr); 
} 

// DISPLAY
  display.clearDisplay();

  display.setTextSize(0);
  display.setTextColor(WHITE);

  display.setCursor(15,6);
  display.println("MAS245 - Gruppe 4");

  display.drawLine(8, 14, 120, 14, WHITE);
  display.drawRoundRect(1, 1, 127, 63, 4, WHITE);
    
  display.setCursor(8, 18);
  display.println("CAN-statistikk");
  
  display.setCursor(8, 23);
  display.println("--------------");
  
  display.setCursor(8, 29);
  display.println("Antall mottatt: ");
  
  display.setCursor(8, 39);
  display.println("Mottok sist ID: ");
  
  display.setCursor(8, 44);
  display.println("--------------");

  display.setCursor(8, 50);
  display.println("IMU z: ");

  display.setCursor(85, 50);
  display.println("m/s^2");

//Antall mottatt:
  display.setCursor(99, 29);
  display.println(can_msg_count);

//Mottok sist ID:
  display.setCursor(99, 39);
  display.println(rxmsg.id,HEX);

//IMU:
  display.setCursor(50, 50);
  display.println(a.acceleration.z);

  display.display();
}
