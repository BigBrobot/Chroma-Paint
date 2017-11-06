
#include "Template.h";
#include <fstream>
#include <string>
#include <Windows.h>
#pragma once
#include "include.h"
#define _CHROMASDKIMPL_H_
#ifdef _WIN64
#define CHROMASDKDLL        _T("RzChromaSDK64.dll")
#else
#define CHROMASDKDLL        _T("RzChromaSDK.dll")
#endif
using namespace ChromaSDK;
using namespace ChromaSDK::Keyboard;
using namespace ChromaSDK::Keypad;
using namespace ChromaSDK::Mouse;
using namespace ChromaSDK::Mousepad;
using namespace ChromaSDK::Headset;
using namespace std;

typedef RZRESULT(*INIT)(void);
typedef RZRESULT(*UNINIT)(void);
typedef RZRESULT(*CREATEEFFECT)(RZDEVICEID DeviceId, ChromaSDK::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CREATEKEYBOARDEFFECT)(ChromaSDK::Keyboard::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CREATEHEADSETEFFECT)(ChromaSDK::Headset::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CREATEMOUSEPADEFFECT)(ChromaSDK::Mousepad::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CREATEMOUSEEFFECT)(ChromaSDK::Mouse::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CREATEKEYPADEFFECT)(ChromaSDK::Keypad::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*SETEFFECT)(RZEFFECTID EffectId);
typedef RZRESULT(*DELETEEFFECT)(RZEFFECTID EffectId);
typedef RZRESULT(*REGISTEREVENTNOTIFICATION)(HWND hWnd);
typedef RZRESULT(*UNREGISTEREVENTNOTIFICATION)(void);
typedef RZRESULT(*QUERYDEVICE)(RZDEVICEID DeviceId, ChromaSDK::DEVICE_INFO_TYPE &DeviceInfo);
char szClassName[] = "Windows App";

WNDCLASSEX wincl;
const HWND window2 = GetForegroundWindow();
HWND hwnd = CreateWindowEx(0, "WindowsApp", "Chroma Paint", WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, HWND_DESKTOP, NULL, NULL, NULL);
HWND test = GetForegroundWindow();
COLORREF canvas[48] = {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK};
COLORREF clear[48] = { BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK };
int red[144];
int green[144];
int blue[144];
string sred[48];
string sgreen[48];
string sblue[48];
COLORREF numpad1[4] = {pad11,pad12,pad13,pad14};
COLORREF numpad2[4] = {RED, ORANGE, YELLOW, GREEN};
COLORREF numpad3[4] = {ORANGE, YELLOW, GREEN, CYAN};
COLORREF numpad4[4] = {PURPLE, PINK, RED, ORANGE};
COLORREF numpad5[4] = {WHITE, WHITE, WHITE, WHITE};
COLORREF numpad6[4] = {YELLOW, GREEN, CYAN, BLUE};
COLORREF numpad7[4] = {BLUE, PURPLE, PINK, RED};
COLORREF numpad8[4] = {CYAN, BLUE, PURPLE, PINK};
COLORREF numpad9[4] = {GREEN, CYAN, BLUE, PURPLE};
COLORREF placeholder[9] = {numpad1[0],numpad2[0],numpad3[0],numpad4[0],numpad5[0],numpad6[0],numpad7[0],numpad8[0],numpad9[0]};
int r = 0;
int g = 0;
int b = 0;
char c;
std::string str;
COLORREF sample = (r, g, b);
char place[48];
BOOL active = false;
string commaholder;
int menu = 0;
int row;
int col;
int set=1;
INIT Init = nullptr;
UNINIT UnInit = nullptr;
CREATEEFFECT CreateEffect = nullptr;
CREATEKEYBOARDEFFECT CreateKeyboardEffect = nullptr;
CREATEMOUSEEFFECT CreateMouseEffect = nullptr;
CREATEHEADSETEFFECT CreateHeadsetEffect = nullptr;
CREATEMOUSEPADEFFECT CreateMousepadEffect = nullptr;
CREATEKEYPADEFFECT CreateKeypadEffect = nullptr;
SETEFFECT SetEffect = nullptr;
DELETEEFFECT DeleteEffect = nullptr;
QUERYDEVICE QueryDevice = nullptr;


BOOL My_Chroma_Implementation::IsDeviceConnected(RZDEVICEID DeviceId)
{
	if (QueryDevice != nullptr)
	{
		ChromaSDK::DEVICE_INFO_TYPE DeviceInfo = {};
		auto Result = QueryDevice(DeviceId, DeviceInfo);

		return DeviceInfo.Connected;
	}

	return FALSE;
}


My_Chroma_Implementation::My_Chroma_Implementation() :m_ChromaSDKModule(nullptr)
{
}
My_Chroma_Implementation::~My_Chroma_Implementation()
{
}

BOOL My_Chroma_Implementation::Initialize()
{
	if (m_ChromaSDKModule == nullptr)
	{
		m_ChromaSDKModule = LoadLibrary(CHROMASDKDLL);
		if (m_ChromaSDKModule == nullptr)
		{
			return FALSE;
		}
	}

	if (Init == nullptr)
	{
		auto Result = RZRESULT_INVALID;
		Init = reinterpret_cast<INIT>(GetProcAddress(m_ChromaSDKModule, "Init"));
		if (Init)
		{
			Result = Init();
			if (Result == RZRESULT_SUCCESS)
			{
				CreateEffect = reinterpret_cast<CREATEEFFECT>(GetProcAddress(m_ChromaSDKModule, "CreateEffect"));
				CreateKeyboardEffect = reinterpret_cast<CREATEKEYBOARDEFFECT>(GetProcAddress(m_ChromaSDKModule, "CreateKeyboardEffect"));
				CreateMouseEffect = reinterpret_cast<CREATEMOUSEEFFECT>(GetProcAddress(m_ChromaSDKModule, "CreateMouseEffect"));
				CreateHeadsetEffect = reinterpret_cast<CREATEHEADSETEFFECT>(GetProcAddress(m_ChromaSDKModule, "CreateHeadsetEffect"));
				CreateMousepadEffect = reinterpret_cast<CREATEMOUSEPADEFFECT>(GetProcAddress(m_ChromaSDKModule, "CreateMousepadEffect"));
				CreateKeypadEffect = reinterpret_cast<CREATEKEYPADEFFECT>(GetProcAddress(m_ChromaSDKModule, "CreateKeypadEffect"));
				SetEffect = reinterpret_cast<SETEFFECT>(GetProcAddress(m_ChromaSDKModule, "SetEffect"));
				DeleteEffect = reinterpret_cast<DELETEEFFECT>(GetProcAddress(m_ChromaSDKModule, "DeleteEffect"));
				QueryDevice = reinterpret_cast<QUERYDEVICE>(GetProcAddress(m_ChromaSDKModule, "QueryDevice"));

				if (CreateEffect &&
					CreateKeyboardEffect &&
					CreateMouseEffect &&
					CreateHeadsetEffect &&
					CreateMousepadEffect &&
					CreateKeypadEffect &&
					SetEffect &&
					DeleteEffect &&
					QueryDevice)
				{
					return TRUE;
				}
				else
				{
					return FALSE;
				}
			}
		}
	}

	return TRUE;
}

