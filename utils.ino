
long Day=0;
int Hour =0;
int Minute=0;
int Second=0;
int HighMillis=0;
int Rollover=0; 

String uptime(){
//** Making Note of an expected rollover *****//   
  if(millis()>=3000000000){ 
    HighMillis=1;
  }
//** Making note of actual rollover **//
  if(millis()<=100000&&HighMillis==1){
    Rollover++;
    HighMillis=0;
  }
long secsUp = millis()/1000;
Second = secsUp%60;
Minute = (secsUp/60)%60;
Hour = (secsUp/(60*60))%24;
Day = (Rollover*50)+(secsUp/(60*60*24));  //First portion takes care of a rollover [around 50 days]
  return String(Day) + ":" + String(Hour) + ":" + String(Minute) + ":" + String(Second);
};

const char* IntToChar (int v) {
  char* buffer;
  sprintf(buffer, "%d", v);
  return buffer;
}

char* Str2Char(String _val)
{
  char* buffer = new char[_val.length()+1];
  strcpy(buffer, _val.c_str());
  //buffer[_val.length()+1]='\0';
  buffer[_val.length()]=0;
  return buffer;
}

/*char* Str2Char(String _val)
{
//std::string str = "string";
  char *cstr = new char[_val.length() + 1];
  strcpy(cstr, str.c_str());
// do stuff
  delete [] cstr;
}*/
