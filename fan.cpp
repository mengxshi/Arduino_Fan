#include "Arduino.h"
#include "Servo.h"


#define BRUSHLESSMOTOR_PIN_DATA_1    5 // Pin for the first motor


const int brushlessMotorLowSpeed = 1000;
const int brushlessMotorFastSpeed = 2000;
Servo brushlessMotor1; // Motor 1


char menuOption = 0;
long time0;
unsigned long fanStartTime = 0;
unsigned long fanDuration = 0;


// Function prototype for reset
void (*resetFunc)(void) = 0; // Declare reset function @ address 0


void setup()
{
    Serial.begin(9600);
    while (!Serial);
    Serial.println("start");
   
    brushlessMotor1.attach(BRUSHLESSMOTOR_PIN_DATA_1); // Attach motor 1 to pin 5
    brushlessMotor1.writeMicroseconds(brushlessMotorLowSpeed);
    menuOption = menu();
}


void loop()
{
    if (Serial.available()) {
        char input = Serial.read();
        if (input == '0') {
            // Stop the fan if '0' is entered
            brushlessMotor1.writeMicroseconds(0);
            brushlessMotor1.detach();
            Serial.println("Fan stopped.");
            fanDuration = 0; // Reset fan duration
            menuOption = menu(); // Go back to menu
            //resetFunc(); // Reset the entire code
        }
    }


    if (fanDuration > 0 && brushlessMotor1.attached()) { // Check if the fan is running and attached
        unsigned long currentTime = millis();
        if (currentTime - fanStartTime <= fanDuration) {
            brushlessMotor1.writeMicroseconds(brushlessMotorFastSpeed);
            Serial.println("Fan running at max speed...");
        } else {
            brushlessMotor1.writeMicroseconds(0);
            brushlessMotor1.detach();
            Serial.println("Fan stopped.");
            fanDuration = 0; // Reset fan duration after completion
            menuOption = menu(); // Go back to menu after completing fan operation
            //resetFunc(); // Reset the entire code
        }
    }
}


char menu()
{
    Serial.println(F("\nEnter the duration (in seconds) for which you want the motor to run:"));
   
    while (!Serial.available());


    int userInput = Serial.parseInt(); // Read an integer input
    if (userInput != 0) {
        fanDuration = (userInput + 1) * 1000; // Increment the input by 1 and convert seconds to milliseconds
        fanStartTime = millis();
        brushlessMotor1.attach(BRUSHLESSMOTOR_PIN_DATA_1);
    }
    return '1'; // Always return '1' to indicate motor operation
}