void My_Chroma_Implementation::ResetEffects(size_t DeviceType)
{
	switch (DeviceType)
	{
	case 0:
		if (CreateKeyboardEffect)
		{
			CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_NONE, nullptr, nullptr);
		}

		if (CreateMousepadEffect)
		{
			CreateMousepadEffect(ChromaSDK::Mousepad::CHROMA_NONE, nullptr, nullptr);
		}

		if (CreateMouseEffect)
		{
			CreateMouseEffect(ChromaSDK::Mouse::CHROMA_NONE, nullptr, nullptr);
		}

		if (CreateHeadsetEffect)
		{
			CreateHeadsetEffect(ChromaSDK::Headset::CHROMA_NONE, nullptr, nullptr);
		}

		if (CreateKeypadEffect)
		{
			CreateKeypadEffect(ChromaSDK::Keypad::CHROMA_NONE, nullptr, nullptr);
		}
		break;
	case 1:
		if (CreateKeyboardEffect)
		{
			CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_NONE, nullptr, nullptr);
		}
		break;
	case 2:
		if (CreateMousepadEffect)
		{
			CreateMousepadEffect(ChromaSDK::Mousepad::CHROMA_NONE, nullptr, nullptr);
		}
		break;
	case 3:
		if (CreateMouseEffect)
		{
			CreateMouseEffect(ChromaSDK::Mouse::CHROMA_NONE, nullptr, nullptr);
		}
		break;
	case 4:
		if (CreateHeadsetEffect)
		{
			CreateHeadsetEffect(ChromaSDK::Headset::CHROMA_NONE, nullptr, nullptr);
		}
		break;
	case 5:
		if (CreateKeypadEffect)
		{
			CreateKeypadEffect(ChromaSDK::Keypad::CHROMA_NONE, nullptr, nullptr);
		}
		break;
	}
}
//ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE Example_keyboard_effect_key = {};
//----
//All the code above is provided by the developer example, you can try to understand it or just use it



//Let's start with a Keyboard effect








Keyboard::CUSTOM_KEY_EFFECT_TYPE Example_keyboard_effect_key = {};

	//Keyboard::CUSTOM_KEY_EFFECT_TYPE Example_keyboard_effect_key = {};
