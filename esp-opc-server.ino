/* This Arduino sketch implements an Open Pixel Control server
 * for controlling NeoPixels from an ESP8266, such as the Adafruit
 * Feather HUZZAH.
 */

#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>

/* Edit this to configure the number of pixels you have, and your SSID and password */
#define PIN 2       // pin that NeoPixels are connected to
#define N_PIXELS 8  // number of NeoPixels on your strip
#define WIFI_SSID   "Your SSID"
#define WIFI_PASSWD "Your password"

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_PIXELS, PIN, NEO_GRB + NEO_KHZ800);
WiFiServer server(7890);

void setup() {
  uint16_t i = 0;

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  // display a moving red pixel while connecting to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWD);
  while (WiFi.status() != WL_CONNECTED) {
    strip.setPixelColor(i, strip.Color(32, 0, 0));
    strip.show();
    delay(200);
    strip.setPixelColor(i, strip.Color(0, 0, 0));
    i++;
    if (i >= strip.numPixels())
      i = 0;
  }

  // all pixels green once we've connected to WiFi
  for (i = 0; i < strip.numPixels(); i++)
    strip.setPixelColor(i, strip.Color(0, 32, 0));
  strip.show();

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    while (client.connected()) {
      readFrame(client);
      yield();
    }
    client.stop();
  }
}

int readFully (WiFiClient &client, uint8_t *buf, size_t len) {
  size_t i;

  for (i = 0; i < len; i++) {
    int b;

    if ((b = blockingRead(client)) < 0)
      return -1;

    buf[i] = (uint8_t) b;
  }

  return 0;
}

int blockingRead (WiFiClient &client) {
  int b;
  while ((b = client.read()) < 0) {
    yield();
    if (! client.connected())
      return -1;
  }
  return b;
}

void readFrame(WiFiClient &client) {
  uint8_t buf4[4];
  uint8_t cmd;
  size_t payload_length, leds_in_payload, i;

  // read channel number (ignored), command, and length
  if (readFully (client, buf4, sizeof (buf4)) < 0)
    return;

  cmd = buf4[1];
  payload_length = (((size_t) buf4[2]) << 8) + (size_t) buf4[3];
  leds_in_payload = payload_length / 3;
  if (leds_in_payload > strip.numPixels())
    leds_in_payload = strip.numPixels();
  if (cmd != 0)                 // we only support command 0, set pixel colors
    leds_in_payload = 0;

  // read pixel data; 3 bytes per pixel
  for (i = 0; i < leds_in_payload; i++) {
    if (readFully (client, buf4, 3) < 0)
      return;
    strip.setPixelColor(i, strip.Color(buf4[0], buf4[1], buf4[2]));
  }
  strip.show();

  // discard any remaining data (e. g. if they sent us more pixels than we have)
  payload_length -= leds_in_payload * 3;

  for (; payload_length != 0; payload_length--) {
    if (blockingRead(client) < 0)
      return;
  }
}
