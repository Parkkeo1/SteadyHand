#pragma once

#include "mouse_pattern_data.h"

const LPCWSTR kClassName = TEXT("SteadyHand");

// constants for Windows API raw input device codes.
const short kDesktopUsage = 1;
const short kMouseUsage = 2;
const short kKeyBoardUsage = 6;

// default hotkey Windows Vkey codes.
const short kExitVKey = 18; // by default it is ALT.
const short kSaveVKey = 9; // by default it is TAB.

// global variables necessary in handling windows and storing message data.
std::vector<MouseData> mouse_data_list;
Weapon curr_weapon;
WNDPROC baseWndProc;

void RegisterInputDevices(HWND window);
bool CreateHiddenWindow(HINSTANCE &hInstance, HWND &window_handle);
void WriteBufferToFile(const Weapon weapon_to_save);

LRESULT CALLBACK WndProc(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param);
void RunMouseRecorder(HINSTANCE &h_inst, bool &is_recording);

