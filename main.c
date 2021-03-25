#include <windows.h>
#include <time.h>
#include <stdint.h>
#include "s.h"
#include "pal.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define INTERNAL_WINDOW_WIDTH 640
#define INTERNAL_WINDOW_HEIGHT 480

//#define FULLSCREEN 1

#ifdef FULLSCREEN
static DEVMODE screenSettings = { 
	{0}, 0, 0, sizeof(screenSettings), 0, DM_PELSWIDTH|DM_PELSHEIGHT,
	{0}, 0, 0, 0, 0, 0, {0}, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0,
	#if(WINVER >= 0x0400)
		0, 0, 0, 0, 0, 0,
			#if (WINVER >= 0x0500) || (_WIN32_WINNT >= 0x0400)
			0, 0
		#endif
	#endif
};
#endif

uint8_t var1;

void WinMainCRTStartup(VOID)
{
	HDC hDC, mdc;
    HBITMAP bitmap;
    HGDIOBJ oldbmp;
    unsigned char* lpBitmapBits;
	BITMAPINFO* bi;
	
    #ifdef FULLSCREEN
	ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN);
	#endif
	HWND window = CreateWindow((LPCSTR)0xC018, 0, WS_POPUP | WS_VISIBLE | WS_BORDER | WS_OVERLAPPEDWINDOW, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, 0, 0);
	
	bi = (BITMAPINFO*)HeapAlloc(GetProcessHeap(), 0, sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*256);
	bi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bi->bmiHeader.biWidth = INTERNAL_WINDOW_WIDTH;
	bi->bmiHeader.biHeight = -INTERNAL_WINDOW_HEIGHT;  //negative so (0,0) is at top left
	bi->bmiHeader.biPlanes = 1;
	bi->bmiHeader.biBitCount = 8;
	/* 254 colors instead of 255 so we can fit it inside of an 8-bits char */
	bi->bmiHeader.biClrUsed	= 255;
	for(var1=0;var1<255;var1++)
	{
		bi->bmiColors[var1].rgbRed   = pal[(var1*3)+0];
		bi->bmiColors[var1].rgbGreen = pal[(var1*3)+1];
		bi->bmiColors[var1].rgbBlue  = pal[(var1*3)+2];
		bi->bmiColors[var1].rgbReserved = 0;
	}
			
	do
	{
			hDC = GetDC(window);
			mdc = CreateCompatibleDC(hDC);
		
			bitmap = CreateDIBSection(mdc, bi, DIB_RGB_COLORS, (VOID**)&lpBitmapBits, NULL, 0);
			oldbmp = SelectObject(mdc, bitmap);
			memcpy(lpBitmapBits, pict, pict_length);
			BitBlt(hDC, 0, 0, INTERNAL_WINDOW_WIDTH, INTERNAL_WINDOW_HEIGHT, mdc, 0, 0, SRCCOPY);

			SelectObject(mdc,oldbmp);  
			DeleteDC(mdc);  
			DeleteObject(bitmap);
	} while(!GetAsyncKeyState(VK_ESCAPE));
	
	//HeapFree(GetProcessHeap(), 0, bi);
}

