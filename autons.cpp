#include "main.h"

/////
// For installation, upgrading, documentations, and tutorials, check out our website!
// https://ez-robotics.github.io/EZ-Template/
/////

// These are out of 127
const int DRIVE_SPEED = 100;
const int TURN_SPEED = 90;
const int SWING_SPEED = 90;

///
// Constants
///
void default_constants() {
  chassis.pid_heading_constants_set(11, 0, 20);
  chassis.pid_drive_constants_set(20, 0, 100);
  chassis.pid_turn_constants_set(3, 0.05, 20, 15);
  chassis.pid_swing_constants_set(6, 0, 65);

  chassis.pid_turn_exit_condition_set(80_ms, 3_deg, 250_ms, 7_deg, 500_ms, 500_ms);
  chassis.pid_swing_exit_condition_set(80_ms, 3_deg, 250_ms, 7_deg, 500_ms, 500_ms);
  chassis.pid_drive_exit_condition_set(80_ms, 1_in, 250_ms, 3_in, 500_ms, 500_ms);

  chassis.pid_turn_chain_constant_set(3_deg);
  chassis.pid_swing_chain_constant_set(5_deg);
  chassis.pid_drive_chain_constant_set(3_in);

  chassis.slew_drive_constants_set(7_in, 80);
}

///
// Drive Example
///
void drive_example() {
  // The first parameter is target inches
  // The second parameter is max speed the robot will drive at
  // The third parameter is a boolean (true or false) for enabling/disabling a slew at the start of drive motions
  // for slew, only enable it when the drive distance is greater than the slew distance + a few inches

  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  chassis.pid_drive_set(-12_in, DRIVE_SPEED);
  chassis.pid_wait();

  chassis.pid_drive_set(-12_in, DRIVE_SPEED);
  chassis.pid_wait();
}

