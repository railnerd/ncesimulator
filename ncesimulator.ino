// ncesimulator
// an arduino sketch which acts like an nce powercab usb interface

const int ledPin = 13;

const byte versionReply[] = {
  0x06,0x03,0x00};

boolean programmingTrackOn = false;


void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect (for Leonardo only)
  }
}


void loop() {

  byte cmd;
  byte addrH, addrL, op1, data;
  byte cvH, cvL;

  if (Serial.available() > 0) {
    digitalWrite(13,HIGH);

    cmd = Serial.read();

    switch (cmd) {

    case 0x80:  // NOP; returns !
      Serial.write("!");
      break;

    case 0x8c:  // NOP + CR-LF reply
      Serial.println("!");
      break;

    case 0x9c:  // run Macro
      data = Serial.read();
      Serial.write("!");
      delay(500);
      break;

    case 0x9e:  // enter programming track
    case 0x9f:  // exit programming track
      programmingTrackOn = (cmd == 0x9e);
      delay(500);
      Serial.write("!");
      break;

    case 0xa0:  // Write Paged CV
    case 0xa8:  // Direct Write CV
      cvH = Serial.read();     
      cvL = Serial.read();
      data = Serial.read();

      delay(900);

      if (programmingTrackOn) {
        Serial.write("!");
      } 
      else {
        Serial.write("0");
      }
      break;

    case 0xa1:  // Read Paged CV
    case 0xa9:  // Direct Read CV
      cvH = Serial.read();     
      cvL = Serial.read();

      if (programmingTrackOn) {

        if (cmd == 0xa1) {
          delay(1800);      // Paged reads take a while
        } 
        else {
          delay(300);      //  Direct reads are faster
        }

        Serial.write(0x00);     // 0xff for cannot read cv
        Serial.write("!");      // 3 for cannot read cv
      } 
      else {
        Serial.write("0");
      }
      break;

    case 0xa2:
      // locomotive command <addrh>,<addrl>,<op1>,<data1>
      addrH = Serial.read();     
      addrL = Serial.read();
      op1 = Serial.read();
      data = Serial.read();

      Serial.write("!");
      break;

    case 0xa6:  // Write Register
      cvL  = Serial.read();
      data = Serial.read();

      delay(300);

      if (programmingTrackOn) {
        Serial.write("!");
      } 
      else {
        Serial.write("0");
      }
      break;

    case 0xa7:  // Read Register
      cvL = Serial.read();

      delay(500);

      if (programmingTrackOn) {
        Serial.write(0x00);     // 0xff for cannot read cv
        Serial.write("!");      // 3 for cannot read cv
      } 
      else {
        Serial.write("0");
      }
      break;

    case 0xaa:
      Serial.write(versionReply,sizeof(versionReply));
      break;

    case 0xad:
      // accessory/signal command
      addrH = Serial.read();     
      addrL = Serial.read();
      op1 = Serial.read();
      data = Serial.read();
      Serial.write("!");
      delay(200);
      break;

    case 0xae:  // ops program locomotive cv
    case 0xaf:  // ops program accessory cv
      addrH = Serial.read();     
      addrL = Serial.read();
      cvH = Serial.read();     
      cvL = Serial.read();
      data = Serial.read();
      Serial.write("!");
      break;

    default:
      break;
    }

  } 

  digitalWrite(13,LOW);
}

