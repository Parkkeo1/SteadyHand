#pragma once
#include <windows.h>
#include <vector>
#include <iostream>
#include <chrono>
#include <tuple>

// constants for Windows API raw input device codes.
const short kDesktopUsage = 1;
const short kMouseUsage = 2;
const short kKeyBoardUsage = 6;

// default hotkey Windows Vkey codes.
const short kExitVKey = 18; // by default it is ALT.
const short kSaveVKey = 9; // by default it is TAB.


// booleans for checking if m1 is held down.
bool was_left_pressed;
bool was_right_pressed;

// global variables necessary in handling windows and storing message data.
std::vector<std::tuple<long long, short, short>> mouse_data_list;
WNDPROC baseWndProc;

// TODO: Add data filtering and file writing functions.
void RegisterInputDevices(HWND window);
bool CreateHiddenWindow(HINSTANCE &hInstance, HWND &window_handle);
LRESULT CALLBACK WndProc(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param);

