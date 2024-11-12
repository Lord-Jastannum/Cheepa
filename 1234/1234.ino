// Pin definitions
#define ULTRASONIC_TRIG_PIN 9  // Trigger pin for ultrasonic sensor
#define ULTRASONIC_ECHO_PIN 10 // Echo pin for ultrasonic sensor
#define BUZZER_PIN 8           // Pin for buzzer
#define LIGHT_SENSOR_PIN A0    // Analog pin for light sensor (OP101)

// Constants
#define DISTANCE_THRESHOLD 50  // Distance threshold for obstacle detection (in cm)
#define MIN_BEEP_DELAY 100     // Minimum delay between beeps (in ms)
#define MAX_BEEP_DELAY 1000    // Maximum delay between beeps (in ms)
#define LIGHT_THRESHOLD 500    // Threshold for light sensor (value from 0 to 1023)

void setup() {
    Serial.begin(9600);           // Initialize Serial Monitor at 9600 baud rate

    // Pin setup
    pinMode(ULTRASONIC_TRIG_PIN, OUTPUT); // Set ultrasonic trigger pin as output
    pinMode(ULTRASONIC_ECHO_PIN, INPUT);  // Set ultrasonic echo pin as input
    pinMode(BUZZER_PIN, OUTPUT);          // Set buzzer pin as output
}

void loop() {
    long duration;   // Variable to store duration of pulse
    int distance;    // Variable to store calculated distance
    int beepDelay;   // Variable to store delay between beeps
    int lightLevel;  // Variable to store light sensor reading

    // Read the light sensor
    lightLevel = analogRead(LIGHT_SENSOR_PIN);

    // Trigger the ultrasonic sensor
    digitalWrite(ULTRASONIC_TRIG_PIN, LOW);   // Ensure trigger pin is low
    delayMicroseconds(2);                    // Wait for 2 microseconds
    digitalWrite(ULTRASONIC_TRIG_PIN, HIGH);  // Set trigger pin high
    delayMicroseconds(10);                   // Wait for 10 microseconds
    digitalWrite(ULTRASONIC_TRIG_PIN, LOW);   // Set trigger pin low again

    // Measure the duration of the echo pulse
    duration = pulseIn(ULTRASONIC_ECHO_PIN, HIGH);  // Measure the pulse width
    distance = duration * 0.0344 / 2;              // Convert duration to distance in cm

    // Calculate delay between beeps based on distance and light level
    if (distance < DISTANCE_THRESHOLD || lightLevel < LIGHT_THRESHOLD) {
        // Calculate distance-based beep delay
        int distanceBeepDelay = map(distance, 0, DISTANCE_THRESHOLD, MIN_BEEP_DELAY, MAX_BEEP_DELAY);
        // Calculate light-based beep delay
        int lightBeepDelay = map(lightLevel, 0, LIGHT_THRESHOLD, MIN_BEEP_DELAY, MAX_BEEP_DELAY);

        // Use the shorter of the two delays to set the beep frequency
        beepDelay = min(distanceBeepDelay, lightBeepDelay);

        // Generate a beep with the calculated delay
        digitalWrite(BUZZER_PIN, HIGH); // Turn the buzzer on
        delay(50);                      // Beep duration
        digitalWrite(BUZZER_PIN, LOW);  // Turn the buzzer off

        // Wait before the next beep
        delay(beepDelay);
    } else {
        // No beeping if distance is beyond threshold and light level is sufficient
        digitalWrite(BUZZER_PIN, LOW);
    }

    // Output the distance and light level to the Serial Monitor
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.print(" cm, Light Level: ");
    Serial.print(lightLevel);
    Serial.print(" , Beep Delay: ");
    Serial.println(beepDelay);

    delay(100); // Small delay before the next measurement
}
