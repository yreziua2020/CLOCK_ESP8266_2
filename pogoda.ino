
// =======================================================================
// Берем погоду с сайта openweathermap.org
// =======================================================================



const char *weatherHost = "api.openweathermap.org";

void getWeatherData()
{
 
  if (tmp_cl_pog==3) {
 
  Serial.print("connecting to "); Serial.println(weatherHost);
  if (client.connect(weatherHost, 80)) {
    client.println(String("GET /data/2.5/weather?id=") + cityID + "&units=metric&appid=" + weatherKey + "&lang=ru" + "\r\n" +
                "Host: " + weatherHost + "\r\nUser-Agent: ArduinoWiFi/1.1\r\n" +
                "Connection: close\r\n\r\n");
  } else {
    Serial.println("connection failed");
    return;
  }
  String line;
  int repeatCounter = 0;
  while (!client.available() && repeatCounter < 10) {
    delay(500);
    Serial.println("w.");
    repeatCounter++;
  }
  while (client.connected() && client.available()) {
    char c = client.read(); 
    if (c == '[' || c == ']') c = ' ';
    line += c;
  }

  client.stop();
  Serial.println(line + "\n");
  DynamicJsonBuffer jsonBuf;
  JsonObject &root = jsonBuf.parseObject(line);
  if (!root.success())
  {
    Serial.println("parseObject() failed");
    return;
  }
  //weatherMain = root["weather"]["main"].as<String>();
  weatherDescription = root["weather"]["description"].as<String>();
  weatherDescription.toLowerCase();
  //  weatherLocation = root["name"].as<String>();
  //  country = root["sys"]["country"].as<String>();
  temp = root["main"]["temp"];
  humidity = root["main"]["humidity"];
  pressure = root["main"]["pressure"];
  windSpeed = root["wind"]["speed"];
  windDeg = root["wind"]["deg"];
  clouds = root["clouds"]["all"];
  String deg = String(char('~'+25));
  weatherString = "Сейчас " + String(temp,0)+" ";
  weatherString += weatherDescription;
  weatherString += " Влажн " + String(humidity) + "% ";
  weatherString += "Давл " + String(pressure/1.3332239,0) + " мм ";
//  weatherString += "Облачность: " + String(clouds) + "% ";

String windDegString;

if (windDeg>=345 || windDeg<=22) windDegString = "Северный";
if (windDeg>=23 && windDeg<=68) windDegString = "Северо-восточный";
if (windDeg>=69 && windDeg<=114) windDegString = "Восточный";
if (windDeg>=115 && windDeg<=160) windDegString = "Юго-восточный";
if (windDeg>=161 && windDeg<=206) windDegString = "Южный";
if (windDeg>=207 && windDeg<=252) windDegString = "Юго-западный";
if (windDeg>=253 && windDeg<=298) windDegString = "Западный";
if (windDeg>=299 && windDeg<=344) windDegString = "Северо-западный";


  weatherString += "Ветер " + windDegString + " " + String(windSpeed,1) + " м/с";



  
  Serial.println("POGODA: " + String(temp,0) + "\n");
  }
}

// =======================================================================
// Берем ПРОГНОЗ!!! погоды с сайта openweathermap.org
// =======================================================================



const char *weatherHostz = "api.openweathermap.org";

void getWeatherDataz()
 {  

  if (tmp_cl_pog==3) {
  Serial.print("connecting to "); Serial.println(weatherHostz);
  if (client.connect(weatherHostz, 80)) {
    client.println(String("GET /data/2.5/forecast/daily?id=") + cityID + "&units=metric&appid=" + weatherKey + "&lang=ru" + "&cnt=2" + "\r\n" +
                "Host: " + weatherHostz + "\r\nUser-Agent: ArduinoWiFi/1.1\r\n" +
                "Connection: close\r\n\r\n");
  } else {
    Serial.println("connection failed");
    return;
  }
  String line;
  int repeatCounter = 0;
  while (!client.available() && repeatCounter < 10) {
    delay(500);
    Serial.println("w.");
    repeatCounter++;
  }
  while (client.connected() && client.available()) {
    char c = client.read(); 
    if (c == '[' || c == ']') c = ' ';
    line += c;
  }
  tvoday(line);
  Serial.println(tempz + "\n");

  client.stop();
  
  DynamicJsonBuffer jsonBuf;
  JsonObject &root = jsonBuf.parseObject(tempz);
  if (!root.success())
  {
    Serial.println("parseObject() failed");
    return;
  }
  lon = root ["coord"]["lon"];
  lat = root ["coord"]["lat"];
  
  float wSpeed = root ["speed"];
  int wDeg = root ["deg"];
  float tempMin = root ["temp"]["min"];
  float tempMax = root ["temp"]["max"];
  weatherDescription = root ["weather"]["description"].as<String>();
  
  weatherStringz = "Завтра " + String(tempMin,1) + " .. " + String(tempMax,1) + " " + weatherDescription;
  Serial.println("!!!!!PROGNOZ: " + weatherStringz + " Wind: "+wSpeed+ " WindDeg: "+(wDeg)+ "\n");
  
  String windDegString;

if (wDeg>=345 || wDeg<=22) windDegString = "Северный";
if (wDeg>=23 && wDeg<=68) windDegString = "Северо-восточный";
if (wDeg>=69 && wDeg<=114) windDegString = "Восточный";
if (wDeg>=115 && wDeg<=160) windDegString = "Юго-восточный";
if (wDeg>=161 && wDeg<=206) windDegString = "Южный";
if (wDeg>=207 && wDeg<=252) windDegString = "Юго-западный";
if (wDeg>=253 && wDeg<=298) windDegString = "Западный";
if (wDeg>=299 && wDeg<=344) windDegString = "Северо-западный";

  weatherStringz1 = "Ветер " + windDegString + " " + String(wSpeed,1) + " м/с";

 }
}


// =======================================================================
  void tvoday(String line){
    String s;
    int strt = line.indexOf('}');
    for (int i=1; i<=4; i++){
      strt = line.indexOf('}', strt + 1);
    }
    s = line.substring(2+strt, line.length());
    tempz=s;
}

// =======================================================================
