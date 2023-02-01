#include <Wire.h>

#include <WebServer.h>
#include <OneWire.h>

#include <LiquidCrystal_I2C.h>


//ph_sensor_variables
float calibration_value = 20.24 - 0.7; //21.34 - 0.7
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10],temp;
float ph_act;


//tubidity_sensor_variables
const int turbpin = 25;
int buf[10];
float voltage;
float turbidity =voltage;


//webserver units
#define REPORTING_PERIOD_MS    100
uint32_t lastReportTime = 0;
uint32_t tsLastReport = 0;
const char* ssid = "Oghenemaega’s iPhone";  // Enter SSID here
const char* password = "maega242";  //Enter Password here
WebServer server(80);  


//lcd_sensor_variables
int lcdColumns = 20;
int lcdRows = 4;
LiquidCrystal_I2C lcd(0x3F, lcdColumns, lcdRows);



//ultrasonic_sensor_variables
#define trigpin 27  
#define echopin 14 
int duration, distance;


//Led_variables
int led1 = 4; 
int led2 = 5; 
int led3 = 18; 
int led4 = 19; 
int led5 = 2; 
int buzzer =33;



//water_sensor_states

String  ph_state = "pH is OKay!";
String turb_state = "clear Water!";
String water_level = "Tank not filled!"; 







void setup(){


//  pinMode(pH, INPUT);
  pinMode(turbpin, INPUT);
  Serial.begin(115200);



  //for water level monitor
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
  lcd.print("pH:"); 
  lcd.setCursor(8, 0);
  lcd.print("Tub:");
  lcd.setCursor(0, 1);
  lcd.print("Water Lvl:");
  lcd.clear();
//  lcd.setCursor(4, 2);
//  lcd.print(" Tester ");    
  





  Serial.println("Connecting to ");
  Serial.println(ssid);
 
  //connect to your local wi-fi network
  WiFi.begin(ssid, password);
 
  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());
 
  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);
 
  server.begin();
  Serial.println("HTTP server started");
 




  
}
void loop(){

  server.handleClient();

    //code for ph sensor

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
   float phv=ph_act-10.00;
   Serial.println(phv);
    if (phv <= 4.00 ) {
       ph_state  = "Water is acidic! ";

   }
    else if (phv >= 10.00 ) {
       ph_state  = "Water is akaline! ";

   }
  
   
   delay(1000);

 



















  

  //code for turbidity sensor
//  int sensorValue = analogRead(turbpin);
//  int turbidity = map(sensorValue, 0,640, 100, 0);
//
//  Serial.println("turbidity:");
//  turbidity*(5.0/1024.0)*3;


  int sensorValue = analogRead(turbpin);// read the input on analog pin A0:
  float value = sensorValue*(5.0/1024.0);
  float turbidity = value;
  
  
  Serial.println(turbidity);
  delay(200);


    if (turbidity <= 1.00 ) {
      turb_state = "Cloudy water ";

  }
 
   
  Serial.println(turbidity);
  delay(200);
  
  
 










   //for the water level

  digitalWrite(trigpin, HIGH);
  
  delayMicroseconds(1000);  
  digitalWrite(trigpin, LOW);
  
  
  duration = pulseIn(echopin,HIGH);
  
  distance = ( duration / 2) / 29.1;
  Serial.println("cm:");
   
  Serial.println(distance);
  delay(200);



  
  
  if(  (distance > 0) && (distance <= 10)   ) 
  {
    digitalWrite(led1, HIGH); 
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH);
    digitalWrite(led5, HIGH);
    digitalWrite(buzzer, HIGH);
    

    water_level = "tank full!";
     
  } else
  if(  (distance > 10) && (distance <= 20)  ) 
  {
  
    digitalWrite(led1, LOW); 
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH); 
  
  } else
  
  if(  (distance > 20) && (distance <= 30)  ) 
  {
  
    digitalWrite(led1, LOW); 
    digitalWrite(led2, LOW);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH);
  } else
  
  if(  (distance > 30) && (distance <= 40)  ) 
  {
  
    digitalWrite(led1, LOW); 
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH);
  } else
  
  if(  (distance > 50) && (distance <= 60)  ) 
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




if (millis() - lastReportTime > REPORTING_PERIOD_MS) {
  //code_for_lcd_display

  lcd.setCursor(0, 0);
  lcd.print("pH:"); 
  lcd.print(ph_act); 
  
  lcd.setCursor(8, 0);
  lcd.print("Tub:");
  lcd.print(turbidity ); 
  
  lcd.setCursor(0, 1);
  lcd.print("Water Lvl:");
  lcd.print(distance); 
  lcd.print("cm  "); 


  
        Serial.println("*********************************");
        Serial.println();
        
       lastReportTime = millis();
  
}


}

void handle_OnConnect() {
  
  server.send(200, "text/html", SendHTML( (String)ph_act, (String)turbidity, (String)distance, (String)ph_state, (String)turb_state, (String)water_level )); 
}
 
