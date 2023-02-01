#include <WiFi.h>
#include <Wire.h>
#include <WebServer.h>
#include <OneWire.h>

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





//webserver units
#define REPORTING_PERIOD_MS    100
uint32_t lastReportTime = 0;
uint32_t tsLastReport = 0;
const char* ssid = "Oghenemaega’s iPhone";  // Enter SSID here
const char* password = "maega242";  //Enter Password here
WebServer server(80);  


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

  Serial.println("Connecting to ");
  Serial.println(ssid);

   
 
  //connect to your local wi-fi network
  WiFi.begin(ssid, password);
 
  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
  lcd.setCursor(1, 0);
  lcd.print("connecting...");
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  lcd.setCursor(1, 0);
  lcd.print("server connected");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());
 
  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);
 
  server.begin();
  Serial.println("HTTP server started");
 




  
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
    server.handleClient();
     
}

void alarm(){
   digitalWrite(buzzer, HIGH);
   delay(1000);
   digitalWrite(buzzer, LOW);
   delay(1000);
}

void handle_OnConnect() {
  
//  turbidity start
  int sensorValue = analogRead(sensorPin);
  float value = sensorValue*(5.0/1024.0);
  float turbidity = value;
  String turber = String(turbidity);
  Serial.println(turbidity);
  Serial.print("\t");
  Serial.print(turb_state  );
  Serial.print("\t");
  
  if (turbidity <= 9.00 ) {
      turb_state = "Clear water ";

  }
  else if (turbidity >= 2.00 ) {
    alarm();
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
  

 String dister = String(distance);



  
  
  server.send(200, "text/html", SendHTML( distance)); 
}
 
void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}



String SendHTML(float distance){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<link href=\"https://fonts.googleapis.com/css?family=Open+Sans:300,400,600\" rel=\"stylesheet\">\n";
  ptr +="<title>ESP32 Weather Report</title>\n";
  ptr +="<style>html { font-family: 'Open Sans', sans-serif; display: block; margin: 0px auto; text-align: center;color: #333333;}\n";
  ptr +="body{margin-top: 50px;}\n";
  ptr +="h1 {margin: 50px auto 30px;}\n";
  ptr +=".side-by-side{display: inline-block;vertical-align: middle;position: relative;}\n";
  ptr +=".humidity-icon{background-color: #3498db;width: 30px;height: 30px;border-radius: 50%;line-height: 36px;}\n";
  ptr +=".humidity-text{font-weight: 600;padding-left: 15px;font-size: 19px;width: 160px;text-align: left;}\n";
  ptr +=".humidity{font-weight: 300;font-size: 60px;color: #3498db;}\n";
  ptr +=".temperature-icon{background-color: #f39c12;width: 30px;height: 30px;border-radius: 50%;line-height: 40px;}\n";
  ptr +=".temperature-text{font-weight: 600;padding-left: 15px;font-size: 19px;width: 160px;text-align: left;}\n";
  ptr +=".temperature{font-weight: 300;font-size: 60px;color: #f39c12;}\n";
  ptr +=".superscript{font-size: 17px;font-weight: 600;position: absolute;right: -20px;top: 15px;}\n";
  ptr +=".data{padding: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  
   ptr +="<div id=\"webpage\">\n";
   
   ptr +="<h1>ESP32 Weather Report</h1>\n";
   ptr +="<div class=\"data\">\n";
   ptr +="<div class=\"side-by-side temperature-icon\">\n";
   ptr +="<svg version=\"1.1\" id=\"Layer_1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" x=\"0px\" y=\"0px\"\n";
   ptr +="width=\"9.915px\" height=\"22px\" viewBox=\"0 0 9.915 22\" enable-background=\"new 0 0 9.915 22\" xml:space=\"preserve\">\n";
   ptr +="<path fill=\"#FFFFFF\" d=\"M3.498,0.53c0.377-0.331,0.877-0.501,1.374-0.527C5.697-0.04,6.522,0.421,6.924,1.142\n";
   ptr +="c0.237,0.399,0.315,0.871,0.311,1.33C7.229,5.856,7.245,9.24,7.227,12.625c1.019,0.539,1.855,1.424,2.301,2.491\n";
   ptr +="c0.491,1.163,0.518,2.514,0.062,3.693c-0.414,1.102-1.24,2.038-2.276,2.594c-1.056,0.583-2.331,0.743-3.501,0.463\n";
   ptr +="c-1.417-0.323-2.659-1.314-3.3-2.617C0.014,18.26-0.115,17.104,0.1,16.022c0.296-1.443,1.274-2.717,2.58-3.394\n";
   ptr +="c0.013-3.44,0-6.881,0.007-10.322C2.674,1.634,2.974,0.955,3.498,0.53z\"/>\n";
   ptr +="</svg>\n";
   ptr +="</div>\n";
   ptr +="<div class=\"side-by-side temperature-text\">Temperature</div>\n";
   ptr +="<div class=\"side-by-side temperature\">";
   ptr +=(int)ph_act;
   ptr +="<span class=\"superscript\">°C</span></div>\n";
   ptr +="</div>\n";
 

  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}

