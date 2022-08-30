#include <ESPMail.h>



void mail_send(String _info) {
  ESPMail mail;
  mail.begin();
  BootBlink(50);
  String htmlBody = "<h2>" + SSDP_Name + " - message</h2><h3>" + _info + "</h3>";
  mail.setSubject("maxim@umodom.ru", "Device message");
  mail.addTo("blkdem@blkdem.ru");
  mail.addCC("umodom@umodom.ru");
  //mail.addBCC("bcc@example.com");
  //mail.addAttachment("test.txt", "This is content of attachment.");
  //mail.setBody("This is an example e-mail.\nRegards Grzesiek");
  //mail.setHTMLBody("This is an example html <b>e-mail<b/>.\n<u>Regards Grzesiek</u>");
  htmlBody += "<h3>Config</h3><hr><p><strong>ssidAP</strong>: " + _ssidAP + "</p>";
  htmlBody += "<p><strong>passwordAP</strong>: " + _passwordAP + "</p>";
  htmlBody += "<p><strong>ssid</strong>: " + _ssid + "</p>";
  htmlBody += "<p><strong>password</strong>: " + _password + "</p>";
  htmlBody += "<p><strong>timezone</strong>: " + String(timezone)  + "</p>";
  htmlBody += "<p><strong>MQTT_IDX</strong>: " + String(MQTT_IDX) + "</p>";
  htmlBody += "<p><strong>IP</strong>: <a href='http://" + WiFi.localIP().toString() + "'>" + WiFi.localIP().toString() + "</a></p>";
  htmlBody += "<p><strong>Date/Time</strong>: " + GetDate()+ " / "+ GetTime() + "</p><hr>";
  //htmlBody += "<h3>Device Status</h3><p><strong>Relay0</strong>: " + String(CurrentRelayState) + "</p>";
  /*char* htmlBody_c = new char[htmlBody.length()+1];
  strcpy(htmlBody_c, htmlBody.c_str());
  htmlBody_c[htmlBody.length()+1]='\0';
  mail.setHTMLBody(htmlBody_c);*/
  mail.setHTMLBody((char*)String(htmlBody).c_str());
  //mail.enableDebugMode();
  //if (mail.send("smtp.server.com", 587, "your_smtp_user", "your_smtp_password") == 0)
  if (mail.send("fs6.umodom.ru", 25, "maxim", "ctdfcnjgjkm") == 0)
  {
    Serial.println("Mail send OK");
  }
}
