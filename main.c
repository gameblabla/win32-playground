/*
4k.
A minimal starting point for 4k GDI demo/game programming.
* 
* Stripped down and modified for my own use - Gameblabla
* 
*/

#define STRICT
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <mmsystem.h>
#include <stdint.h>
#include <stdbool.h>
#include "main.h"

#include "s.h"
#include "pal.h"

#ifdef SIXTEEN_BPP
BITMAPINFO bi;
#else
BITMAPINFO* bi;
#endif
uint8_t var1;

// Initialization and shutdown:

static inline void Initialize()
{
	/* It's set to 640x480 but actually, we will detect the screen resolution later */
    g_hDC = GetDC(CreateWindow("STATIC", 0, WS_POPUP | WS_VISIBLE | WS_MAXIMIZE, 0, 0, 640, 480, 0, 0, 0, 0));

	#ifdef SIXTEEN_BPP
	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biWidth = INTERNAL_WINDOW_WIDTH;
	bi.bmiHeader.biHeight = -INTERNAL_WINDOW_HEIGHT;  //negative so (0,0) is at top left
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 16;
	#else
	bi = (BITMAPINFO*)HeapAlloc(GetProcessHeap(), 0, sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*256);
	bi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bi->bmiHeader.biWidth = INTERNAL_WINDOW_WIDTH;
	bi->bmiHeader.biHeight = -INTERNAL_WINDOW_HEIGHT;  //negative so (0,0) is at top left
	bi->bmiHeader.biPlanes = 1;
	bi->bmiHeader.biBitCount = 8;
	bi->bmiHeader.biClrUsed	= sizeof(pal) / 3;
	for(var1=0;var1<sizeof(pal) / 3;var1++)
	{
		bi->bmiColors[var1].rgbRed   = pal[(var1*3)+0];
		bi->bmiColors[var1].rgbGreen = pal[(var1*3)+1];
		bi->bmiColors[var1].rgbBlue  = pal[(var1*3)+2];
	}
	#endif
}


 // Update step and main loop:

static inline void Update ()
{
    // just paint the whole back buffer in blue:
    //RECT rcWindow = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    //FillRect(g_hMemDC, &rcWindow, g_hBlue);
	
	StretchDIBits(g_hDC,0,0,GetDeviceCaps(g_hDC,HORZRES),GetDeviceCaps(g_hDC,VERTRES),0,0,INTERNAL_WINDOW_WIDTH,INTERNAL_WINDOW_HEIGHT, pict,
	#ifdef SIXTEEN_BPP
	&bi,
	#else
	bi,
	#endif
	DIB_RGB_COLORS, SRCCOPY);
}


static inline void Loop ()
{
	MSG msg;
    do
    {
       // DWORD dwStart = timeGetTime();

        // poll windows events:
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0) DispatchMessage(&msg);
		
        // update and copy the back buffer:
        Update();

        // sleep until next step:
        /*DWORD dwDelta = timeGetTime() - dwStart;
        if (dwDelta < STEP_RATE) {
            Sleep(STEP_RATE - dwDelta);
        }*/
    } while(!GetAsyncKeyState(VK_ESCAPE));
}


// Entry point:

void WinMainCRTStartup(VOID)
{
	Initialize();
    Loop();
}

