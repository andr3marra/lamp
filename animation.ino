void callAnimation(char animationChar) {
  if ( lastAnimationChar != animationChar) {
    FastLED.setBrightness( brightness );        // Inserir no single color?
    FastLED.show();
  }
  switch (animationChar) {
    case 's':             // Solid
      break;
    case 'b':             // Breathing
      breathing();
      break;
    case 'w':             // Wave
      break;
    case 'o':             // Off
      break;
  }
  lastAnimationChar = animationChar;
}

void breathing() {
  t = millis();
  if ( (t - tLast) > delayBreathing / 255) {
    breathCount++;
    Serial.print(breathCount); Serial.print(" "); Serial.println(sin8(breathCount));
    FastLED.setBrightness( sin8(breathCount) * brightness / 255 );
    FastLED.show();
    tLast = millis();
  }
}
//void wave() {
//  for ( int i = 0; i < NUM_LEDS; i++) {
//    leds[(i)].setRGB( map(i, 0, NUM_LEDS - 1, rgbDouble[0], rgbDouble[3]), map(i, 0, NUM_LEDS - 1, rgbDouble[1], rgbDouble[4]), map(i, 0, NUM_LEDS - 1, rgbDouble[2], rgbDouble[5]));
//  }
//  FastLED.show();
//}
