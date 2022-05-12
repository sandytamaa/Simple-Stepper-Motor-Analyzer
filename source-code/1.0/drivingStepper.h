#include <TMCStepper.h>

#define EN_PIN    PC2   // Enable
#define DIR_PIN   PA2   // Direction
#define STEP_PIN  PC3   // Step
#define CS_PIN    PC1   // Chip select
#define SW_MOSI   PA7   // Software Master Out Slave In (MOSI)
#define SW_MISO   PA6   // Software Master In Slave Out (MISO)
#define SW_SCK    PA5   // Software Slave Clock (SCK)


#define R_SENSE 0.075f // Match to your driver
                     // SilentStepStick series use 0.11
                     // UltiMachine Einsy and Archim2 boards use 0.2
                     // Panucatt BSD2660 uses 0.1
                     // Watterott TMC5160 uses 0.075

// Select your stepper driver type
//TMC2130Stepper driver = TMC2130Stepper(CS_PIN, R_SENSE); // Hardware SPI
//TMC2130Stepper driver = TMC2130Stepper(CS_PIN, R_SENSE, SW_MOSI, SW_MISO, SW_SCK); // Software SPI
//TMC2660Stepper driver = TMC2660Stepper(CS_PIN, R_SENSE); // Hardware SPI
//TMC2660Stepper driver = TMC2660Stepper(CS_PIN, R_SENSE, SW_MOSI, SW_MISO, SW_SCK);
//TMC5160Stepper driver = TMC5160Stepper(CS_PIN, R_SENSE);
TMC5160Stepper driver = TMC5160Stepper(CS_PIN, R_SENSE, SW_MOSI, SW_MISO, SW_SCK);
bool dir = true;

void setupDriverStepper(int current) {
	// while(!Serial);
	Serial.println("Start...");
	driver.begin(); 			// Initiate pins and registeries
	driver.rms_current(current); 	// Set stepper current to 600mA. The command is the same as command TMC2130.setCurrent(600, 0.11, 0.5);
	driver.en_pwm_mode(1);  	// Enable extremely quiet stepping
  driver.microsteps(256);

	pinMode(EN_PIN, OUTPUT);
	pinMode(STEP_PIN, OUTPUT);
	digitalWrite(EN_PIN, LOW); 	// Enable driver in hardware

	Serial.print("DRV_STATUS=0b");
	Serial.println(driver.DRV_STATUS(), BIN);
}

void driveStepperCCW()
{
//	 digitalWrite(DIR_PIN, HIGH);
	digitalWrite(STEP_PIN, HIGH);
	delayMicroseconds(2);
	digitalWrite(STEP_PIN, LOW);
	delayMicroseconds(2);
	
	driver.shaft(0);
}

void driveStepperCW()
{
//   digitalWrite(DIR_PIN, HIGH);
  digitalWrite(STEP_PIN, HIGH);
  delayMicroseconds(2);
  digitalWrite(STEP_PIN, LOW);
  delayMicroseconds(2);
  
  driver.shaft(1);
}
