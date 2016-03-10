# SX Create: Making music on a microcontroller, instructions

Welcome to our session at SX Create! If you have any questions, please just give a shout out to Austin, Jan or Gregory, we're here to help everyone make the most of today.

In this session we'll work through eigth examples together. Jan will demonstrate everything, but this document should help you in case you're stuck or did not completely follow along. If you feel you're way faster than everyone else, we have some extra tasks at the bottom of each example. Please do not get ahead, the later examples all do something with music, and that will be very annoying for the other participants. :-)

## Setup

1. Connect the FRDM-K64F board to your computer. There's 2 micro-USB ports on the board, use the OpenSDA one!
1. If you're on Windows, please install the [serial driver](https://developer.mbed.org/handbook/Windows-serial-configuration). On OS/X and Linux, no action required.
1. The board mounts as a mass-storage device (like a USB drive). Verify that you can see it (drive name will be MBED).
1. Go to http://c9.io/auth/mbed, and sign up for an account.
1. After creating the account you will be at the dashboard. Click 'Create new workspace'.
1. Choose a workspace name (f.e. 'sxsw'), and choose 'Public' as the workspace type.
1. Under 'Clone from...' enter: https://github.com/janjongboom/sxsw.git

An IDE should open. Congratulations!

**Optional:** We can talk to the board via a serial port, but you might need some software. Read [this doc](https://developer.mbed.org/handbook/SerialPC#host-interface-and-terminal-applications) and install required software (like PuTTY on Windows).

## 1. Blinky

1. First we need to set up our project and our target. In the button next to 'Build' in Cloud9, select '1_blinky'.
1. On the button next to that, click 'Search in registry' and select 'frdm-k64f-gcc'.
1. Open sxsw/1_blinky/source/main.cpp in the tree.
1. We need to implement a function that will blink the LED.
1. Under '// YOUR CODE HERE' add the following code

```cpp
static void blinky() {
    // we inverse the value of 'red', toggling the LED every time this function is called
    red = !red;
}
```

1. Now press 'Build' (in the top bar)
1. A file downloads (blinky.bin)
1. Drag the file to the 'MBED' disk
1. The board will flash (green). When it stops, hit the 'Reset' button.
1. Blinky runs!

**Optional:** We use [minar](https://docs.mbed.com/docs/getting-started-mbed-os/en/latest/Full_Guide/MINAR/) as our event scheduler. We use it to periodically run the blinky function, but we can also use it to delay execution. E.g.:

```cpp
Scheduler::postCallback(blinky).delay(milliseconds(500));
```

Will call `blinky` once, in 500 ms. But does not repeat the process. Rewrite the app to use `delay` instead of `period`, but let it still blink the LED. Afterwards see if you can vary the time between blinks ([hint](http://www.cplusplus.com/reference/cstdlib/rand/)).

## 2. Two buttons, two colors

1. Switch projects, click `1_blinky`, and change to `2_two_buttons`
1. Open sxsw/2_two_buttons/source/main.cpp
1. We want to execute a function when the button is clicked
1. We already have code in place which responds when the buttons 'fall', which means the button is pressed
1. Let's write similar code as before, but now with two functions (one for green, one for red)
1. Under '// YOUR CODE HERE' add the following code

```cpp
static void toggle_red() {
    red = !red;
}
static void toggle_green() {
    green = !green;
}
```

1. Hit the Build button and flash the application. Clicking SW2 and SW3 will now toggle the LEDs. Having them both on will also allow you to create yellow.

**Optional:** We use `fall` currently, which triggers when the button is clicked. We can also use `rise` to get an event when the button is no longer clicked. Change the code so that the LEDs only burn when the button is pressed down.

**Optional 2:** When we start a periodic timer with minar, we can cancel the timer as well via:

```cpp
callback_handle_t callback = Scheduler::postCallback(blinky).period(milliseconds(500)).getHandle();
Scheduler::cancelCallback(callback);
```

Use the above pattern to combine 1. and 2. Pressing the button should start blinking the color, instead of turning it on. Pressing the button again should cancel the blinking.

## 3. Sound instead of color

1. Switch projects, click `2_two_buttons`, and change to `3_sound`
1. We have a buzzer, which we can use instead of the color as an output
1. If the buzzer is not yet connected to your board, do xyz
1. We use pulse width modulation to create the square waves, the buzzer is connected to pin 3
1. We have a `play_tone` function, now let's hook it up...
1. Under '// YOUR CODE HERE' add the following code

```cpp
static void play_note1() {
    play_tone(NOTE_C4, 200);
}
static void play_note2() {
    play_tone(NOTE_D4, 200);
}
```

1. Build and flash, when hitting the buttons you'll now get sound (a C and a D tone)
1. Try and play with different tones and tone lengths

**Optional:** Currently tones have fixed length. Change the code to play a tone when the button fall's, and call silence when button rise's. This should give you the ability to play some small (two-tone) songs. Look in the directory for '4_accelerometer' for hints.

## 4. More inputs!

1. Switch projects, click `3_sound` and change to `4_accelerometer`
1. We're going to use the accelerometer movement detector to play a sound too, this means we can knock the board on a table and get a sound
1. A new library is included which does this for you already
1. At the bottom of main.cpp you'll see initialization logic for the accelerometer
1. When the accelerometer triggers (movement detected), we can execute some code again
1. Under '// YOUR CODE HERE' add the following code:

```
static void play_note3() {
    play_tone(NOTE_E4);
    
    Scheduler::postCallback(silence).delay(milliseconds(200));
}
```

1. We manually call `silence` now to get predictable behavior
1. Try and change the code to use `accel_interrupt_pin.rise` to silence instead (remove the Scheduler call from play_note3 as well). See what it does.
1. Change the notes and play some simple melodies

**Optional:** You can read the data from the accelerometer (x, y, z axis) via:

```cpp
static void read_accel() {
    SRAWDATA accel_data;
    SRAWDATA magnet_data;
    accel.get_data(&accel_data, &magnet_data);
    
    // this will show up in PuTTY
    printf("x=%d y=%d z=%d\r\n", accel_data.x, accel_data.y, accel_data.z);
}
```

Use minar to read the data every 10 ms., then use the value from the accelerometer (e.g. `accel_data.z`) and feed it into `play_tone` function. We can now change the sound depending on how you hold the device in physical space. Allow to pause the sound by pressing one of the buttons.

You can play with the distribution of the tone. Just feeding the raw data into the algorithm doesn't generate the nicest effect.

## 4b. Pads (advanced)

1. We can add vibration pads as well, although this requires a bit of assembly
1. Take one of the pads from the front, and a 1 million Ohm resistor (brown, black, green)
1. Take a cable stripper and strip a bit of the red and black wires of the pad
1. Put the red cable into A0 ([pinout](https://developer.mbed.org/media/uploads/GregC/xfrdm-k64f_header-pinout.jpg.pagespeed.ic.GDev93u6zd.jpg))
1. Put the black cable in one of the GND
1. Put the resistor in A0 and GND

Now we can read the value of the sensor via:

```cpp
// top of the file
AnalogIn vibrate(A0);

// with your other functions
static void read_vibrate() {
    // value is between 0 and 65535
    printf("vibrate %d\r\n", vibrate.read_u16());
}

// in app_start
Scheduler::postCallback(read_vibrate).period(milliseconds(30));
```

Now use this reading to generate tones whenever you detect a big enough hit on the pad (> 1000 for example). You can play with different tones, depending on how hard you hit the pad as well.

## 5. Songs

1. We can play tones from various inputs, but we could also play songs that we program in
1. We create a simple format to write notes (in app_start).
1. Now we need to write the code that will play this format.
1. Our `play_song` function takes 3 arguments:
    * The number of notes left
    * The notes left (as a pointer)
    * The durations left (as a pointer)
1. Every time we call this function we subtract 1 from the notes left, and up the notes and durations. When we reach 0 we're done.
1. We add a pause between each note as well.
1. Under 'YOUR CODE HERE' insert:

```cpp
    // melody and duration are pointers, they point to the array of tones and durations we declared earlier
    // every time we call this function, we up these pointers (move one element forward)
    // so the current tone is always the first element of melody (same for duration)
    int tone = melody[0];
    // BPM is quarter notes per minute, so length in milliseconds is:
    int length = (int)(static_cast<float>(BPM / 60) * (1000.0f / (float)duration[0]));
    
    play_tone(tone);

    // is this the last tone?    
    if (notes_left - 1 == 0) {
        // we're done! just finish this note and silence
        Scheduler::postCallback(silence).delay(milliseconds(length / 2));
    }
    else {
        // after half the length of this tone, we silence
        Scheduler::postCallback(silence).delay(milliseconds(length / 2));
        
        // after the full length of this tone, we call ourselves, but up the melody, and down the notes_left
        FunctionPointer3<void, int, int*, int*> fp(play_song);
        Scheduler::postCallback(fp.bind(notes_left - 1, melody + 1, duration + 1)).delay(milliseconds(length));
    }
```

1. Delete the pause between the notes, what do you see?
1. Find some melody and program it in, make some of your own music. Play with the BPM as well to speed things up or slow things down.

