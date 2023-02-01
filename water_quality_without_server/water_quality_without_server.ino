#include <Adafruit_GFX.h>

#include <LiquidCrystal_I2C.h>


//lcd_sensor_variables
int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x3F, lcdColumns, lcdRows);



//Led_variables
int led1 = 4; 
int led2 = 5; 
int led3 = 18; 
int led4 = 19; 
int led5 = 2; 
int buzzer =33;





//water_sensor_states

String  ph_state = "pH Value Good!";
String turb_state = "clear Water!";
String water_level = "Tank not filled!"; 






// ph sensor
float calibration_value = 20.24 - 0.7; //21.34 - 0.7
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10],temp;

float ph_act;





//ultrasonic_sensor_variables
#define trigpin 27  
#define echopin 14 
int duration, distance;




//turbidity sensor
int sensorPin = 25;



void setup()
{
  Serial.begin(9600);
  pinMode(trigpin, OUTPUT);
  pinMode(echopin, INPUT);

  pinMode(led1, OUTPUT); 
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(buzzer, OUTPUT);

  digitalWrite(led1, LOW); 
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  digitalWrite(led5, LOW);
  delay(200);




  //lcd_begin

  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
  // Initialize LCD and print
  lcd.setCursor(1, 0);
  lcd.print(" Water Quality "); 
  lcd.setCursor(3, 1);
  lcd.print(" Tester ");   
  delay(5000);
  lcd.clear();


  lcd.setCursor(1, 0);
  lcd.print(" Ejiro Promise "); 
  lcd.setCursor(2 , 1);
  lcd.print("COT/2107/2016 ");   
  delay(5000);
  lcd.clear();
  
  lcd.setCursor(1, 0);
  lcd.print(" Obote Jerry "); 
  lcd.setCursor(2, 1);
  lcd.print("COT/3638/2017 ");   
  delay(5000);
  lcd.clear();
  
  lcd.setCursor(2, 0);
  lcd.print("pH"); 
  lcd.setCursor(6, 0);
  lcd.print("Tub");
  lcd.setCursor(10, 0);
  lcd.print("Lvl");
  lcd.clear();

}




void loop() {

//  turbidity start
  int sensorValue = analogRead(sensorPin);
  float value = sensorValue*(5.0/1024.0);
  float turbidity = 20/value;
  Serial.println(turbidity);
  Serial.print("\t");
  Serial.print(turb_state  );
  Serial.print("\t");
  
  if (turbidity <= 1.00 ) {
      turb_state = "Clear water ";

  }
  else if (turbidity >= 1.50 ) {
//    alarm();
    turb_state = "Cloudy water ";

  }
 
//turbidity end



  //ph sensor start
  for(int i=0;i<10;i++) 
 { 
 buffer_arr[i]=analogRead(26);
 delay(30);
 }
 for(int i=0;i<9;i++)
 {
 for(int j=i+1;j<10;j++)
 {
 if(buffer_arr[i]>buffer_arr[j])
 {
 temp=buffer_arr[i];
 buffer_arr[i]=buffer_arr[j];
 buffer_arr[j]=temp;
 }
 }
 }
 avgval=0;
 for(int i=2;i<8;i++)
 avgval+=buffer_arr[i];
 float volt=(float)avgval*3.3/4096.0/6;  
 //Serial.print("Voltage: ");
 //Serial.println(volt);
  ph_act = -5.70 * volt + calibration_value;


   Serial.print("pH Val: ");
   Serial.println(ph_act);
   Serial.print("\t");
   Serial.print(ph_state );
   Serial.print("\t");
   

   if (ph_act <= 6.00  ) {
       ph_state  = "Water is acidic! ";
      

   }
    else if (ph_act >= 10.00 ) {
       ph_state  = "Water is akaline! ";
      

   }

   //ph sensor end














   //ultasonic sensor start
  digitalWrite(trigpin, HIGH);
  
  delayMicroseconds(1000);  
  digitalWrite(trigpin, LOW);
  
  
  duration = pulseIn(echopin,HIGH);
  
  distance = ( duration / 2) / 29.1;
  Serial.println("cm:");
   
  Serial.println(distance);
  //ytrasonic sensor end 


  delay(2000);


    
  if(  (distance > 0) && (distance <= 10)   ) 
  {
    digitalWrite(led1, HIGH); 
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH);
    digitalWrite(led5, HIGH);

    

    water_level = "tank full!";
   

    Serial.print("\t");
    Serial.print(water_level );
    Serial.print("\t");
    alarm();


     
  } else
  if(  (distance > 10) && (distance <= 15)  ) 
  {
  
    digitalWrite(led1, LOW); 
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH); 
  
  } else
  
  if(  (distance > 15) && (distance <= 20)  ) 
  {
  
    digitalWrite(led1, LOW); 
    digitalWrite(led2, LOW);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH);
  } else
  
  if(  (distance > 20) && (distance <= 25)  ) 
  {
  
    digitalWrite(led1, LOW); 
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH);
  } else
  
  if(  (distance > 25) && (distance <= 35)  ) 
  {
  
    digitalWrite(led1, LOW); 
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    digitalWrite(led5, HIGH);
  } else
  
  if(  distance > 60 ) 
  {
  
    digitalWrite(led1, LOW); 
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
  } 







  

  //ultrasonic sensor end




 //lcd dsisplay
      
  lcd.setCursor(1, 0);
  lcd.print("pH "); 

  
  lcd.setCursor(6, 0);
  lcd.print("Tub");
 
  
  lcd.setCursor(10, 0);
  lcd.print("Lvl-cm"); 



  lcd.setCursor(0, 1);
  lcd.print(ph_act); 

  lcd.setCursor(6, 1);
  lcd.print(turbidity ); 
  lcd.print(" ");

  lcd.setCursor(13, 1);
  lcd.print(distance);
  lcd.print(" ");
     
}

void alarm(){
   digitalWrite(buzzer, HIGH);
   delay(1000);
   digitalWrite(buzzer, LOW);
   delay(1000);
}