///
// Turn Example
///
void turn_example() {
  // The first parameter is the target in degrees
  // The second parameter is max speed the robot will drive at

  chassis.pid_turn_set(90_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();
}

///
// Combining Turn + Drive
///
void drive_and_turn() {
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(-45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_drive_set(-24_in, DRIVE_SPEED, true);
  chassis.pid_wait();
}

///
// Wait Until and Changing Max Speed
///
void wait_until_change_speed() {
  // pid_wait_until will wait until the robot gets to a desired position

  // When the robot gets to 6 inches slowly, the robot will travel the remaining distance at full speed
  chassis.pid_drive_set(24_in, 30, true);
  chassis.pid_wait_until(6_in);
  chassis.pid_speed_max_set(DRIVE_SPEED);  // After driving 6 inches at 30 speed, the robot will go the remaining distance at DRIVE_SPEED
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(-45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();

  // When the robot gets to -6 inches slowly, the robot will travel the remaining distance at full speed
  chassis.pid_drive_set(-24_in, 30, true);
  chassis.pid_wait_until(-6_in);
  chassis.pid_speed_max_set(DRIVE_SPEED);  // After driving 6 inches at 30 speed, the robot will go the remaining distance at DRIVE_SPEED
  chassis.pid_wait();
}

///
// Swing Example
///
void swing_example() {
  // The first parameter is ez::LEFT_SWING or ez::RIGHT_SWING
  // The second parameter is the target in degrees
  // The third parameter is the speed of the moving side of the drive
  // The fourth parameter is the speed of the still side of the drive, this allows for wider arcs

  chassis.pid_swing_set(ez::LEFT_SWING, 45_deg, SWING_SPEED, 45);
  chassis.pid_wait();

  chassis.pid_swing_set(ez::RIGHT_SWING, 0_deg, SWING_SPEED, 45);
  chassis.pid_wait();

  chassis.pid_swing_set(ez::RIGHT_SWING, 45_deg, SWING_SPEED, 45);
  chassis.pid_wait();

  chassis.pid_swing_set(ez::LEFT_SWING, 0_deg, SWING_SPEED, 45);
  chassis.pid_wait();
}

///
// Motion Chaining
///
void motion_chaining() {
  // Motion chaining is where motions all try to blend together instead of individual movements.
  // This works by exiting while the robot is still moving a little bit.
  // To use this, replace pid_wait with pid_wait_quick_chain.
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait_quick_chain();

  chassis.pid_turn_set(-45_deg, TURN_SPEED);
  chassis.pid_wait_quick_chain();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();

  // Your final motion should still be a normal pid_wait
  chassis.pid_drive_set(-24_in, DRIVE_SPEED, true);
  chassis.pid_wait();
}

///
// Auto that tests everything
///
void combining_movements() {
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_swing_set(ez::RIGHT_SWING, -45_deg, SWING_SPEED, 45);
  chassis.pid_wait();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_drive_set(-24_in, DRIVE_SPEED, true);
  chassis.pid_wait();
}

///
// Interference example
///
void tug(int attempts) {
  for (int i = 0; i < attempts - 1; i++) {
    // Attempt to drive backward
    printf("i - %i", i);
    chassis.pid_drive_set(-12_in, 127);
    chassis.pid_wait();

    // If failsafed...
    if (chassis.interfered) {
      chassis.drive_sensor_reset();
      chassis.pid_drive_set(-2_in, 20);
      pros::delay(1000);
    }
    // If the robot successfully drove back, return
    else {
      return;
    }
  }
}

// If there is no interference, the robot will drive forward and turn 90 degrees.
// If interfered, the robot will drive forward and then attempt to drive backward.
void interfered_example() {
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  if (chassis.interfered) {
    tug(3);
    return;
  }

  chassis.pid_turn_set(90_deg, TURN_SPEED);
  chassis.pid_wait();
}

// . . .
// Make your own autonomous functions here!
// . . .

void auton_0() {
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();
}

void auton_skills() {
  chassis.pid_drive_set(-8_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  Clamp.set_value(true);
  pros::delay(100);
  chassis.pid_turn_set(-90_deg, DRIVE_SPEED, false);
  chassis.pid_wait();
  Roller.move(120);
  Intake.move(-120);
  chassis.pid_drive_set(25_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  chassis.pid_drive_set(25_in, DRIVE_SPEED / 2, true);
  chassis.pid_wait();
  chassis.pid_drive_set(-15_in, DRIVE_SPEED / 2, true);
  chassis.pid_wait();
  chassis.pid_turn_set(120_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(32_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  chassis.pid_turn_set(-135_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  chassis.pid_turn_set(-90_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(32_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  chassis.pid_turn_set(-90_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(-20_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  Clamp.set_value(false);
  //
  chassis.pid_turn_set(-30_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(32_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  chassis.pid_turn_set(-180_deg, TURN_SPEED);
  chassis.pid_wait();
  //
  chassis.pid_drive_set(-5_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  Clamp.set_value(true);
  pros::delay(100);
  chassis.pid_turn_set(40_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(25_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  chassis.pid_drive_set(25_in, DRIVE_SPEED / 2, true);
  chassis.pid_wait();
  chassis.pid_drive_set(-15_in, DRIVE_SPEED / 2, true);
  chassis.pid_wait();
  chassis.pid_turn_set(-120_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(32_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  chassis.pid_turn_set(135_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  chassis.pid_turn_set(90_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(32_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  chassis.pid_turn_set(90_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(-20_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  Clamp.set_value(false);

}

void auton_bn() {
  chassis.pid_drive_set(-36_in, DRIVE_SPEED, true);
  pros::delay(200);
  chassis.pid_wait();
  chassis.pid_drive_set(-2_in, DRIVE_SPEED, false);
  chassis.pid_wait();
  Clamp.set_value(true);
  Roller.move(120);
  Intake.move(-120);
  chassis.pid_turn_set(-39_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(25_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  chassis.pid_drive_set(-25_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  chassis.pid_turn_set(-93_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_drive_set(20.5_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  chassis.pid_drive_set(-21.5_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  chassis.pid_drive_set(6_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  Clamp.set_value(false);
  chassis.pid_turn_set(-145_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(14_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  // Reverse into Post to grab goal
  /*
  chassis.pid_drive_set(-40_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  Clamp.set_value(true);
  chassis.pid_drive_set(-7_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  // Start spin to get ring
  Intake.move(-120);
  // Turn towards ring
  Roller.move(120);
  chassis.pid_wait();
  chassis.pid_turn_set(-43_deg, TURN_SPEED);
  chassis.pid_wait();
  // Drive to ring
  chassis.pid_drive_set(24_in, 85, true);
  chassis.pid_wait();
  pros::delay(10);
  // Move back
  chassis.pid_drive_set(-20_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  chassis.pid_turn_set(22_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(20_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  pros::delay(60);
  Roller.move(0);
  Intake.move(0);
  */
}

void auton_rn() {
  // Reverse into Post to grab goal
  Clamp.set_value(true);
  chassis.pid_drive_set(-48_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  // Grab goal
  Clamp.set_value(false);
  // Start spin to get ring
  Intake.move(-120);
  // Turn towards ring
  chassis.pid_turn_set(125_deg, TURN_SPEED);
  chassis.pid_wait();
  Roller.move(120);
  // Drive to ring
  chassis.pid_drive_set(18_in, 85, true);
  chassis.pid_wait();
  pros::delay(10);
  // Move back
  chassis.pid_drive_set(-20_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  chassis.pid_turn_set(22_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(60_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  pros::delay(10);
  Intake.move(0);
  Roller.move(0);
}

void auton_bp() {
  Roller.move(120);
  chassis.pid_drive_set(51.5_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  chassis.pid_turn_set(18_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(-11.75_in, DRIVE_SPEED, 5);
  chassis.pid_wait();
  chassis.pid_drive_set(.30_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  Clamp.set_value(true);
  pros::delay(500);
  chassis.pid_drive_set(18.75_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  Intake.move(-120);
  chassis.pid_turn_set(130_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(60_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  /*
  // Reverse into Post to grab goal
  Clamp.set_value(true);
  chassis.pid_drive_set(-48_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  // Grab goal
  Clamp.set_value(false);
  // Start spin to get ring
  Intake.move(-120);
  // Turn towards ring
  chassis.pid_turn_set(-40_deg, TURN_SPEED);
  chassis.pid_wait();
  Roller.move(120);
  // Drive to ring
  chassis.pid_drive_set(50_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  // Spin to face next ring
  chassis.pid_turn_set(140_deg, TURN_SPEED);
  chassis.pid_wait();
  // Move back
  chassis.pid_drive_set(-20_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  Clamp.set_value(true);
  chassis.pid_drive_set(15_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  Intake.move(0);
  Roller.move(0);
  */
}

void auton_rp() {
  // Reverse into Post to grab goal
  Clamp.set_value(true);
  chassis.pid_drive_set(-48_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  // Grab goal
  Clamp.set_value(false);
  // Start spin to get ring
  Intake.move(-120);
  // Turn towards ring
  chassis.pid_turn_set(40_deg, TURN_SPEED);
  chassis.pid_wait();
  Roller.move(120);
  // Drive to ring
  chassis.pid_drive_set(50_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  // Spin to face next ring
  chassis.pid_turn_set(-140_deg, TURN_SPEED);
  chassis.pid_wait();
  // Move back
  chassis.pid_drive_set(-20_in, DRIVE_SPEED, true);
  chassis.pid_wait();
  Clamp.set_value(true);
  chassis.pid_drive_set(15_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  Intake.move(0);
  Roller.move(0);
}

void skills() {
}