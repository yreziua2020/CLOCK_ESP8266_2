void setup() {
  P.begin();
  P.setInvert(false);
  P.setFont(fontBG);
 P.setZoneEffect(0,true,PA_FLIP_UD);
 P.setZoneEffect(0,true,PA_FLIP_LR);
 P.setIntensity(0);
//P.setIntensity(0);

  bool CFG_saved = false;
  int WIFI_connected = false;
  Serial.begin(115200);


  pinMode(LED_PIN,OUTPUT);
  pinMode(buttonPin,INPUT);
  digitalWrite(buttonPin, HIGH);
  digitalWrite(LED_PIN, HIGH);
 
  //**** Network Config load 
  EEPROM.begin(512); // define an EEPROM space of 512Bytes to store data
  CFG_saved = ReadConfig();

  //  Connect to WiFi acess point or start as Acess point
  if (CFG_saved)  //if no configuration yet saved, load defaults
  {    
      // Connect the ESP8266 to local WIFI network in Station mode
      Serial.println("Booting");
      
      WiFi.mode(WIFI_STA);

  if (!config.dhcp)
  {
    WiFi.config(IPAddress(config.IP[0], config.IP[1], config.IP[2], config.IP[3] ),  IPAddress(config.Gateway[0], config.Gateway[1], config.Gateway[2], config.Gateway[3] ) , IPAddress(config.Netmask[0], config.Netmask[1], config.Netmask[2], config.Netmask[3] ) , IPAddress(config.DNS[0], config.DNS[1], config.DNS[2], config.DNS[3] ));
  }
      WiFi.begin(config.ssid.c_str(), config.password.c_str());
      printConfig();
      WIFI_connected = WiFi.waitForConnectResult();
      

    
  
      if(WIFI_connected!= WL_CONNECTED ){
        Serial.println("Connection Failed! activating to AP mode...");
        Serial.print("Wifi ip:");Serial.println(WiFi.localIP());
        Serial.print("Email:");Serial.println(config.email.c_str());
        
      }
  }

  if ( (WIFI_connected!= WL_CONNECTED) or !CFG_saved){
    // DEFAULT CONFIG
    scrollConnect();
    Serial.println("Setting AP mode default parameters");
    config.ssid = "UFA Iot";       // SSID of access point
    config.password = "" ;   // password of access point
    config.dhcp = true;
    config.IP[0] = 192; config.IP[1] = 168; config.IP[2] = 1; config.IP[3] = 100;
    config.Netmask[0] = 255; config.Netmask[1] = 255; config.Netmask[2] = 255; config.Netmask[3] = 0;
    config.Gateway[0] = 192; config.Gateway[1] = 168; config.Gateway[2] = 1; config.Gateway[3] = 1;
    config.DNS[0] = 192; config.DNS[1] = 168; config.DNS[2] = 1; config.DNS[3] = 1;
    config.ntpServerName = "0.ru.pool.ntp.org"; // to be adjusted to PT ntp.ist.utl.pt
    config.Update_Time_Via_NTP_Every =  10;
    config.timeZone = 3;
    config.isDayLightSaving = true;
    config.DeviceName = "API ключь";
    config.email = "cityID";
    WiFi.mode(WIFI_AP);  
    WiFi.softAP(config.ssid.c_str());
    Serial.print("Wifi ip:");Serial.println(WiFi.softAPIP());

   }
   

    // Start HTTP Server for configuration
    server.on ( "/", []() {
      Serial.println("admin.html");
      server.send_P ( 200, "text/html", PAGE_AdminMainPage);  // const char top of page
    }  );
  
    server.on ( "/favicon.ico",   []() {
      Serial.println("favicon.ico");
      server.send( 200, "text/html", "" );
    }  );
  
    // Network config
    server.on ( "/config.html", send_network_configuration_html );
    // Info Page
    server.on ( "/info.html", []() {
      Serial.println("info.html");
      server.send_P ( 200, "text/html", PAGE_Information );
    }  );
    server.on ( "/ntp.html", send_NTP_configuration_html  );
  
    //server.on ( "/appl.html", send_application_configuration_html  );
    server.on ( "/general.html", send_general_html  );
    //  server.on ( "/example.html", []() { server.send_P ( 200, "text/html", PAGE_EXAMPLE );  } );
    server.on ( "/style.css", []() {
      Serial.println("style.css");
      server.send_P ( 200, "text/plain", PAGE_Style_css );
    } );
    server.on ( "/microajax.js", []() {
      Serial.println("microajax.js");
      server.send_P ( 200, "text/plain", PAGE_microajax_js );
    } );
    server.on ( "/admin/values", send_network_configuration_values_html );
    server.on ( "/admin/connectionstate", send_connection_state_values_html );
    server.on ( "/admin/infovalues", send_information_values_html );
    server.on ( "/admin/ntpvalues", send_NTP_configuration_values_html );
    //server.on ( "/admin/applvalues", send_application_configuration_values_html );
    server.on ( "/admin/generalvalues", send_general_configuration_values_html);
    server.on ( "/admin/devicename",     send_devicename_value_html);
  
  
    server.onNotFound ( []() {
      Serial.println("Page Not Found");
      server.send ( 400, "text/html", "Page not Found" );
    }  );
    server.begin();
    Serial.println( "HTTP server started" );

    // ***********  OTA SETUP
  
    ArduinoOTA.setHostname(config.DeviceName.c_str());
    ArduinoOTA.onEnd([]() { // do a fancy thing with our board led at end
        ESP.restart();
      });
  
    ArduinoOTA.onError([](ota_error_t error) { 
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR) Serial.println("End Failed");
        ESP.restart(); 
      });
  
     /* setup the OTA server */
    ArduinoOTA.begin();
    Serial.println("Ready");
          for(int i=0; i<3; i++){ // Bling the LED to show the program started
           digitalWrite(LED_PIN, LOW);
           delay(200);
           digitalWrite(LED_PIN, HIGH);
           delay(200);
          }
    
      
    // start internal time update ISR
//    tkSecond.attach(1, ISRsecondTick);
    
    
    //**** Normal Sketch code here...
    




ipstring = (
String(WiFi.localIP()[0]) +"." +
String(WiFi.localIP()[1]) + "." +
String(WiFi.localIP()[2]) + "." +
String(WiFi.localIP()[3])
);


{


  for (uint8_t i=0; i<ARRAY_SIZE(catalog); i++)
  {   //P.getSpeed()=10 ВСЕГДА
    Serial.print("P.getSpeed()="); Serial.println(P.getSpeed());
    catalog[i].speed *= P.getSpeed();

    catalog[i].pause *= 500;
  }
}

t.every(1000, ISRsecondTick);

if  (WiFi.status() == WL_CONNECTED) {
getTime();
//scrollIP();

  //  t.every(10000, getTime);

  }  
   getTime();
   getWeatherData();
   getWeatherDataz();
weatherKey = config.DeviceName.c_str();
cityID = config.email.c_str();

  delay(1000);

Serial.println(tyuii);

}
