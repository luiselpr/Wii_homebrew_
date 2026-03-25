#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <aesndlib.h>
#include <gcmodplay.h>

// include generated header
#include "technique_mod.h"
#include "tetrisMusic_mod.h"
#include "avernus_mod.h"

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;
static MODPlay play;

//---------------------------------------------------------------------------------
int main(int argc, char **argv) {
//---------------------------------------------------------------------------------

	// Initialise the video system
	VIDEO_Init();

	// Initialise the attached controllers
	WPAD_Init();

	// Initialise the audio subsystem
	AESND_Init();

	// Obtain the preferred video mode from the system
	// This will correspond to the settings in the Wii menu
	rmode = VIDEO_GetPreferredMode(NULL);

	// Allocate memory for the display in the uncached region
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));

	// Initialise the console, required for printf
	console_init(xfb,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);

	// Set up the video registers with the chosen mode
	VIDEO_Configure(rmode);

	// Tell the video hardware where our display memory is
	VIDEO_SetNextFramebuffer(xfb);

	// Make the display visible
	VIDEO_SetBlack(false);

	// Flush the video register changes to the hardware
	VIDEO_Flush();

	// Wait for Video setup to complete
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();


	// The console understands VT terminal escape codes
	// This positions the cursor on row 2, column 0
	// we can use variables for this with format codes too
	// e.g. printf ("\x1b[%d;%dH", row, column );
	printf("\x1b[2;0H");


	printf("Hello World!");

	MODPlay_Init(&play);
	MODPlay_SetMOD(&play,technique_mod);
	MODPlay_SetVolume(&play,63,63);
	MODPlay_Start(&play);

	static int paused = 0;

	while(SYS_MainLoop()) {

		// Call WPAD_ScanPads each loop, this reads the latest controller states
		WPAD_ScanPads();

		// WPAD_ButtonsDown tells us which buttons were pressed in this loop
		// this is a "one shot" state which will not fire again until the button has been released
		u32 pressed = WPAD_ButtonsDown(0);

		// We return to the launcher application via exit
		if ( pressed & WPAD_BUTTON_HOME ) exit(0);

		u32 held = WPAD_ButtonsHeld(0);
		//B
		if (held & WPAD_BUTTON_B) {
			printf("\x1b[6;0HPresionaste B     ");
		} else {
    printf("\x1b[6;0H                 ");
}
		//Arriba
		if (held & WPAD_BUTTON_UP) {
			printf("\x1b[8;0HPresionaste Arriba         ");
		} else {
    printf("\x1b[8;0H                                 ");
}
		//Abajo
		if (held & WPAD_BUTTON_DOWN) {
			printf("\x1b[10;0HPresionaste Abajo          ");
		} else {
    printf("\x1b[10;0H                 ");
}
		//Izquierda
		if (held & WPAD_BUTTON_LEFT) {
			printf("\x1b[12;0HPresionaste Izquierda      ");
		} else {
    printf("\x1b[12;0H                                ");
}
		//Derecha
		if (held & WPAD_BUTTON_RIGHT) {
			printf("\x1b[14;0HPresionaste Derecha        ");
		} else {
    printf("\x1b[14;0H                             ");
}
		//1
		if (held & WPAD_BUTTON_1) {
			printf("\x1b[16;0HPresionaste 1        ");
		} else {
    printf("\x1b[16;0H                             ");
}
		//2
		if (held & WPAD_BUTTON_2) {
			printf("\x1b[18;0HPresionaste 2        ");
		} else {
    printf("\x1b[18;0H                             ");
}

				if (pressed & WPAD_BUTTON_A) {
				if (paused) {
					MODPlay_Pause(&play, 0);//reanudar
					paused = 0;
					printf("\x1b[4;0HReanudado   ");
				} else {
					MODPlay_Pause(&play, 1);//pausa
					paused = 1;
					printf("\x1b[4;0HPausado     ");
				}
	}

if (held & WPAD_BUTTON_MINUS) {
    MODPlay_Stop(&play);   
    MODPlay_Start(&play);  
    paused = 0;
    printf("\x1b[20;0HReiniciado ");
}  else {
    printf("\x1b[20;0H                               ");
}

		// Wait for the next frame
		VIDEO_WaitVSync();
	}

	return 0;
}
