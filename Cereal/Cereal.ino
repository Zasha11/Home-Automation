#include <Servo.h>

Servo myservo;
unsigned int pinStatus = 0;
const int ldrPin = A0;
const int TempPin = A5;


char operation; // Holds operation (R, W, ...)
char mode; // Holds the mode (D, A)
int pin_number; // Holds the pin number
int digital_value; // Holds the digital value
int analog_value; // Holds the analog value
int value_to_write; // Holds the value that we want to write
int wait_for_transmission = 5; // Delay in ms in order to receive the serial data

boolean lampOn = false;
boolean fanOn = false;
boolean autoLampOn = true;
boolean autoFanOn = true;


void set_pin_mode(int pin_number, char mode){
    /*
     * Performs a pinMode() operation depending on the value of the parameter
     * mode :
     * - I: Sets the mode to INPUT
     * - O: Sets the mode to OUTPUT
     * - P: Sets the mode to INPUT_PULLUP
     */

    switch (mode){
        case 'I':
            pinMode(pin_number, INPUT);
            break;
        case 'O':
            pinMode(pin_number, OUTPUT);
            break;
        case 'P':
            pinMode(pin_number, INPUT_PULLUP);
            break;
    }
}

void digital_read(int pin_number){
    /*
     * Performs a digital read on pin_number and returns the value read to serial
     * in this format: D{pin_number}:{value}\n where value can be 0 or 1
     */

    digital_value = digitalRead(pin_number);
    Serial.print('D');
    Serial.print(pin_number);
    Serial.print(':');
    Serial.println(digital_value); // Adds a trailing \n
}

void analog_read(int pin_number){
    /*
     * Performs an analog read on pin_number and returns the value read to serial
     * in this format: A{pin_number}:{value}\n where value ranges from 0 to 1023
     */

    analog_value = analogRead(pin_number);
    Serial.print('A');
    Serial.print(pin_number);
    Serial.print(':');
    Serial.println(analog_value); // Adds a trailing \n
}

void digital_write(int pin_number, int digital_value){
    /*
     * Performs a digital write on pin_number with the digital_value
     * The value must be 1 or 0
     */
  digitalWrite(pin_number, digital_value);
}

void analog_write(int pin_number, int analog_value){
    /*
   * Performs an analog write on pin_number with the analog_value
   * The value must be range from 0 to 255
     */
  analogWrite(pin_number, analog_value);
}


void setup()
{
  Serial.begin(9600);
  pinMode(8,OUTPUT);//Bulb
  pinMode(11,OUTPUT);//Case Fan
  pinMode(10,OUTPUT);//CPU Fan
  pinMode(ldrPin,INPUT);//LDR Sensor
  pinMode(TempPin,INPUT);//LDR Sensor
  myservo.attach(9);//Door Lock

  digitalWrite(8, LOW);
  digitalWrite(11, LOW);
  digitalWrite(10, LOW);
  myservo.write(0);
}

void loop()
{
  int LDRVal = analogRead(ldrPin);
  int LMVal = analogRead(TempPin);
  float voltage = LMVal * (5000 / 1024.0);
  float temperature = voltage/10;
  Serial.println("Sunlight: ");
  Serial.println(LDRVal);
  Serial.println("Temperature: ");
  Serial.println(temperature);
  
  if (Serial.available() > 0)
{
  
  // parse incoming Serial data to integer
  pinStatus = Serial.parseInt();
  switch (pinStatus)
  {
    case 1:
    digitalWrite(10, HIGH);
    break;
    case 2:
    digitalWrite(10, LOW);
    break;
    case 3:
    digitalWrite(11, HIGH);
    break;
    case 4:
    digitalWrite(11, LOW);
    break;
    case 5:
    digitalWrite(8, HIGH);
    break;
    case 6:
    digitalWrite(8, LOW);
    break;
    case 7:
    myservo.write(180);
    break;
    case 8:
    myservo.write(0);
    break;
    default:
    break;
    }

    operation = Serial.read();
        delay(wait_for_transmission); // If not delayed, second character is not correctly read
        mode = Serial.read();
        pin_number = Serial.parseInt(); // Waits for an int to be transmitted
        if (Serial.read()==':'){
            value_to_write = Serial.parseInt(); // Collects the value to be written
        }
        switch (operation){
            case 'R': // Read operation, e.g. RD12, RA4
                if (mode == 'D'){ // Digital read
                    digital_read(pin_number);
                } else if (mode == 'A'){ // Analog read
                    analog_read(pin_number);
       } else {
          break; // Unexpected mode
        }
                break;

            case 'W': // Write operation, e.g. WD3:1, WA8:255
                if (mode == 'D'){ // Digital write
                    digital_write(pin_number, value_to_write);
                } else if (mode == 'A'){ // Analog write
                    analog_write(pin_number, value_to_write);
                } else {
                    break; // Unexpected mode
                }
                break;

            case 'M': // Pin mode, e.g. MI3, MO3, MP3
                set_pin_mode(pin_number, mode); // Mode contains I, O or P (INPUT, OUTPUT or PULLUP_INPUT)
                break;

            default: // Unexpected char
                break;
        }
  }

  if (LDRVal <=30)
  {
    delay(1000);
    digitalWrite(8, HIGH);
    Serial.println("It is Dark, Bulb will turn on");
  }
  else{
    delay(1000);
    digitalWrite(8, LOW);
    Serial.println("It is Bright, Bulb will turn off");
  }
  delay(1000);
}
