
// modified from
// http://playground.arduino.cc/Code/EepromUtil


void clear_EEPROM() {
  EEPROM.begin(512);
  // write a 0 to all 512 bytes of the EEPROM
  for (int i = 0; i < 512; i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
  EEPROM.end();
}



boolean eeprom_is_addr_ok(int addr) {
  return ((addr >= EEPROM_MIN_ADDR) && (addr <= EEPROM_MAX_ADDR));
}


boolean eeprom_write_bytes(int startAddr, const byte* array, int numBytes) {

  // both first byte and last byte addresses must fall within
  // the allowed range
  if (!eeprom_is_addr_ok(startAddr) || !eeprom_is_addr_ok(startAddr + numBytes)) {
    return false;
  }
  //Serial.println("w start: " + String(startAddr) + " total: " + String(numBytes) + " string: " + String(array));
  EEPROM.begin(512);
  for (int i = 0; i < numBytes; i++) {
    EEPROM.write(startAddr + i, array[i]);
  }
  EEPROM.commit();
  EEPROM.end();
  return true;
}

boolean eeprom_read_bytes(int startAddr, byte array[], int numBytes) {

  // both first byte and last byte addresses must fall within
  // the allowed range
  if (!eeprom_is_addr_ok(startAddr) || !eeprom_is_addr_ok(startAddr + numBytes)) {
    return false;
  }
  EEPROM.begin(512);
  for (int i = 0; i < numBytes; i++) {
    array[i] = EEPROM.read(startAddr + i);
  }
  EEPROM.end();
  //Serial.println("r start: " + String(startAddr) + " total: " + String(numBytes) + " string: " + String(array));
  return true;
}

//void eeprom_serial_dump_column() {
//  EEPROM.begin(512);
//  counter
//  int i;

//   byte read from eeprom
//  byte b;

//   buffer used by sprintf
//  char buf[10];
//
//  for (i = EEPROM_MIN_ADDR; i <= EEPROM_MAX_ADDR; i++) {
//    b = EEPROM.read(i);
//    sprintf(buf, "%03X: %02X", i, b);
//    Serial.println(buf);
//  }
//  EEPROM.end();
//}

//
// Writes a string starting at the specified address.
// Returns true if the whole string is successfully written.
// Returns false if the address of one or more bytes
// fall outside the allowed range.
// If false is returned, nothing gets written to the eeprom.
//
boolean eeprom_write_string(int addr, const char* string) {
  // actual number of bytes to be written
  int numBytes;

  // we'll need to write the string contents
  // plus the string terminator byte (0x00)
  numBytes = strlen(string) + 1;
  return eeprom_write_bytes(addr, (const byte*)string, numBytes);
}

//
// Reads a string starting from the specified address.
// Returns true if at least one byte (even only the
// string terminator one) is read.
// Returns false if the start address falls outside
// or declare buffer size os zero.
// the allowed range.
// The reading might stop for several reasons:
// - no more space in the provided buffer
// - last eeprom address reached
// - string terminator byte (0x00) encountered.
// The last condition is what should normally occur.
//
boolean eeprom_read_string(int addr, char* buffer, int bufSize) {
  EEPROM.begin(512);
  // byte read from eeprom
  byte ch;

  // number of bytes read so far
  int bytesRead;

  // check start address
  if (!eeprom_is_addr_ok(addr)) {
    return false;
  }

  // how can we store bytes in an empty buffer ?
  if (bufSize == 0) {
    return false;
  }

  // is there is room for the string terminator only,
  // no reason to go further
  if (bufSize == 1) {
    buffer[0] = 0;
    return true;
  }

  // initialize byte counter
  bytesRead = 0;

  // read next byte from eeprom
  ch = EEPROM.read(addr + bytesRead);

  // store it into the user buffer
  buffer[bytesRead] = ch;

  // increment byte counter
  bytesRead++;

  // stop conditions:
  // - the character just read is the string terminator one (0x00)
  // - we have filled the user buffer
  // - we have reached the last eeprom address
  while ( (ch != 0x00) && (bytesRead < bufSize) && ((addr + bytesRead) <= EEPROM_MAX_ADDR) ) {
    // if no stop condition is met, read the next byte from eeprom
    ch = EEPROM.read(addr + bytesRead);

    // store it into the user buffer
    buffer[bytesRead] = ch;

    // increment byte counter
    bytesRead++;
  }

  // make sure the user buffer has a string terminator
  // (0x00) as its last byte
  if ((ch != 0x00) && (bytesRead >= 1)) {
    buffer[bytesRead - 1] = 0;
  }
  EEPROM.end();
  return true;
}


