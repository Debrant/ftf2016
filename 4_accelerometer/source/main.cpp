#include "mbed-drivers/mbed.h"       // this tells us to load mbed OS related functions
#include "tones.h"                   // list of all the tones and their frequencies
#include "fxos8700cq/fxos8700cq.h"   // library for the accelerometer

using namespace minar;

InterruptIn btn2(SW2);               // we create a variable 'btn2', use it as an in port
InterruptIn btn3(SW3);               // we create a variable 'btn3', use it as an in port

PwmOut buzzer(D3);                   // our buzzer is a PWM output (pulse-width modulation)

// Set up the accelerometer (this is specific to the onboard one)
InterruptIn accel_interrupt_pin(PTC13);
FXOS8700CQ accel(PTE25, PTE24, FXOS8700CQ_SLAVE_ADDR1);

static void silence() {
    buzzer.write(0.0f); // silence!
}

// this is our function that plays a tone. 
// Takes in a tone frequency, and after duration (in ms.) we stop playing again
static void play_tone(int tone) {
    buzzer.period_us(tone);
    buzzer.write(0.10f); // 10% duty cycle, otherwise it's too loud
}

static void play_note1() {
    play_tone(NOTE_C4);
}
static void play_note2() {
    play_tone(NOTE_D4);
}

// YOUR CODE HERE

// this code runs when the microcontroller starts up
void app_start(int, char**) {
    // play note when we fall
    btn2.fall(play_note1);
    btn3.fall(play_note2);
    
    // silence when we rise
    btn2.rise(silence);
    btn3.rise(silence);
    
    // accelerometer setup
    accel_interrupt_pin.fall(play_note3);
    accel_interrupt_pin.mode(PullUp);
    
    accel.config_int();      // enabled interrupts from accelerometer
    accel.config_feature();  // turn on motion detection
    accel.enable();          // enable accelerometer
}

