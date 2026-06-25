#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <TJpg_Decoder>
#include <TFT_eSPI>
#include <SpotifyEsp32.h>
#include <SPI.h>
#include "SpotifyLogo.h"
#include "songButtons.h"

TFT_eSPI tft = TFT_eSPI();

String lastSongID = "";
bool songPlaying = false;

const char* SSID = "Insert_SSID_HERE"; // Insert SSID
const char* PASSWORD = "INSERT_PASS_HERE"; // Insert Password
const char* CLIENT_ID = "INSERT_CLIENT_ID"; // Insert Client ID from Spotify API
const char* CLIENT_SECRET = "INSERT_CLIENT_SECRET"; // Insert Client Secret ID in Spotify API

Spotify spotifyPlayer(CLIENT_ID, CLIENT_SECRET);

int xPos = 0; //place holders. Want to actually startd drawing when I assemble the build
int yPos = 0;

void setup() {
  Serial.begin(115200);
  tft.begin();
  tft.setTextColor(0xFFFF, 0x0000);
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(1);
  Serial.println("TFT Display Initialized!");

  TJpgDec.setJpgScale(1);

  TJpgDec.setSwapBytes(true);

  TJpgDec.setCallback(tftOutput);

  WiFi.begin(SSID, PASSWORD);
  Serial.print("Attempting to connect to wifi...");
  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected To Wifi!\n");

  spotifyPlayer.begin();
  while(!spotifyPlayer.is_auth()){
    delay(1000);
    spotifyPlayer.handle_client();
  }
  Serial.println("Authenticated Spotify Key!");

  tft.pushImage(0,0, Spotify_logo_without_text_svg_2_[], 50, 50);
  tft.pushImage(20, 100, 10, 10, previousIcon[]);
  tft.pushImage(140, 100, 10, 10, nextIcon[]);

}

void loop() {
  songPlaying = spotifyPlayer.is_playing();
  if(songPlaying){
    tft.pushImage(80, 100, 10, 10, pauseIcon[]);
  }
  else{
    tft.pushImage(80, 100, 10, 10, playIcon[]);
  }
  String currentSongID = spotifyPlayer.current_track_id();
  if (currentSongID.length() > 0 && currentSongID != lastSongID){
    lastSongID = currentSongID;
    String currentArtist = spotifyPlayer.current_artist_names();
    String currentTrack = spotifyPlayer.current_track_name();
    tft.drawString(currentTrack, xPos, yPos);
    tft.drawString(currentArtist, xPos, yPos); // will draw properly when it is fully assembled

    drawAlbumCover();
  }

}

bool tftOutput(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap){
  if (y >= tft.height()) return 0;

  tft.pushImage(x,y,w,h,bitmap);
  return 1;
}

void drawAlbumCover(){
  String currentAlbumCover = spotifyPlayer.get_current_album_image_url(2);
  TJpgDec.drawJpg(x,y, currentAlbumCover); // x and y are placeholders for when I actually decide where to place album properly
}
