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

#include "s.h"
#include "pal.h"

HINSTANCE g_hInstance;
HWND g_hWnd;
HDC g_hDC;

#define DEFAULT_WINDOW_WIDTH 640
#define DEFAULT_WINDOW_HEIGHT 480

#define INTERNAL_WINDOW_WIDTH 120
#define INTERNAL_WINDOW_HEIGHT 90
/* Minimum amount of milliseconds for each update cycle:*/
#define STEP_RATE 13

#ifdef SIXTEEN_BPP
BITMAPINFO bi;
#else
BITMAPINFO* bi;
#endif
uint8_t var1;

// Entry point:

void WinMainCRTStartup(VOID)
{
	MSG msg;
	/* It's set to 640x480 but actually, we will detect the screen resolution later (unless WINDOWED is set) */
	g_hDC = GetDC(CreateWindow("STATIC", 0, 
	#ifdef WINDOWED
	WS_OVERLAPPED | WS_VISIBLE,
	#else
	WS_POPUP | WS_VISIBLE | WS_MAXIMIZE,
	#endif
	0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT + 25, 0, 0, 0, 0));

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
	
    do
    {
       // DWORD dwStart = timeGetTime();

        // poll windows events:
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0) DispatchMessage(&msg);
		
        // update and copy the back buffer:
		StretchDIBits(g_hDC,0,0,
		#ifdef WINDOWED
		DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT,
		#else
		GetDeviceCaps(g_hDC,HORZRES),GetDeviceCaps(g_hDC,VERTRES),
		#endif
		0,0,INTERNAL_WINDOW_WIDTH,INTERNAL_WINDOW_HEIGHT, pict,
		#ifdef SIXTEEN_BPP
		&bi,
		#else
		bi,
		#endif
		DIB_RGB_COLORS, SRCCOPY);

        // sleep until next step:
        /*DWORD dwDelta = timeGetTime() - dwStart;
        if (dwDelta < STEP_RATE) {
            Sleep(STEP_RATE - dwDelta);
        }*/
    } while(!GetAsyncKeyState(VK_ESCAPE));
}