BOOL My_Chroma_Implementation::example_keyboard() {

	//Choose one of the following methods by just deleting the first and last line :)

	//Result_Keyboard = CreateKeyboardEffect(Keyboard::CHROMA_WAVE, &Example_keyboard_effect_key, NULL);



	//The keyboard effect is initialized as a 2 dimensional matrix/array
	//e.g. the ESC-key is [0][1]
	// Source: http://developer.razerzone.com/chroma/razer-chroma-led-profiles/
	// Take the super keyboard as standard, so your programm will work with every keyboard out of the box 


	//Now we apply the effect to the keyboard

	//You can work with the Result as well, e.g. checking if everythings ok

	//RZRESULT Result_Keyboard = CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_CUSTOM, &Example_keyboard_effect, nullptr);


 //Initialize key-based Effect

	//The keyboard key effect is initialized as a 2 dimensional matrix/array, but this time called by the HIBYTE and the LOBYTE of the key itself
	//e.g. the ESC-key is [HIBYTE(RZKEY_ESC)][LOBYTE(RZKEY_ESC)]
	//Use this if you want to light up specific keys
	//The whole naming scheme can be found in Keyboard::RZKEY of the RzChromaSDKTypes.h
	//Important! Don't forget the 0x01000000 | before the used COLORREF, otherwise it won't work

		//Example_keyboard_effect_key.Key[HIBYTE(RZKEY_1)][LOBYTE(RZKEY_1)] = 0x01000000 | RED; // 1-key will light up red

	//CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_WAVE, &Effect, NULL);
	//CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_WAVE, &Effect, NULL);
	//CreateKeyboardEffect(Keyboard::CHROMA_WAVE, &Effect, NULL);
	//CreateKeyboardEffect(Keyboard::CHROMA_WAVE, &Effect, NULL);

	HWND Window = GetForegroundWindow();
	if (window2 == Window) {
		//RZRESULT Result_Keyboard = CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_CUSTOM, &Example_keyboard_effect, nullptr);
		if (GetKeyState(101) < 0)
		{
			if (GetKeyState(VK_LMENU) < 0) {
				numpad5[menu] = RGB(r, g, b);
			}
			else {
				colour = numpad5[menu];
			}
		}
		if (GetKeyState(98) < 0)
		{
			if (GetKeyState(VK_LMENU) < 0) {
				numpad2[menu] = RGB(r, g, b);
			}
			else {
				colour = numpad2[menu];
			}
		}
		if (GetKeyState(100) < 0)
		{
			if (GetKeyState(VK_LMENU) < 0) {
				numpad4[menu] = RGB(r, g, b);
			}
			else {
				colour = numpad4[menu];
			}
		}
		if (GetKeyState(102) < 0)
		{
			if (GetKeyState(VK_LMENU) < 0) {
				numpad6[menu] = RGB(r, g, b);
			}
			else {
				colour = numpad6[menu];
			}
		}
		if (GetKeyState(99) < 0)
		{
			if (GetKeyState(VK_LMENU) < 0) {
				numpad3[menu] = RGB(r, g, b);
			}
			else {
				colour = numpad3[menu];
			}
		}
		if (GetKeyState(97) < 0)
		{
			if (GetKeyState(VK_LMENU) < 0) {
				numpad1[menu] = RGB(r, g, b);
			}
			else {
				colour = numpad1[menu];
			}
		}
		if (GetKeyState(105) < 0)
		{
			if (GetKeyState(VK_LMENU) < 0) {
				numpad9[menu] = RGB(r, g, b);
			}
			else {
				colour = numpad9[menu];
			}
		}
		if (GetKeyState(103) < 0)
		{
			if (GetKeyState(VK_LMENU) < 0) {
				numpad7[menu] = RGB(r, g, b);
			}
			else {
				colour = numpad7[menu];
			}
		}
		if (GetKeyState(104) < 0)
		{
			if (GetKeyState(VK_LMENU) < 0) {
				numpad8[menu] = RGB(r, g, b);
			}
			else {
				colour = numpad8[menu];
			}
		}
		if (GetKeyState(96) < 0)
		{
			colour = BLACK;
		}
		if (GetKeyState(0x41) & 0x8000)
		{
			canvas[1] = colour;
		}
		if (GetKeyState('B') < 0)
		{
			canvas[2] = colour;
		}
		if (GetKeyState('C') < 0)
		{
			canvas[3] = colour;
		}
		if (GetKeyState('D') < 0)
		{
			canvas[4] = colour;
		}
		if (GetKeyState('E') < 0)
		{
			canvas[5] = colour;
		}
		if (GetKeyState('F') < 0)
		{
			canvas[6] = colour;
		}
		if (GetKeyState('G') < 0)
		{
			canvas[7] = colour;
		}
		if (GetKeyState('H') < 0)
		{
			canvas[8] = colour;
		}
		if (GetKeyState('I') < 0)
		{
			canvas[9] = colour;
		}
		if (GetKeyState('J') < 0)
		{
			canvas[10] = colour;
		}
		if (GetKeyState('K') < 0)
		{
			canvas[11] = colour;
		}
		if (GetKeyState('L') < 0)
		{
			canvas[12] = colour;
		}
		if (GetKeyState('M') < 0)
		{
			canvas[13] = colour;
		}
		if (GetKeyState('N') < 0)
		{
			canvas[14] = colour;
		}
		if (GetKeyState('O') < 0)
		{
			canvas[15] = colour;
		}
		if (GetKeyState('P') < 0)
		{
			canvas[16] = colour;
		}
		if (GetKeyState('Q') < 0)
		{
			canvas[17] = colour;
		}
		if (GetKeyState('R') < 0)
		{
			canvas[18] = colour;
		}
		if (GetKeyState('S') < 0)
		{
			canvas[19] = colour;
		}
		if (GetKeyState('T') < 0)
		{
			canvas[20] = colour;
		}
		if (GetKeyState('U') < 0)
		{
			canvas[21] = colour;
		}
		if (GetKeyState('V') < 0)
		{
			canvas[22] = colour;
		}
		if (GetKeyState('W') < 0)
		{
			canvas[23] = colour;
		}
		if (GetKeyState('X') < 0)
		{
			canvas[24] = colour;
		}
		if (GetKeyState('Y') < 0)
		{
			canvas[25] = colour;
		}
		if (GetKeyState('Z') < 0)
		{
			canvas[26] = colour;
		}
		if (GetKeyState('1') < 0)
		{
			canvas[27] = colour;
		}
		if (GetKeyState('2') < 0)
		{
			canvas[28] = colour;
		}
		if (GetKeyState('3') < 0)
		{
			canvas[29] = colour;
		}
		if (GetKeyState('4') < 0)
		{
			canvas[30] = colour;
		}
		if (GetKeyState('5') < 0)
		{
			canvas[31] = colour;
		}
		if (GetKeyState('6') < 0)
		{
			canvas[32] = colour;
		}
		if (GetKeyState('7') < 0)
		{
			canvas[33] = colour;
		}
		if (GetKeyState('8') < 0)
		{
			canvas[34] = colour;
		}
		if (GetKeyState('9') < 0)
		{
			canvas[35] = colour;
		}
		if (GetKeyState('0') < 0)
		{
			canvas[36] = colour;
		}
		if (GetKeyState(VK_OEM_4) < 0)
		{
			canvas[37] = colour;
		}
		if (GetKeyState(VK_OEM_6) < 0)
		{
			canvas[38] = colour;
		}
		if (GetKeyState(VK_OEM_MINUS) < 0)
		{
			canvas[39] = colour;
		}
		if (GetKeyState(VK_OEM_PLUS) < 0)
		{
			canvas[40] = colour;
		}
		if (GetKeyState(VK_OEM_1) < 0)
		{
			canvas[41] = colour;
		}
		if (GetKeyState(VK_OEM_3) < 0)
		{
			canvas[42] = colour;
		}
		if (GetKeyState(VK_OEM_7) < 0)
		{
			canvas[43] = colour;
		}
		if (GetKeyState(VK_OEM_5) < 0)
		{
			canvas[44] = colour;
		}
		if (GetKeyState(VK_OEM_COMMA) < 0)
		{
			canvas[45] = colour;
		}
		if (GetKeyState(VK_OEM_PERIOD) < 0)
		{
			canvas[46] = colour;
		}
		if (GetKeyState(VK_OEM_2) < 0)
		{
			canvas[47] = colour;
		}
		if (GetKeyState(VK_F8) < 0)
		{
			colour = NULL;
		}
		if (GetKeyState(VK_INSERT) < 0)
		{
			r = r + 1;
			Sleep(10);
		}
		if (GetKeyState(VK_DELETE) < 0)
		{
			r = r - 1;
			Sleep(10);
		}
		if (r > 255) {
			cout << "\nRed value is at maximum.";
			r = 255;
			Sleep(1000);
		}
		if (r < 0) {
			cout << "\nRed value is at minimum.";
			r = 0;
			Sleep(1000);
		}
		if (GetKeyState(VK_HOME) < 0)
		{
			g = g + 1;
			Sleep(10);
		}
		if (GetKeyState(VK_END) < 0)
		{
			g = g - 1;
			Sleep(10);
		}
		if (g > 255) {
			cout << "\nGreen value is at maximum.";
			g = 255;
			Sleep(1000);
		}
		if (g < 0) {
			cout << "\nGreen value is at minimum.";
			g = 0;
			Sleep(1000);
		}
		if (GetKeyState(VK_PRIOR) < 0)
		{
			b = b + 1;
			Sleep(10);
		}
		if (GetKeyState(VK_NEXT) < 0)
		{
			b = b - 1;
			Sleep(10);
		}
		if (b > 255) {
			cout << "\nBlue value is at maximum.";
			b = 255;
			Sleep(1000);
		}
		if (b < 0) {
			cout << "\nBlue value is at minimum.";
			b = 0;
			Sleep(1000);
		}
		if (GetKeyState(VK_F1) < 0)
		{
			Sleep(500);
			menu = 0;
			placeholder[0] = numpad1[0];
			placeholder[1] = numpad2[0];
			placeholder[2] = numpad3[0];
			placeholder[3] = numpad4[0];
			placeholder[4] = numpad5[0];
			placeholder[5] = numpad6[0];
			placeholder[6] = numpad7[0];
			placeholder[7] = numpad8[0];
			placeholder[8] = numpad9[0];
			for (set; set < 48; set = set + 1) {
				clear[set] = BLACK;
			}
			set = 1;
			numpad1[0] = WHITE;
			numpad2[0] = WHITE;
			numpad3[0] = WHITE;
			numpad4[0] = WHITE;
			numpad5[0] = WHITE;
			numpad6[0] = WHITE;
			numpad7[0] = WHITE;
			numpad8[0] = WHITE;
			numpad9[0] = WHITE;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_F1)][LOBYTE(RZKEY_F1)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_F2)][LOBYTE(RZKEY_F2)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMLOCK)][LOBYTE(RZKEY_NUMLOCK)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD_DIVIDE)][LOBYTE(RZKEY_NUMPAD_DIVIDE)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD_MULTIPLY)][LOBYTE(RZKEY_NUMPAD_MULTIPLY)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD_SUBTRACT)][LOBYTE(RZKEY_NUMPAD_SUBTRACT)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_F5)][LOBYTE(RZKEY_F5)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_F6)][LOBYTE(RZKEY_F6)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_F7)][LOBYTE(RZKEY_F7)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_INSERT)][LOBYTE(RZKEY_INSERT)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_DELETE)][LOBYTE(RZKEY_DELETE)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_HOME)][LOBYTE(RZKEY_HOME)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_END)][LOBYTE(RZKEY_END)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_PAGEUP)][LOBYTE(RZKEY_PAGEUP)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_PAGEDOWN)][LOBYTE(RZKEY_PAGEDOWN)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_A)][LOBYTE(RZKEY_A)] = 0x01000000 | clear[1];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_B)][LOBYTE(RZKEY_B)] = 0x01000000 | clear[2];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_C)][LOBYTE(RZKEY_C)] = 0x01000000 | clear[3];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_D)][LOBYTE(RZKEY_D)] = 0x01000000 | clear[4];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_E)][LOBYTE(RZKEY_E)] = 0x01000000 | clear[5];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_F)][LOBYTE(RZKEY_F)] = 0x01000000 | clear[6];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_G)][LOBYTE(RZKEY_G)] = 0x01000000 | clear[7];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_H)][LOBYTE(RZKEY_H)] = 0x01000000 | clear[8];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_I)][LOBYTE(RZKEY_I)] = 0x01000000 | clear[9];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_J)][LOBYTE(RZKEY_J)] = 0x01000000 | clear[10];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_K)][LOBYTE(RZKEY_K)] = 0x01000000 | clear[11];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_L)][LOBYTE(RZKEY_L)] = 0x01000000 | clear[12];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_M)][LOBYTE(RZKEY_M)] = 0x01000000 | clear[13];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_N)][LOBYTE(RZKEY_N)] = 0x01000000 | clear[14];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_O)][LOBYTE(RZKEY_O)] = 0x01000000 | clear[15];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_P)][LOBYTE(RZKEY_P)] = 0x01000000 | clear[16];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_Q)][LOBYTE(RZKEY_Q)] = 0x01000000 | clear[17];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_R)][LOBYTE(RZKEY_R)] = 0x01000000 | clear[18];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_S)][LOBYTE(RZKEY_S)] = 0x01000000 | clear[19];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_T)][LOBYTE(RZKEY_T)] = 0x01000000 | clear[20];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_U)][LOBYTE(RZKEY_U)] = 0x01000000 | clear[21];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_V)][LOBYTE(RZKEY_V)] = 0x01000000 | clear[22];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_W)][LOBYTE(RZKEY_W)] = 0x01000000 | clear[23];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_X)][LOBYTE(RZKEY_X)] = 0x01000000 | clear[24];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_Y)][LOBYTE(RZKEY_Y)] = 0x01000000 | clear[25];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_Z)][LOBYTE(RZKEY_Z)] = 0x01000000 | clear[26];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_1)][LOBYTE(RZKEY_1)] = 0x01000000 | clear[27];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_2)][LOBYTE(RZKEY_2)] = 0x01000000 | clear[28];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_3)][LOBYTE(RZKEY_3)] = 0x01000000 | clear[29];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_4)][LOBYTE(RZKEY_4)] = 0x01000000 | clear[30];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_5)][LOBYTE(RZKEY_5)] = 0x01000000 | clear[31];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_6)][LOBYTE(RZKEY_6)] = 0x01000000 | clear[32];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_7)][LOBYTE(RZKEY_7)] = 0x01000000 | clear[33];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_8)][LOBYTE(RZKEY_8)] = 0x01000000 | clear[34];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_9)][LOBYTE(RZKEY_9)] = 0x01000000 | clear[35];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_0)][LOBYTE(RZKEY_0)] = 0x01000000 | clear[36];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_OEM_4)][LOBYTE(RZKEY_OEM_4)] = 0x01000000 | clear[37];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_OEM_5)][LOBYTE(RZKEY_OEM_5)] = 0x01000000 | clear[38];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_OEM_2)][LOBYTE(RZKEY_OEM_2)] = 0x01000000 | clear[39];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_OEM_3)][LOBYTE(RZKEY_OEM_3)] = 0x01000000 | clear[40];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_OEM_7)][LOBYTE(RZKEY_OEM_7)] = 0x01000000 | clear[41];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_OEM_8)][LOBYTE(RZKEY_OEM_8)] = 0x01000000 | clear[42];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_EUR_1)][LOBYTE(RZKEY_EUR_1)] = 0x01000000 | clear[43];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_EUR_2)][LOBYTE(RZKEY_EUR_2)] = 0x01000000 | clear[44];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_OEM_9)][LOBYTE(RZKEY_OEM_9)] = 0x01000000 | clear[45];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_OEM_10)][LOBYTE(RZKEY_OEM_10)] = 0x01000000 | clear[46];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_OEM_11)][LOBYTE(RZKEY_OEM_11)] = 0x01000000 | clear[47];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD2)][LOBYTE(RZKEY_NUMPAD2)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD9)][LOBYTE(RZKEY_NUMPAD9)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD7)][LOBYTE(RZKEY_NUMPAD7)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD5)][LOBYTE(RZKEY_NUMPAD5)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD8)][LOBYTE(RZKEY_NUMPAD8)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD6)][LOBYTE(RZKEY_NUMPAD6)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD3)][LOBYTE(RZKEY_NUMPAD3)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD1)][LOBYTE(RZKEY_NUMPAD1)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD4)][LOBYTE(RZKEY_NUMPAD4)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(0x0014)][LOBYTE(0x0014)] = 0x01000000 | BLACK;
			cout << "\n\nFor your assistance, the location of the relevant keys are displayed in white on\nthe keyboard when being described in this help guide.";
			RZRESULT Result_Keyboard = CreateKeyboardEffect(Keyboard::CHROMA_CUSTOM_KEY, &Example_keyboard_effect_key, NULL);
			Sleep(7000);
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD2)][LOBYTE(RZKEY_NUMPAD2)] = 0x01000000 | numpad2[menu];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD9)][LOBYTE(RZKEY_NUMPAD9)] = 0x01000000 | numpad9[menu];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD7)][LOBYTE(RZKEY_NUMPAD7)] = 0x01000000 | numpad7[menu];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD5)][LOBYTE(RZKEY_NUMPAD5)] = 0x01000000 | numpad5[menu];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD8)][LOBYTE(RZKEY_NUMPAD8)] = 0x01000000 | numpad8[menu];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD6)][LOBYTE(RZKEY_NUMPAD6)] = 0x01000000 | numpad6[menu];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD3)][LOBYTE(RZKEY_NUMPAD3)] = 0x01000000 | numpad3[menu];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD1)][LOBYTE(RZKEY_NUMPAD1)] = 0x01000000 | numpad1[menu];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD4)][LOBYTE(RZKEY_NUMPAD4)] = 0x01000000 | numpad4[menu];
			Result_Keyboard = CreateKeyboardEffect(Keyboard::CHROMA_CUSTOM_KEY, &Example_keyboard_effect_key, NULL);
			cout << "\n\nThe numberpad to the right of the main keyboard acts as a palette.\nSelecting a colour on the numberpad will determine the paint colour.";
			numpad1[0] = placeholder[0];
			numpad2[0] = placeholder[1];
			numpad3[0] = placeholder[2];
			numpad4[0] = placeholder[3];
			numpad5[0] = placeholder[4];
			numpad6[0] = placeholder[5];
			numpad7[0] = placeholder[6];
			numpad8[0] = placeholder[7];
			numpad9[0] = placeholder[8];
			Sleep(7000);
			cout << "\n\nThe number zero on the keypad is used as an eraser tool and will erase any colour on the canvas.";
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD2)][LOBYTE(RZKEY_NUMPAD2)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD9)][LOBYTE(RZKEY_NUMPAD9)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD7)][LOBYTE(RZKEY_NUMPAD7)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD5)][LOBYTE(RZKEY_NUMPAD5)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD8)][LOBYTE(RZKEY_NUMPAD8)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD6)][LOBYTE(RZKEY_NUMPAD6)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD3)][LOBYTE(RZKEY_NUMPAD3)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD1)][LOBYTE(RZKEY_NUMPAD1)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD4)][LOBYTE(RZKEY_NUMPAD4)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD0)][LOBYTE(RZKEY_NUMPAD0)] = 0x01000000 | WHITE;
			Result_Keyboard = CreateKeyboardEffect(Keyboard::CHROMA_CUSTOM_KEY, &Example_keyboard_effect_key, NULL);

			Sleep(7000);
			cout << "\n\nThe '+' addition key to the right of the numberpad enables you to cycle through the 36 available colour slots.";
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD0)][LOBYTE(RZKEY_NUMPAD0)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD_ADD)][LOBYTE(RZKEY_NUMPAD_ADD)] = 0x01000000 | WHITE;
			Result_Keyboard = CreateKeyboardEffect(Keyboard::CHROMA_CUSTOM_KEY, &Example_keyboard_effect_key, NULL);
			Sleep(7000);
			cout << "\n\nTo the left of the numberpad are the red, green, and blue colour filters which enable you to alter the concentration of red, green and blue in the colour.";
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_INSERT)][LOBYTE(RZKEY_INSERT)] = 0x01000000 | WHITE;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_DELETE)][LOBYTE(RZKEY_DELETE)] = 0x01000000 | WHITE;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_HOME)][LOBYTE(RZKEY_HOME)] = 0x01000000 | WHITE;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_END)][LOBYTE(RZKEY_END)] = 0x01000000 | WHITE;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_PAGEUP)][LOBYTE(RZKEY_PAGEUP)] = 0x01000000 | WHITE;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_PAGEDOWN)][LOBYTE(RZKEY_PAGEDOWN)] = 0x01000000 | WHITE;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD_ADD)][LOBYTE(RZKEY_NUMPAD_ADD)] = 0x01000000 | BLACK;
			Result_Keyboard = CreateKeyboardEffect(Keyboard::CHROMA_CUSTOM_KEY, &Example_keyboard_effect_key, NULL);
			Sleep(7000);
			cout << "\nThe top row increases the concentration, and the bottom row decreases.";
			Sleep(7000);
			cout << "\n\nThe 3 keys above display the resulting colour.";
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_PRINTSCREEN)][LOBYTE(RZKEY_PRINTSCREEN)] = 0x01000000 | WHITE;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_SCROLL)][LOBYTE(RZKEY_SCROLL)] = 0x01000000 | WHITE;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_PAUSE)][LOBYTE(RZKEY_PAUSE)] = 0x01000000 | WHITE;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_INSERT)][LOBYTE(RZKEY_INSERT)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_DELETE)][LOBYTE(RZKEY_DELETE)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_HOME)][LOBYTE(RZKEY_HOME)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_END)][LOBYTE(RZKEY_END)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_PAGEUP)][LOBYTE(RZKEY_PAGEUP)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_PAGEDOWN)][LOBYTE(RZKEY_PAGEDOWN)] = 0x01000000 | BLACK;
			Result_Keyboard = CreateKeyboardEffect(Keyboard::CHROMA_CUSTOM_KEY, &Example_keyboard_effect_key, NULL);
			Sleep(7000);
			cout << "\n\nPressing alt and a key on the numberpad simultaneously will assign and display the colour you've just created onto that key.";
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_PRINTSCREEN)][LOBYTE(RZKEY_PRINTSCREEN)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_SCROLL)][LOBYTE(RZKEY_SCROLL)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_PAUSE)][LOBYTE(RZKEY_PAUSE)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_LALT)][LOBYTE(RZKEY_LALT)] = 0x01000000 | WHITE;
			Result_Keyboard = CreateKeyboardEffect(Keyboard::CHROMA_CUSTOM_KEY, &Example_keyboard_effect_key, NULL);
			Sleep(10000);
			cout << "\n\nThe F5 key is the paint bucket tool which fills the main canvas with the selected colour.";
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_LALT)][LOBYTE(RZKEY_LALT)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_F5)][LOBYTE(RZKEY_F5)] = 0x01000000 | WHITE;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_PRINTSCREEN)][LOBYTE(RZKEY_PRINTSCREEN)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_SCROLL)][LOBYTE(RZKEY_SCROLL)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_PAUSE)][LOBYTE(RZKEY_PAUSE)] = 0x01000000 | BLACK;
			Result_Keyboard = CreateKeyboardEffect(Keyboard::CHROMA_CUSTOM_KEY, &Example_keyboard_effect_key, NULL);
			Sleep(14000);
			cout << "\n\nThe F6 key saves the drawn profile to the location of the application, its filename is 'Profile_Save.txt'.";
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_F5)][LOBYTE(RZKEY_F5)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_F6)][LOBYTE(RZKEY_F6)] = 0x01000000 | WHITE;
			Result_Keyboard = CreateKeyboardEffect(Keyboard::CHROMA_CUSTOM_KEY, &Example_keyboard_effect_key, NULL);
			Sleep(7000);
			cout << "\n\nThe F7 key loads the profile that is saved in the text file. If you want to load someone elses profile, make sure that\nit is named 'Profile_Save.txt' and that no other files have the same name.";
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_F6)][LOBYTE(RZKEY_F6)] = 0x01000000 | BLACK;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_F7)][LOBYTE(RZKEY_F7)] = 0x01000000 | WHITE;
			Result_Keyboard = CreateKeyboardEffect(Keyboard::CHROMA_CUSTOM_KEY, &Example_keyboard_effect_key, NULL);
			Sleep(10000);
			cout << "\n\nIt is reccomended to have a folder where you can move the files to and from.";
			Sleep(5000);
			cout << "\n\nThe canvas is the main area of the keyboard where you can design your own profiles.";
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_F7)][LOBYTE(RZKEY_F7)] = 0x01000000 | BLACK;
			for (set; set < 48; set = set + 1) {
				clear[set] = WHITE;
			}
			set = 1;
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_A)][LOBYTE(RZKEY_A)] = 0x01000000 | clear[1];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_B)][LOBYTE(RZKEY_B)] = 0x01000000 | clear[2];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_C)][LOBYTE(RZKEY_C)] = 0x01000000 | clear[3];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_D)][LOBYTE(RZKEY_D)] = 0x01000000 | clear[4];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_E)][LOBYTE(RZKEY_E)] = 0x01000000 | clear[5];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_F)][LOBYTE(RZKEY_F)] = 0x01000000 | clear[6];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_G)][LOBYTE(RZKEY_G)] = 0x01000000 | clear[7];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_H)][LOBYTE(RZKEY_H)] = 0x01000000 | clear[8];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_I)][LOBYTE(RZKEY_I)] = 0x01000000 | clear[9];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_J)][LOBYTE(RZKEY_J)] = 0x01000000 | clear[10];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_K)][LOBYTE(RZKEY_K)] = 0x01000000 | clear[11];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_L)][LOBYTE(RZKEY_L)] = 0x01000000 | clear[12];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_M)][LOBYTE(RZKEY_M)] = 0x01000000 | clear[13];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_N)][LOBYTE(RZKEY_N)] = 0x01000000 | clear[14];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_O)][LOBYTE(RZKEY_O)] = 0x01000000 | clear[15];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_P)][LOBYTE(RZKEY_P)] = 0x01000000 | clear[16];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_Q)][LOBYTE(RZKEY_Q)] = 0x01000000 | clear[17];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_R)][LOBYTE(RZKEY_R)] = 0x01000000 | clear[18];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_S)][LOBYTE(RZKEY_S)] = 0x01000000 | clear[19];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_T)][LOBYTE(RZKEY_T)] = 0x01000000 | clear[20];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_U)][LOBYTE(RZKEY_U)] = 0x01000000 | clear[21];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_V)][LOBYTE(RZKEY_V)] = 0x01000000 | clear[22];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_W)][LOBYTE(RZKEY_W)] = 0x01000000 | clear[23];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_X)][LOBYTE(RZKEY_X)] = 0x01000000 | clear[24];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_Y)][LOBYTE(RZKEY_Y)] = 0x01000000 | clear[25];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_Z)][LOBYTE(RZKEY_Z)] = 0x01000000 | clear[26];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_1)][LOBYTE(RZKEY_1)] = 0x01000000 | clear[27];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_2)][LOBYTE(RZKEY_2)] = 0x01000000 | clear[28];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_3)][LOBYTE(RZKEY_3)] = 0x01000000 | clear[29];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_4)][LOBYTE(RZKEY_4)] = 0x01000000 | clear[30];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_5)][LOBYTE(RZKEY_5)] = 0x01000000 | clear[31];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_6)][LOBYTE(RZKEY_6)] = 0x01000000 | clear[32];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_7)][LOBYTE(RZKEY_7)] = 0x01000000 | clear[33];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_8)][LOBYTE(RZKEY_8)] = 0x01000000 | clear[34];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_9)][LOBYTE(RZKEY_9)] = 0x01000000 | clear[35];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_0)][LOBYTE(RZKEY_0)] = 0x01000000 | clear[36];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_OEM_4)][LOBYTE(RZKEY_OEM_4)] = 0x01000000 | clear[37];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_OEM_5)][LOBYTE(RZKEY_OEM_5)] = 0x01000000 | clear[38];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_OEM_2)][LOBYTE(RZKEY_OEM_2)] = 0x01000000 | clear[39];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_OEM_3)][LOBYTE(RZKEY_OEM_3)] = 0x01000000 | clear[40];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_OEM_7)][LOBYTE(RZKEY_OEM_7)] = 0x01000000 | clear[41];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_OEM_8)][LOBYTE(RZKEY_OEM_8)] = 0x01000000 | clear[42];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_EUR_1)][LOBYTE(RZKEY_EUR_1)] = 0x01000000 | clear[43];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_EUR_2)][LOBYTE(RZKEY_EUR_2)] = 0x01000000 | clear[44];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_OEM_9)][LOBYTE(RZKEY_OEM_9)] = 0x01000000 | clear[45];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_OEM_10)][LOBYTE(RZKEY_OEM_10)] = 0x01000000 | clear[46];
			Example_keyboard_effect_key.Color[HIBYTE(RZKEY_OEM_11)][LOBYTE(RZKEY_OEM_11)] = 0x01000000 | clear[47];
			Result_Keyboard = CreateKeyboardEffect(Keyboard::CHROMA_CUSTOM_KEY, &Example_keyboard_effect_key, NULL);
			Sleep(10000);
			cout << "\nTo start drawing, select a colour from the keypad; notice how the Razer logo changes colour, this shows\nyour selected colour.";
			Sleep(6000);
			cout << "\nYou can transfer the colour to any key that is lit up white.";
			Sleep(5000);
			cout << "\n\nThat's pretty much it for now, if you have any more questions or ideas for the remaining function keys\nyou can message me on discord or contact me on the razer forums. Enjoy!";
		}
		if (GetKeyState(VK_F2) < 0) {
			cout << "\n\n- 16/08/17    : Version 1.0 Released, can still be found at https://github.com/BigBrobot/Chroma-Paint";
			cout << "\n\n- 17/08/17    : Version 1.1 Released, colour picker added, moved the paint bucket tool to F5\n                Introduced changelog and help features.";
			cout << "\n\n- 05/11/2017  : Version 1.2 Released, Introduced the ability to save and load profiles.\n                Fixed the paint tool still painting while using other apps.";
			Sleep(1000);
		}
		if (GetKeyState(VK_F6) < 0) {
			Sleep(1000);
			int writeFile();
			{
				ofstream myfile;
				myfile.open("Profile_Save.txt");
				for (set; set < 48; set = set + 1) {
					myfile << COLORREF(GetRValue(canvas[set]));
					myfile << " ";
					myfile << COLORREF(GetGValue(canvas[set]));
					myfile << " ";
					myfile << COLORREF(GetBValue(canvas[set]));
					myfile << "\n";
				}
				set = 1;
				myfile.close();
			}
			cout << "\nYour Profile has been saved in the same folder as the application.";

		}
		if (GetKeyState(VK_F7) < 0) {
			Sleep(1000);
			string line;
			ifstream myfile("Profile_Save.txt");
			if (myfile.is_open())
			{
				for (set; set < 48; set = set + 1) {
					myfile >> sred[set] >> sgreen[set] >> sblue[set];
				}
				set = 1;
				for (set; set < 48; set = set + 1) {
					red[set] = stoi(sred[set]);
					green[set] = stoi(sgreen[set]);
					blue[set] = stoi(sblue[set]);
					canvas[set] = RGB(red[set], green[set], blue[set]);
				}
				set = 1;
				cout << "\nYour Profile has been loaded";
			}
		}
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_PRINTSCREEN)][LOBYTE(RZKEY_PRINTSCREEN)] = 0x01000000 | RGB(r, g, b);
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_SCROLL)][LOBYTE(RZKEY_SCROLL)] = 0x01000000 | RGB(r, g, b);
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_PAUSE)][LOBYTE(RZKEY_PAUSE)] = 0x01000000 | RGB(r, g, b);
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_B)][LOBYTE(RZKEY_B)] = 0x01000000 | canvas[2];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_C)][LOBYTE(RZKEY_C)] = 0x01000000 | canvas[3];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_D)][LOBYTE(RZKEY_D)] = 0x01000000 | canvas[4];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_E)][LOBYTE(RZKEY_E)] = 0x01000000 | canvas[5];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_F)][LOBYTE(RZKEY_F)] = 0x01000000 | canvas[6];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_G)][LOBYTE(RZKEY_G)] = 0x01000000 | canvas[7];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_H)][LOBYTE(RZKEY_H)] = 0x01000000 | canvas[8];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_I)][LOBYTE(RZKEY_I)] = 0x01000000 | canvas[9];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_J)][LOBYTE(RZKEY_J)] = 0x01000000 | canvas[10];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_K)][LOBYTE(RZKEY_K)] = 0x01000000 | canvas[11];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_L)][LOBYTE(RZKEY_L)] = 0x01000000 | canvas[12];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_M)][LOBYTE(RZKEY_M)] = 0x01000000 | canvas[13];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_N)][LOBYTE(RZKEY_N)] = 0x01000000 | canvas[14];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_O)][LOBYTE(RZKEY_O)] = 0x01000000 | canvas[15];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_P)][LOBYTE(RZKEY_P)] = 0x01000000 | canvas[16];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_Q)][LOBYTE(RZKEY_Q)] = 0x01000000 | canvas[17];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_R)][LOBYTE(RZKEY_R)] = 0x01000000 | canvas[18];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_S)][LOBYTE(RZKEY_S)] = 0x01000000 | canvas[19];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_T)][LOBYTE(RZKEY_T)] = 0x01000000 | canvas[20];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_U)][LOBYTE(RZKEY_U)] = 0x01000000 | canvas[21];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_V)][LOBYTE(RZKEY_V)] = 0x01000000 | canvas[22];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_W)][LOBYTE(RZKEY_W)] = 0x01000000 | canvas[23];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_X)][LOBYTE(RZKEY_X)] = 0x01000000 | canvas[24];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_Y)][LOBYTE(RZKEY_Y)] = 0x01000000 | canvas[25];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_Z)][LOBYTE(RZKEY_Z)] = 0x01000000 | canvas[26];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_1)][LOBYTE(RZKEY_1)] = 0x01000000 | canvas[27];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_2)][LOBYTE(RZKEY_2)] = 0x01000000 | canvas[28];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_3)][LOBYTE(RZKEY_3)] = 0x01000000 | canvas[29];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_4)][LOBYTE(RZKEY_4)] = 0x01000000 | canvas[30];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_5)][LOBYTE(RZKEY_5)] = 0x01000000 | canvas[31];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_6)][LOBYTE(RZKEY_6)] = 0x01000000 | canvas[32];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_7)][LOBYTE(RZKEY_7)] = 0x01000000 | canvas[33];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_8)][LOBYTE(RZKEY_8)] = 0x01000000 | canvas[34];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_9)][LOBYTE(RZKEY_9)] = 0x01000000 | canvas[35];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_0)][LOBYTE(RZKEY_0)] = 0x01000000 | canvas[36];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_OEM_4)][LOBYTE(RZKEY_OEM_4)] = 0x01000000 | canvas[37];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_OEM_5)][LOBYTE(RZKEY_OEM_5)] = 0x01000000 | canvas[38];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_OEM_2)][LOBYTE(RZKEY_OEM_2)] = 0x01000000 | canvas[39];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_OEM_3)][LOBYTE(RZKEY_OEM_3)] = 0x01000000 | canvas[40];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_OEM_7)][LOBYTE(RZKEY_OEM_7)] = 0x01000000 | canvas[41];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_OEM_8)][LOBYTE(RZKEY_OEM_8)] = 0x01000000 | canvas[42];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_EUR_1)][LOBYTE(RZKEY_EUR_1)] = 0x01000000 | canvas[43];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_EUR_2)][LOBYTE(RZKEY_EUR_2)] = 0x01000000 | canvas[44];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_OEM_9)][LOBYTE(RZKEY_OEM_9)] = 0x01000000 | canvas[45];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_OEM_10)][LOBYTE(RZKEY_OEM_10)] = 0x01000000 | canvas[46];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_OEM_11)][LOBYTE(RZKEY_OEM_11)] = 0x01000000 | canvas[47];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD2)][LOBYTE(RZKEY_NUMPAD2)] = 0x01000000 | numpad2[menu];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD9)][LOBYTE(RZKEY_NUMPAD9)] = 0x01000000 | numpad9[menu];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD7)][LOBYTE(RZKEY_NUMPAD7)] = 0x01000000 | numpad7[menu];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD5)][LOBYTE(RZKEY_NUMPAD5)] = 0x01000000 | numpad5[menu];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD8)][LOBYTE(RZKEY_NUMPAD8)] = 0x01000000 | numpad8[menu];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD6)][LOBYTE(RZKEY_NUMPAD6)] = 0x01000000 | numpad6[menu];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD3)][LOBYTE(RZKEY_NUMPAD3)] = 0x01000000 | numpad3[menu];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD1)][LOBYTE(RZKEY_NUMPAD1)] = 0x01000000 | numpad1[menu];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD4)][LOBYTE(RZKEY_NUMPAD4)] = 0x01000000 | numpad4[menu];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMLOCK)][LOBYTE(RZKEY_NUMLOCK)] = 0x01000000 | WHITE;
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD_DIVIDE)][LOBYTE(RZKEY_NUMPAD_DIVIDE)] = 0x01000000 | WHITE;
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD_MULTIPLY)][LOBYTE(RZKEY_NUMPAD_MULTIPLY)] = 0x01000000 | WHITE;
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_NUMPAD_SUBTRACT)][LOBYTE(RZKEY_NUMPAD_SUBTRACT)] = 0x01000000 | WHITE;
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_F5)][LOBYTE(RZKEY_F5)] = 0x01000000 | WHITE;
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_F6)][LOBYTE(RZKEY_F6)] = 0x01000000 | WHITE;
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_F7)][LOBYTE(RZKEY_F7)] = 0x01000000 | WHITE;
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_A)][LOBYTE(RZKEY_A)] = 0x01000000 | canvas[1];
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_INSERT)][LOBYTE(RZKEY_INSERT)] = 0x01000000 | RED;
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_DELETE)][LOBYTE(RZKEY_DELETE)] = 0x01000000 | RED;
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_HOME)][LOBYTE(RZKEY_HOME)] = 0x01000000 | GREEN;
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_END)][LOBYTE(RZKEY_END)] = 0x01000000 | GREEN;
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_PAGEUP)][LOBYTE(RZKEY_PAGEUP)] = 0x01000000 | BLUE;
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_PAGEDOWN)][LOBYTE(RZKEY_PAGEDOWN)] = 0x01000000 | BLUE;
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_F1)][LOBYTE(RZKEY_F1)] = 0x01000000 | WHITE;
		Example_keyboard_effect_key.Color[HIBYTE(RZKEY_F2)][LOBYTE(RZKEY_F2)] = 0x01000000 | WHITE;

		if (GetKeyState(VK_F5) < 0) {
			for (set; set < 48; set = set + 1) {
				canvas[set] = colour;
			}

		}
		set = 1;
		if (GetKeyState(VK_DIVIDE) < 0) {
			menu = 1;
		}
		else if (GetKeyState(VK_MULTIPLY) < 0) {
			menu = 2;
		}
		else if (GetKeyState(VK_SUBTRACT) < 0) {
			menu = 3;
		}
		else if (GetKeyState(VK_NUMLOCK) < 0) {
			menu = 0;
		}
		if (GetKeyState(VK_ADD) < 0) {
			menu = menu + 1;
			Sleep(200);
		}
		//Now we apply the effect to the keyboard

		//You can work with the Result as well, e.g. checking if everythings ok
		if (menu > 3) {
			menu = 0;
		}

		Example_keyboard_effect_key.Color[HIBYTE(0x0014)][LOBYTE(0x0014)] = 0x01000000 | colour;
		RZRESULT Result_Keyboard = CreateKeyboardEffect(Keyboard::CHROMA_CUSTOM_KEY, &Example_keyboard_effect_key, NULL);
		return Result_Keyboard;

		return FALSE;

		//return Result_Keyboard;
		return FALSE;
	}
}

int main() {
	SetConsoleTitle("Chroma Paint");
	cout << "Starting...\n";
	My_Chroma_Implementation impl_test; //Initialize ChromaSDK
	auto test_for_init = impl_test.Initialize(); // Initialize all Chroma devices	
	if (test_for_init == TRUE) {
		cout << "Chroma Initialized";
		Sleep(500);
		cout << ".";
		Sleep(500);
		cout << ".";
		Sleep(500);
		cout << ".\n";
		cout << "\n";	
		cout << "Welcome to ";
		HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hStdOut, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		cout << "C";
		SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		cout << "h";
		SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << "r";
		SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		cout << "o";
		SetConsoleTextAttribute(hStdOut, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << "m";
		SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		cout << "a";		
		SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		cout << " Paint V1.2. For help press F1, for changelog press F2";

		
				while (true) {
			auto Keyboard = impl_test.example_keyboard();
			
		}	
			//If you want to, catch the BOOL Values and display a message if something has gone wrong
			
		
	}
	else
	{
		cout << "Unable to initialize Chroma.\n";
		cin.ignore();
	}



	return 0;

}