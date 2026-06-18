#include <Arduino.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Wifi.h>
#include <SpotifyEsp32.h>
#include <SPI.h>
#include "SpotifyLogo.h"

#define TFT_CS 1
#define TFT_RST 2
#define TFT_DC 3
#define TFT_SCLK 4
#define TFT_MOSI 5

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

const char* SSID = "Insert_SSID_HERE";
const char* PASSWORD = "INSERT_PASS_HERE"
const char* CLIENT_ID = "INSERT_CLIENT_ID";
const char* CLIENT_SECRET = "INSERT_CLIENT_SECRET"

void setup() {
  Serial.begin(115200)

  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  Serial.println("TFT Display Initialized!");
  tft.fillScreen(ST77XX_BLACK);

  Wifi.begin(SSID, PASSWORD);
  Serial.print("Attempting to connect to wifi...");
  while(Wifi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected To Wifi!\n");

  tft.setCursor(0.0); // cursor to the top left
  tft.write(Wifi.localIP().toString().c_str()); // print IP to screen

  Spotify spotifyPlayer(CLIENT_ID, CLIENT_SECRET);

  spotifyPlayer.begin();
  while(!spotifyPlayer.is_auth()){
    delay(1000);
    spotifyPlayer.handle_Client();
  }
  Serial.println("Authenticated Spotify Key!");

}

void loop() {
  tft.displayRGBBitmap(0,0, Spotify_logo_without_text_svg_2_[], 128, 128)
  String currentArtist = spotifyPlayer.current_artist_names();
  String currentTrack = spotifyPlayer.current_track_name();
  String spotifyPlayer.get_current_album_image_url();

  }
}

}

void convertAlbumToCArray(String url){

}
