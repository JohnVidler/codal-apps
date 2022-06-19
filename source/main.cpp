#include "MicroBit.h"

// The Micro:bit control object
MicroBit uBit;

// An update to the basic "Disco Lights" demo found at:
// https://microbit.org/projects/make-it-code-it/disco-lights/
// 
// This one adds a slight sustain time to the LED on-time, so they fade nicely
// rather than snapping off immediately.
void disco_lights()
{
    // Turn on the audio subsystem
    MicroBitAudio::requestActivation();
    
    // Create an image with full brightness on all pixels
    const char * const image ="\
        255,255,255,255,255\n\
        255,255,255,255,255\n\
        255,255,255,255,255\n\
        255,255,255,255,255\n\
        255,255,255,255,255\n";

    // Set the screen image to our new image
    uBit.display.image = Image( image );

    // Create a variable to hold the current brightness
    float sustain = 0.0;

    // Loop forever
    while( true )
    {
        // Grab a sample and compare (and update) the sustain value if we're louder than it
        float sample = uBit.audio.level->getValue();
        if( sustain < sample )
            sustain = sample;

        // Actually write the brightness value to the entire screen
        uBit.display.setBrightness( (int)sustain );

        // Decay the brightness by 85% each loop if its non-zero
        if( sustain > 0 )
            sustain = sustain * 0.85;

        // Delay a little, to slow down the effect
        uBit.sleep( 10 );
    }
}

// This version of the classic "Disco Lights" demo picks a random subset of LEDs to
// turn on; the louder the sound, the more LEDs are illuminated.
// Combined with a per-pixel sustain, this gives us a very nice fading grid effect
// similar to that seen on some sci-fi computers :)
void fancy_disco_lights()
{
    // Turn on the audio subsystem
    MicroBitAudio::requestActivation();

    // Create a blank image to clear the screen that we can use later
    const char * image ="\
        0,0,0,0,0\n\
        0,0,0,0,0\n\
        0,0,0,0,0\n\
        0,0,0,0,0\n\
        0,0,0,0,0\n";

    // Set the current screen image to our blank one
    uBit.display.image = Image( image );

    // Loop forever
    while( true )
    {
        // Grab a sample
        int sample = uBit.audio.level->getValue();

        // Get a pointer to wherever CODAL has put the bitmap of our image
        uint8_t * bitmap = uBit.display.image.getBitmap();

        // Update the bitmap values
        for( int i=0; i<sample/10; i++ )
        {
            int offset = uBit.random( 25 );
            if( bitmap[offset] < sample )
                bitmap[offset] = 255;
        }
        
        // Decay everything on screen by 10 levels
        for( int j=0; j<10; j++ )
            for( int i=0; i<25; i++ )
                if( bitmap[i] > 0 )
                    bitmap[i] = bitmap[i] - 1;

        // Delay a little
        uBit.sleep( 10 );
    }
}

// For fans of Knight Rider, this demo attempts to replicate the screen on KITT which
// responds to his voice as the car speaks.
// Its not quite perfect, and I'd gladly accept patches to this one if someone wants to
// tune the gain values some more, but as it is, its a nice effect anyway.
void night_rider()
{
    // Turn on the audio subsystem
    MicroBitAudio::requestActivation();

    // Create a new blank image for us to use
    const char * image ="\
        0,0,0,0,0\n\
        0,0,0,0,0\n\
        0,0,0,0,0\n\
        0,0,0,0,0\n\
        0,0,0,0,0\n";
    
    // Create a gain value for every pixel on the screen
    // This we will use to scale the brightness we apply for each pixel.
    const float gain [] = {
        0.45, 0.45, 0.45, 0.45, 0.45,
        0.75, 0.75, 0.75, 0.75, 0.75,
        0.85, 0.85, 0.95, 0.85, 0.85,
        0.75, 0.75, 0.75, 0.75, 0.75,
        0.45, 0.45, 0.45, 0.45, 0.45,
    };

    // Set the screen image to our new image
    uBit.display.image = Image( image );

    // Loop forever
    while( true )
    {
        // Grab a sound level sample
        float sample = uBit.audio.level->getValue();

        // Get a pointer to wherever CODAL has put our bitmap
        uint8_t * bitmap = uBit.display.image.getBitmap();

        // For every pixel...
        for( int i=0; i<25; i++ )
        {
            // If the brightness at this pixel is less than our sample, set it to sample * gain for this pixel
            if( bitmap[i] < (int)sample )
                bitmap[i] = (int)(sample * gain[i]);
        }
        
        // Decay everything on screen by 10.
        // We do this incrementally, as we don't want to accidentally go under zero!
        for( int i=0; i<25; i++ )
            if( bitmap[i] > 0 )
                bitmap[i] -= 1;

        // Delay a little
        uBit.sleep( 10 );
    }
}

// Its a face! The louder the sounds around the micro:bit, the wider the mouth will open :D
// Try this demo while playing sung orchestral music. Bonus points for playing sound into a
// bunch of micro:bit boards all with this programmed for a choir :O :o :D :o
void face_demo()
{
    // Turn on the audio subsystem
    MicroBitAudio::requestActivation();
    
    // Create a few animation frames for our face image.
    // Later images are used for louder sounds.
    const int frames [5][25] = {
        {
            0,0,0,0,0,
            0,255,0,255,0,
            0,0,0,0,0,
            0,255,255,255,0,
            0,0,0,0,0,
        },
        {
            0,0,0,0,0,
            0,255,0,255,0,
            0,0,0,0,0,
            0,0,255,0,0,
            0,0,0,0,0,
        },
        {
            0,0,0,0,0,
            0,255,0,255,0,
            0,0,0,0,0,
            0,255,0,255,0,
            0,0,0,0,0,
        },
        {
            0,0,0,0,0,
            0,255,0,255,0,
            0,0,255,0,0,
            0,255,0,255,0,
            0,0,255,0,0,
        },
        {
            0,0,0,0,0,
            0,255,0,255,0,
            0,255,255,255,0,
            255,0,0,0,255,
            0,255,255,255,0,
        }
    };

    while( true )
    {
        // Grab a sound level sample, and use it to determine the frame for our animation
        // Handily, as we have 5 frames of animation, and getValue() will return a value
        // between 0 and 255, we can just divide our sound level value by 25 to get a frame.
        int sample = uBit.audio.level->getValue() / 25;
        
        // Just in case we ever go over index 4 (aka, frame 5, as arrays start at zero),
        // reset our sample to 4.
        if( sample > 4 )
            sample = 4;

        // Get a pointer to wherever CODAL has put the bitmap of our image
        uint8_t * bitmap = uBit.display.image.getBitmap();

        // Write each value in our current frame to a pixel on the screen.
        // This effectively copies the bit of the array we care about to the screen, but
        // we could have used the memcpy() function and pointer offsets to do the same.
        for( int i=0; i<25; i++ )
            bitmap[i] = frames[sample][i];

        // Delay a little
        uBit.sleep( 10 );
    }
}

// Our main function, run at startup
int main() {

    // Uncomment one of the following to pick a demo
    //disco_lights();
    fancy_disco_lights();
    //night_rider();
    //face_demo();

    // Will never return, but here so the compiler is happy :)
    return 0;
}