void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}
String SendHTML(String  ph_act, String  ph_state, String turbidity, String distance, String turb_state,String  water_level) {
  String html = "<!DOCTYPE html>";
html += "<html>";
html += "<head>";
html += "<title>Water Purity tester</title>";
html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
html += "<link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.7.2/css/all.min.css'>";
html += "<link rel='stylesheet' type='text/css' href='styles.css'>";
html += "<style>";
html += "body { background-color: #fff; font-family: sans-serif; color: #333333; font: 14px Helvetica, sans-serif box-sizing: border-box;}";
html += "#page { margin: 20px; background-color: #fff;}";
html += ".container { height: inherit; padding-bottom: 20px;}";
html += ".header { padding: 20px;}";
html += ".header h1 { padding-bottom: 0.3em; color: #008080; font-size: 45px; font-weight: bold; font-family: Garmond, 'sans-serif'; text-align: center;}";
html += "h2 { padding-bottom: 0.2em; border-bottom: 1px solid #eee; margin: 2px; text-align: left;}";
html += ".header h3 { font-weight: bold; font-family: Arial, 'sans-serif'; font-size: 17px; color: #b6b6b6; text-align: center;}";
html += ".box-full { padding: 20px; border 1px solid #ddd; border-radius: 1em 1em 1em 1em; box-shadow: 1px 7px 7px 1px rgba(0,0,0,0.4); background: #fff; margin: 20px; width: 300px;}";
html += "@media (max-width: 494px) { #page { width: inherit; margin: 5px auto; } #content { padding: 1px;} .box-full { margin: 8px 8px 12px 8px; padding: 10px; width: inherit;; float: none; } }";
html += "@media (min-width: 494px) and (max-width: 980px) { #page { width: 465px; margin 0 auto; } .box-full { width: 380px; } }";
html += "@media (min-width: 980px) { #page { width: 930px; margin: auto; } }";
html += ".sensor { margin: 12px 0px; font-size: 2.5rem;}";
html += ".sensor-labels { font-size: 1rem; vertical-align: middle; padding-bottom: 15px;}";
html += ".units { font-size: 1.2rem;}";
html += "hr { height: 1px; color: #eee; background-color: #eee; border: none;}";
html += "</style>";

//Ajax Code Start
  html += "<script>\n";
  html += "setInterval(loadDoc,1000);\n";
  html += "function loadDoc() {\n";
  html += "var xhttp = new XMLHttpRequest();\n";
  html += "xhttp.onreadystatechange = function() {\n";
  html += "if (this.readyState == 4 && this.status == 200) {\n";
  html += "document.body.innerHTML =this.responseText}\n";
  html += "};\n";
  html += "xhttp.open(\"GET\", \"/\", true);\n";
  html += "xhttp.send();\n";
  html += "}\n";
  html += "</script>\n";
  //Ajax Code END
  
html += "</head>";
html += "<body>";
html += "<div id='page'>";
html += "<div class='header'>";
html += "<h2>Water Purity tester by Mjoghenemaega </h2>";
html += "</div>";
html += "<div id='content' align='center'>";
html += "<div class='box-full' align='left'>";
html += "<h2>Water sensor Readings</h2>";
html += "<div class='sensors-container'>";

//For ph level
html += "<div class='sensors'>";
html += "<p class='sensor'>";
html += "<i class='fas fa-thermometer-half' style='color:#0275d8'></i>";
html += "<span class='sensor-labels'>pH level </span>"; 
html += ph_act;
html += ph_state;
//html += "<sup class='units'>  -- ph_state</sup>";
html += "</p>";
html += "<hr>";
html += "</div>";

//For turbidity
html += "<div class='sensors'>";
html += "<p class='sensor'>";
html += "<i class='fas fa-tint' style='color:#5bc0de'></i>";
html += "<span class='sensor-labels'> Water Turbidity </span>"; 
html += turbidity;
html += turb_state;
//html += "<sup class='units'>%</sup>";
html += "</p>";
html += "<hr>";

//For water level
html += "<p class='sensor'>";
html += "<i class='fas fa-heartbeat' style='color:#cc3300'></i>";
html += "<span class='sensor-labels'> Water Level </span>"; 
html += distance;
html += "<sup class='units'>cm</sup>";
html += water_level;
html += "</p>";
html += "<hr>";

//For Sp02
//html += "<p class='sensor'>";
//html += "<i class='fas fa-burn' style='color:#f7347a'></i>";
//html += "<span class='sensor-labels'> Sp02 </span>";
//html += (int)SpO2;
//html += "<sup class='units'>%</sup>";
//html += "</p>";
//html += "<hr>";


html += "</div>";
html += "</div>";
html += "</div>";
html += "</div>";
html += "</div>";
html += "</body>";
html += "</html>";
return html;
}
