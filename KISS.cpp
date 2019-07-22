#include "KISS.h"

KISSClass::KISSClass(HardwareSerial& hwSerial) :
  _serial(&hwSerial) {
}

void KISSClass::begin(unsigned long baudRate) {
  begin(baudRate, SERIAL_8N1);
}

void KISSClass::begin(unsigned long baudRate, uint16_t config) {
  _baudRate = baudRate;
  _config = config;

  _serial->begin(_baudRate, _config);
}

void KISSClass::end() {
  _serial->end();
}

int KISSClass::available() {
  return _serial->available();
}

int KISSClass::peek() {
  return _serial->peek();
}

int KISSClass::read(void) {
  return _serial->read();
}

void KISSClass::flush() {
  return _serial->flush();
}

size_t KISSClass::write(uint8_t byte) {
  return _serial->write(byte);
}

void KISSClass::sendData(const char *buffer, int bufferSize) {
  write(FEND);
  _serial->write(CMD_DATA);

  for (int i = 0; i < bufferSize; i++) {
    escapedWrite(buffer[i]);
  }

  write(FEND);
}

void KISSClass::sendTxDelay(const uint8_t value) {
  write(FEND);
  _serial->write(CMD_TXDELAY);
  escapedWrite(value);
  write(FEND);
}

void KISSClass::sendPersist(const uint8_t value) {
  write(FEND);
  _serial->write(CMD_PERSIST);
  escapedWrite(value);
  write(FEND);
}

void KISSClass::sendTimeSlot(const uint8_t value) {
  write(FEND);
  _serial->write(CMD_SLOTTIME);
  escapedWrite(value);
  write(FEND);
}

void KISSClass::sendTxTail(const uint8_t value) {
  write(FEND);
  _serial->write(CMD_TXTAIL);
  escapedWrite(value);
  write(FEND);
}

void KISSClass::sendDuplex(const uint8_t value) {
  write(FEND);
  _serial->write(CMD_DUPLEX);
  escapedWrite(value);
  write(FEND);
}

void KISSClass::escapedWrite (uint8_t bufferByte) {
  switch(bufferByte) {
    case FEND:
      write(FESC);
      write(TFEND);
      break;
    case FESC:
      write(FESC);
      write(TFESC);
      break;
    default:
      write(bufferByte);
  }
}
