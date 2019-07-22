#ifndef APRS_H
#define APRS_H

#include <Arduino.h>
#include <String.h>

#define UI_FRAME              0x03
#define APRS_PID              0xF0

#define DEFAULT_SSID          0

// POSITION
#define LATITUDE_DEGREE_SIZE  2
#define LONGITUDE_DEGREE_SIZE 3
#define MINUTE_STRING_SIZE    5
#define MINUTE_AFTER_DECIMAL  2
#define HEMISPHERE_SIZE       1
#define NORTH                 0x4E
#define SOUTH                 0x53
#define WEST                  0x57
#define EAST                  0x45

// TELEMETRY
#define TELEMETRY_STRING_SIZE 34

// WEATHER
#define RAW_TIMESTAMP_SIZE    8
#define RAW_WEATHER_SIZE      38

static char TRACKR[7]              = {0x54, 0x52, 0x41, 0x43, 0x4B, 0x52, 0x00};
static char DEFAULT_DESTINATION[7] = {0x41, 0x50, 0x5A, 0x49, 0x4E, 0x41, 0x00};

class APRSClass {
  public:
    APRSClass(char * destination, char * source, int destinationSsid, int sourceSsid);
    virtual ~APRSClass();
    void setDestination(char * destination, int ssid);
    void setSource(char * source, int ssid);
    char * createPosition(const float latitude, const float longitude, char * comments);
    char * createWeather(const int month, const int day, const int hour, const int minute,
      const float windDirection, const float windSpeed, const float temperature,
      const float gust, const float rain, const float humidity,
      const float barometricPressure);
    char * createTelemetry(int counter, int analogValue1, int analogValue2,
        int analogValue3, int analogValue4, int analogValue5, boolean bit0,
        boolean bit1, boolean bit2, boolean bit3, boolean bit4, boolean bit5,
        boolean bit6, boolean bit7);
  private:
    char * aton(char * callsign, int ssid);
    char * longitudeToAPRS(const float longitude);
    char * latitudeToAPRS(const float latitude);
    char * createHeader(char * destination, char * source, int destinationSsid, int sourceSsid);
    char * _destination;
    char * _source;
    int _destinationSsid;
    int _sourceSsid;
};

extern APRSClass APRS;
#endif
