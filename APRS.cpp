#include "APRS.h"

APRSClass::APRSClass(char * destination, char * source, int destinationSsid, int sourceSsid) :
  _destination(destination),
  _source(source),
  _destinationSsid(destinationSsid),
  _sourceSsid(sourceSsid) {
}

APRSClass::~APRSClass() {
}

char * APRSClass::createHeader(char * destination, char * source, int destinationSsid, int sourceSsid) {
  char * destinationAPRS = aton(destination, destinationSsid);
  char * sourceAPRS = aton(source, sourceSsid);
  char digipeater[8] = { 0xAE, 0x92, 0x88, 0x8A, 0x62, 0x40, 0x63, 0x00 }; // WIDE1-1

  char * header = (char*)malloc(sizeof(char)*(strlen(destinationAPRS) + strlen(sourceAPRS) +
                          strlen(digipeater) + 2 + 1));
  sprintf(header, "%s%s%s%c%c", destinationAPRS, sourceAPRS, digipeater, UI_FRAME, APRS_PID);
  free(destinationAPRS);
  free(sourceAPRS);

  return header;
}

char * APRSClass::createWeather(const int month, const int day, const int hour, const int minute,
  const float windDirection,
  const float windSpeed, const float temperature, const float gust,
  const float rain, const float humidity, const float barometricPressure) {
    char * header = createHeader(_destination, _source, _destinationSsid, _sourceSsid);

    char * timeStamp = (char*)malloc(sizeof(char)*9);
    sprintf(timeStamp, "%02d%02d%02d%02d", month, day, hour, minute);

    char * weatherString = (char*)malloc(sizeof(char)*39);
    sprintf(weatherString, "_%sc%03ds%03dg%03dt%03dr%03dh%02db%05d", timeStamp,
      (int)windDirection, (int)windSpeed, (int)gust, (int)temperature,
      (int)rain, (int)humidity, (int)barometricPressure);

    char * weather = (char*)malloc(sizeof(char)*(strlen(header) + strlen(weatherString) + 1));
    sprintf(weather, "%s%s", header, weatherString);
    free(header);
    free(timeStamp);
    free(weatherString);

    return weather;
  }

char * APRSClass::createPosition(const float latitude, const float longitude, char * comments) {
  char * header = createHeader(_destination, _source, _destinationSsid, _sourceSsid);

  char * latitudeAPRS = latitudeToAPRS(latitude);
  char * longitudeAPRS = longitudeToAPRS(longitude);

  char * positionString = (char*)malloc(sizeof(char)*21);
  sprintf(positionString, "%s!%s/%s$%s", header, latitudeAPRS, longitudeAPRS,
    comments);
  free(header);
  free(latitudeAPRS);
  free(longitudeAPRS);

  return positionString;
}

char *APRSClass::longitudeToAPRS(const float longitude) {
  char *longitudeAPRS = (char*)malloc(sizeof(char)*10);

  int degreeLongitude = abs((int) longitude);
  float minuteLongitude = (abs(longitude) - degreeLongitude)*60;
  char hemisphere = (longitude > 0) ? EAST : WEST;

  char * buf = (char*)malloc(sizeof(char)*6);
  dtostrf(minuteLongitude, 5, 2, buf);

  sprintf(longitudeAPRS, "%03d%s%c", degreeLongitude, buf, hemisphere);
  return longitudeAPRS;
}

char *APRSClass::latitudeToAPRS(const float latitude) {
  char *latitudeAPRS = (char*)malloc(sizeof(char)*9);

  int degreeLatitude = abs((int) latitude);
  float minuteLatitude = (abs(latitude) - degreeLatitude)*60;
  char hemisphere = (latitude > 0) ? NORTH : SOUTH;

  char * buf = (char*)malloc(sizeof(char)*6);
  dtostrf(minuteLatitude, 5, 2, buf);

  sprintf(latitudeAPRS, "%02d%s%c", degreeLatitude, buf, hemisphere);
  free(buf);
  return latitudeAPRS;
}

char * APRSClass::aton(char * callsign, int ssid) {
  int i;
  char * networkCallsign = (char*)malloc(sizeof(char)*8);

  for (i = 0; i < strlen(callsign); i++) {
    networkCallsign[i] = (toupper(callsign[i]) << 1);
  }
  networkCallsign[i] = (ssid << 1) | 0xE0;
  networkCallsign[i+1] = 0x00;
  return networkCallsign;
}

APRSClass APRS(TRACKR, DEFAULT_DESTINATION, DEFAULT_SSID, DEFAULT_SSID);