//
//String SendHTML(
//  float ph_act,
//  String ph_state,
//  String turber,
//  String dister,
//  String turb_state,
//  String water_level
//  ) 
//  
//  
//  {
//String html = "<!DOCTYPE html>";
//html += "<html>";
//html += "<head>";
//html += "<title>Water Purity tester</title>";
//html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
//html += "<link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.7.2/css/all.min.css'>";
//html += "<link rel='stylesheet' type='text/css' href='styles.css'>";
//html += "<style>";
//html += "body { background-color: #fff; font-family: sans-serif; color: #333333; font: 14px Helvetica, sans-serif box-sizing: border-box;}";
//html += "#page { margin: 20px; background-color: #fff;}";
//html += ".container { height: inherit; padding-bottom: 20px;}";
//html += ".header { padding: 20px;}";
//html += ".header h1 { padding-bottom: 0.3em; color: #008080; font-size: 45px; font-weight: bold; font-family: Garmond, 'sans-serif'; text-align: center;}";
//html += "h2 { padding-bottom: 0.2em; border-bottom: 1px solid #eee; margin: 2px; text-align: left;}";
//html += ".header h3 { font-weight: bold; font-family: Arial, 'sans-serif'; font-size: 17px; color: #b6b6b6; text-align: center;}";
//html += ".box-full { padding: 20px; border 1px solid #ddd; border-radius: 1em 1em 1em 1em; box-shadow: 1px 7px 7px 1px rgba(0,0,0,0.4); background: #fff; margin: 20px; width: 300px;}";
//html += "@media (max-width: 494px) { #page { width: inherit; margin: 5px auto; } #content { padding: 1px;} .box-full { margin: 8px 8px 12px 8px; padding: 10px; width: inherit;; float: none; } }";
//html += "@media (min-width: 494px) and (max-width: 980px) { #page { width: 465px; margin 0 auto; } .box-full { width: 380px; } }";
//html += "@media (min-width: 980px) { #page { width: 930px; margin: auto; } }";
//html += ".sensor { margin: 12px 0px; font-size: 2.5rem;}";
//html += ".sensor-labels { font-size: 1rem; vertical-align: middle; padding-bottom: 15px;}";
//html += ".units { font-size: 1.2rem;}";
//html += "#level { font-size: 0.5rem; color:red;}";
//html += "hr { height: 1px; color: #eee; background-color: #eee; border: none;}";
//html += "</style>";
//
////Ajax Code Start
//  html += "<script>\n";
//  html += "setInterval(loadDoc,1000);\n";
//  html += "function loadDoc() {\n";
//  html += "var xhttp = new XMLHttpRequest();\n";
//  html += "xhttp.onreadystatechange = function() {\n";
//  html += "if (this.readyState == 4 && this.status == 200) {\n";
//  html += "document.body.innerHTML =this.responseText}\n";
//  html += "};\n";
//  html += "xhttp.open(\"GET\", \"/\", true);\n";
//  html += "xhttp.send();\n";
//  html += "}\n";
//  html += "</script>\n";
//  //Ajax Code END
//
//
//  html += "</head>";
//  html += "<body>";
//  html += "<div id='page'>";
//  html += "<div class='header'>";
//  html += "<h2>Water Purity tester by Mjoghenemaega </h2>";
//  html += "</div>";
//  html += "<div id='content' align='center'>";
//  html += "<div class='box-full' align='left'>";
//  html += "<h2>Water Quality heck Values</h2>";
//
//  html += "<div class='sensors-container'>";
//
//  //For ph level
//  html += "<div class='sensors'>";
//  html += "<p class='sensor'>";
//  html += "<i class='fas fa-solid fa-skull' style='color:#fff'></i>";
//  html += "<span class='sensor-labels'>pH level </span>"; 
//  html += (int)ph_act;
//  html += "<div id='level'>";
//  html += ph_state;
//  html += "</div>";
//  html += "</p>";
//  html += "<hr>";
//  html += "</div>";
//
//
//  return html;
//}
//  
//
