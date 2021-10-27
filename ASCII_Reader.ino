#include <SoftwareSerial.h>

SoftwareSerial rfid = SoftwareSerial(6, 5); // (RX = DATA0, TX = RESET)

void setup() {
  Serial.begin(9600);
  rfid.begin(9600);
  Serial.println("Ready");
}

void loop() {
  String message = "";

  while (rfid.available() > 0) {
    message += char(rfid.read()); // RFID serial read returns a hexadecimal character, and loop adds it to message
    delay(1);
  }
  
  if (message.length() >= 13) {
    message = message.substring(1,13); // Isolates data and checksum from message (see documentation for details)
    Serial.println("");
    Serial.println(message);
    verifyMessage(message);
  }
  delay(1);
}

// Function to verify message data transfer by calculating checksum (see documentation for details)
void verifyMessage (String inputMsg) {
  int check = 0;
  for (int i = 0; i < 10; i = i + 2) {
    int part1 = asciiToDecimal(inputMsg.charAt(i)) << 4;
    int part2 = asciiToDecimal(inputMsg.charAt(i+1));
    check ^= part1 + part2;
  }
  int realCheck = (asciiToDecimal(inputMsg.charAt(10)) << 4) + asciiToDecimal(inputMsg.charAt(11));;
  check == realCheck ? Serial.println("Valid checksum") : Serial.println("Invalid checksum");
}

// Function to convert ASCII to decimal values
// Example: letter A = 65 in ASCII, but 10 in decimal
int asciiToDecimal (int hex) {
  if (hex >= '0' && hex <= '9') {
    hex = hex - '0';
  } else if (hex >= 'A' && hex <= 'F') {
    hex = 10 + hex - 'A';
  }
  return hex;
}
