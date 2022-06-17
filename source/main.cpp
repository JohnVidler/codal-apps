#include "MicroBit.h"

// The Micro:bit control object
MicroBit uBit;

// Out main function, run at startup
int main() {

    uBit.audio.requestActivation();
    uBit.audio.setSpeakerEnabled( true );
    

    // Loop forever
    int iter = 0;
    while( true ) {
        DMESG( "%d", iter++ );
        uBit.audio.soundExpressions.playAsync( "010230440050000440044008881023012800000000240000000000000000000000000000" );
        uBit.sleep( 250 );
    }

    // Will never return, but here so the compiler is happy :)
    return 0;
}