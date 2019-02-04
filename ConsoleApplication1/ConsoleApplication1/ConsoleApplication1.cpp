// auraavgscr.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#undef _UNICODE
#include <stdio.h>
#include <tchar.h>
#include <vector>
#include "Header.h"

#define PI 3.1415926

EnumerateMbControllerFunc EnumerateMbController;
SetMbModeFunc SetMbMode;
SetMbColorFunc SetMbColor;
GetMbColorFunc GetMbColor;
GetMbLedCountFunc GetMbLedCount;

EnumerateDramFunc EnumerateDram;
SetDramModeFunc SetDramMode;
SetDramColorFunc SetDramColor;
GetDramColorFunc GetDramColor;
GetDramLedCountFunc GetDramLedCount;

typedef struct {
	double r;       // a fraction between 0 and 1
	double g;       // a fraction between 0 and 1
	double b;       // a fraction between 0 and 1
} rgb;

typedef struct {
	double h;       // angle in degrees
	double s;       // a fraction between 0 and 1
	double v;       // a fraction between 0 and 1
} hsv;

int hsv2rgb(double h, double s, double v)
{
	hsv			in;
	in.h = h;
	in.s = s;
	in.v = v;
	double      hh, p, q, t, ff;
	long        i;
	rgb         out;

	if (in.s <= 0.0) {       // < is bogus, just shuts up warnings
		out.r = in.v;
		out.g = in.v;
		out.b = in.v;
		return 0xff;
	}
	hh = in.h;
	if (hh >= 360.0) hh = 0.0;
	hh /= 60.0;
	i = (long)hh;
	ff = hh - i;
	p = in.v * (1.0 - in.s);
	q = in.v * (1.0 - (in.s * ff));
	t = in.v * (1.0 - (in.s * (1.0 - ff)));

	switch (i) {
	case 0:
		out.r = in.v;
		out.g = t;
		out.b = p;
		break;
	case 1:
		out.r = q;
		out.g = in.v;
		out.b = p;
		break;
	case 2:
		out.r = p;
		out.g = in.v;
		out.b = t;
		break;

	case 3:
		out.r = p;
		out.g = q;
		out.b = in.v;
		break;
	case 4:
		out.r = t;
		out.g = p;
		out.b = in.v;
		break;
	case 5:
	default:
		out.r = in.v;
		out.g = p;
		out.b = q;
		break;
	}

	if (out.r < 255) {
		out.r = out.r * 255;
		if (out.r > 255) {
			out.r = 255;
		}
	}

	if (out.g < 255) {
		out.g = out.g * 255;
		if (out.g > 255) {
			out.g = 255;
		}
	}

	if (out.b < 255) {
		out.b = out.b * 255;
		if (out.b > 255) {
			out.b = 255;
		}
	}

	return ((int)(out.r) << 16) + ((int)(out.g) << 8) + (int)out.b;
}

int _tmain(int argc, _TCHAR* argv[])
{
	//FreeConsole();

	printf("Run this as an administrator!\n");
	//==============================

	HMODULE hLib = nullptr;

	hLib = LoadLibraryA("AURA_SDK.dll");

	if (hLib == nullptr) {
		printf(" - ERROR!\n");
		return 1;
	}

	(FARPROC&)EnumerateMbController = GetProcAddress(hLib, "EnumerateMbController");
	(FARPROC&)SetMbMode = GetProcAddress(hLib, "SetMbMode");
	(FARPROC&)SetMbColor = GetProcAddress(hLib, "SetMbColor");
	(FARPROC&)GetMbColor = GetProcAddress(hLib, "GetMbColor");
	(FARPROC&)GetMbLedCount = GetProcAddress(hLib, "GetMbLedCount");

	(FARPROC&)EnumerateDram = GetProcAddress(hLib, "EnumerateDram");
	(FARPROC&)SetDramMode = GetProcAddress(hLib, "SetDramMode");
	(FARPROC&)SetDramColor = GetProcAddress(hLib, "SetDramColor");
	(FARPROC&)GetDramColor = GetProcAddress(hLib, "GetDramColor");
	(FARPROC&)GetDramLedCount = GetProcAddress(hLib, "GetDramLedCount");

	int rgb = 0xff00;
	int deg = 0;

	DWORD _count = EnumerateMbController(NULL, 0);

	MbLightControl* _mbLightCtrl = new MbLightControl[_count];
	EnumerateMbController(_mbLightCtrl, _count);

	SetMbMode(_mbLightCtrl[0], 1);

	DWORD _ledCount = GetMbLedCount(_mbLightCtrl[0]);

	BYTE* color = new BYTE[_ledCount * 3];
	ZeroMemory(color, _ledCount * 3);
	
	/*
	//ram
	DWORD _countr = EnumerateMbController(NULL, 0);
	DramLightControl* _dramLightCtrl = new DramLightControl[_countr];

	EnumerateDram(_dramLightCtrl, _countr);

	SetDramMode(_mbLightCtrl[0], 1);

	DWORD _ledCountr = GetDramLedCount(_dramLightCtrl[0]);

	BYTE* colorr = new BYTE[_ledCountr * 3];
	ZeroMemory(color, _ledCountr * 3);
	*/
	while (true) {
		printf("UPDATING LIGHTS");

		// MB

		for (int j = 0; j < 3; j++) {
			for (int t = 0; t < 125; t++) {
				for (size_t i = 0; i < _ledCount * 3; ++i)
				{
					// phanteks rgb strip correction for me :)
					//if (i >= (_ledCount * 3) - 4) {
						if (i % 3 == 0) {
							color[i] = (rgb << 16) >> 16;//b
						}
						if (i % 3 == 1) {
							color[i] = (rgb >> 16);//r
						}
						if (i % 3 == 2) {
							color[i] = ((rgb << 8) >> 16);//g
						}
					//}
					//else {
						if (i % 3 == 0) {
							color[i] = (rgb >> 16);//r
						}
						if (i % 3 == 1) {
							color[i] = ((rgb << 8) >> 16);//g
						}
						if (i % 3 == 2) {
							color[i] = (rgb << 16) >> 16;//b
						}
					//}
				}

				SetMbColor(_mbLightCtrl[0], color, _ledCount * 3);
			}
		}

		//ram
		/*
		for (int j = 0; j < 3; j++) {
			for (int t = 0; t < 125; t++) {
				for (size_t i = 0; i < _ledCountr * 3; ++i)
				{
					if (i % 3 == 0) {
						colorr[i] = (rgb >> 16);//r
					}
					if (i % 3 == 1) {
						colorr[i] = ((rgb << 8) >> 16);//g
					}
					if (i % 3 == 2) {
						colorr[i] = (rgb << 16) >> 16;//b
					}
				}

				SetDramColor(_dramLightCtrl[0], colorr, _ledCountr * 3);
			}
		}*/

		deg += 1;

		if (deg >= 360) {
			deg = 0;
		}

		rgb = hsv2rgb(deg, 1.0, 1.0);

		printf(" - UPDATED LIGHTS to ");
		std::cout << rgb;
		printf("\n");

		Sleep(50);
	}

	FreeLibrary(hLib);

	//==============================

	return 0;
}
