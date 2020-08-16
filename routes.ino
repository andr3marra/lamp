void setRoutes() {
  server.on("/", handleRoot);

  server.on("/brightness", []() {               // Set brightness
    brightness = server.arg(0).toInt();
    FastLED.setBrightness( brightness );        
    FastLED.show();
    server.send(200);
  });

  server.on("/color/single", []() {             // Single color
    for ( int i = 0; i < 3; i++) {
      rgbSingleColor[i] = server.arg(i).toInt();
    }
    modeChar = 's';
    singleColor(rgbSingleColor);
    server.send(200);
  });

  server.on("/color/double", []() {             // Double color
    for ( int i = 0; i < 6; i++) {
      rgbDoubleColor[i] = server.arg(i).toInt();
    }
    modeChar = 'd';
    doubleColor(rgbDoubleColor);
    server.send(200);
  });

  server.on("/color/rainbow", []() {            // Rainbow color
    modeChar = 'r';
    server.send(200);
  });

  server.on("/color/custom", HTTP_POST, []() {  // Custom color
    String data;
    data = server.arg(0);
    for ( uint8_t i = 0; i < NUM_LEDS; i++) {
      leds[(i)].setRGB( (byte) data.charAt(i), (byte) data.charAt(i + 1), (byte) data.charAt(i + 2));
    }
    FastLED.show();
    server.send(200);
  });

  server.on("/animation", []() {                // Animation Selection
    String animation = server.arg(0);
    animationChar = animation.charAt(0);
    server.send(200);
    if ( animationChar == 'b') {
      delayBreathing = server.arg(1).toInt();
      breathCount = 0;
    }
  });

  server.on("/status", []() {                 // turn on and off, and get current mode
    //server.send(200, "text/plain", animationChar);
    //server.send(200, "text/plain", (String) brightness);
  });

  server.on("/connect", []() {                 // conenct to wifi
    ssid = server.arg(0);
    server.send(200);
    ssid.replace("%20", " ");
    password = server.arg(1);
    server.send(200, "text/plain", WiFi.macAddress());
    Serial.println(ssid);
    Serial.println(password);
    wifiMode = 's';                                     // inserir validação para ssid e password
    server.stop();
    WiFiconfig(wifiMode);
  });
  server.onNotFound(handleNotFound);
}

void handleRoot() {
  server.send(200, "text/plain", "XX   XXXXX   XXXXXXXX  XX      XXXXXX  XXXXXX\nXX  XX   XX     XX     XX      XX      XX   XX\nXX  XX   XX     XX     XX      XXXXX   XX   XX\nXX  XX   XX     XX     XX      XX      XX   XX\nXX   XXXXX      XX     XXXXXX  XXXXXX  XXXXXX");
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
