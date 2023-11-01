//==========================================================
void displayInfo(){
     // static uint32_t tmp2 ;
       static long tmp2 ;
   if (!P.displayAnimate()) tmp2++;
    if (P.displayAnimate()){
      
    utf8rus(Text).toCharArray(buf, 256);
    P.displayText(buf, PA_CENTER, catalog[rnd].speed, 5000, catalog[rnd].effect, catalog[rnd].effect); 
    
    if (!P.displayAnimate()) {disp = 2; Serial.print("disp =");Serial.print(disp); Serial.print("tmp2 =");Serial.println(tmp2);}

    }
}
//==========================================================
void displayInfo1(){
    if (P.displayAnimate()){
    utf8rus(Text).toCharArray(buf, 256);
    P.displayText(buf, PA_CENTER, catalog[rnd].speed, 5000, catalog[rnd].effect, catalog[rnd].effect);   
    if (!P.displayAnimate()) { disp = 4; Serial.print("disp =");Serial.println(disp);}
    }
}
//==========================================================
void displayInfo2(){
    if (P.displayAnimate()){
    utf8rus(Text).toCharArray(buf, 256);
    P.displayText(buf, PA_CENTER, catalog[rnd].speed, 5000, catalog[rnd].effect, catalog[rnd].effect);   
    if (!P.displayAnimate()) { disp = 6;Serial.print("disp =");Serial.println(disp);}
    }
}
//==========================================================
void displayInfo3(){
    if (P.displayAnimate()){
    utf8rus(Text).toCharArray(buf, 256);
    P.displayText(buf, PA_CENTER, catalog[rnd].speed, 5000, catalog[rnd].effect, catalog[rnd].effect);   
    if (!P.displayAnimate())  {disp = 0;Serial.print("disp =");Serial.println(disp);}
    }
}
//==========================================================
void scrollText(){
  static  long tmp3 ;
  if (!P.displayAnimate()) tmp3++;
  if  (P.displayAnimate()){
  utf8rus(Text).toCharArray(buf, 256);
  P.displayScroll(buf, PA_LEFT, PA_SCROLL_LEFT, 40);

  if (!P.displayAnimate()) {disp = 3;Serial.print("disp =");Serial.println(disp);Serial.print("tmp3 =");Serial.println(tmp3);tmp3=0;}
  }
}
//==========================================================
void scrollText1(){
       static  long tmp5 ;
  if  (P.displayAnimate()){
  utf8rus(Text).toCharArray(buf, 256);
  P.displayScroll(buf, PA_LEFT, PA_SCROLL_LEFT, 40);
   tmp5++;
  if (!P.displayAnimate()) {disp = 5;Serial.print("disp =");Serial.print(disp); Serial.print("tmp5 =");Serial.println(tmp5);tmp5=0;}
  }
}
//==========================================================
void scrollText2(){
  if  (P.displayAnimate()){
  utf8rus(Text).toCharArray(buf, 256);
  P.displayScroll(buf, PA_LEFT, PA_SCROLL_LEFT, 40);
  if (!P.displayAnimate()) {disp = 0;Serial.print("disp =");Serial.println(disp);}
  }
}
//==========================================================
void scrollText3(){
  if  (P.displayAnimate()){
  utf8rus(Text).toCharArray(buf, 256);
  P.displayScroll(buf, PA_LEFT, PA_SCROLL_LEFT, 40);
  if (!P.displayAnimate()) {disp = 0;Serial.print("disp =");Serial.println(disp);}
  }
}





//==========================================================
void scrollIP(){
  
    Text = "Ваш IP: "+ipstring;
  if  (P.displayAnimate()){
  utf8rus(Text).toCharArray(buf, 256);
  P.displayScroll(buf, PA_LEFT, PA_SCROLL_LEFT, 60);
  }

}
//==========================================================
void scrollConnect(){
  Text = "Отсутствует подключение к WIFI. Подключитесь к точке доступа 'UFA Iot' и войдите в веб интерфейс 192.168.4.1" ;
  if  (P.displayAnimate()){
  utf8rus(Text).toCharArray(buf, 256);
  P.displayScroll(buf, PA_LEFT, PA_SCROLL_LEFT, 40);
  if (!P.displayAnimate()) disp = 3;
  }
}







String utf8rus(String source)
{
  int i,k;
  String target;
  unsigned char n;
  char m[2] = { '0', '\0' };

  k = source.length(); i = 0;

  while (i < k) {
    n = source[i]; i++;

    if (n >= 0xC0) {
      switch (n) {
        case 0xD0: {
          n = source[i]; i++;
          if (n == 0x81) { n = 0xA8; break; }
          if (n >= 0x90 && n <= 0xBF) n = n + 0x30;
          break;
        }
        case 0xD1: {
          n = source[i]; i++;
          if (n == 0x91) { n = 0xB8; break; }
          if (n >= 0x80 && n <= 0x8F) n = n + 0x70;
          break;
        }
      }
    }
    m[0] = n; target = target + String(m);
  }
return target;
}

