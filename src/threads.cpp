#include "threads.h"

const LPCWSTR kClassName = TEXT("SteadyHand");

// constants for Windows API raw input device codes.
const short kDesktopUsage = 1;
const short kMouseUsage = 2;
const short kKeyBoardUsage = 6;

void ServerThread::threadedFunction() {
	crow::SimpleApp steady_hand_server;

	CROW_ROUTE(steady_hand_server, "/GS").methods("POST"_method)
		([this](const crow::request &post_request) {

		try {
			json weap_payload = json::parse(post_request.body)["player"]["weapons"];
			if (!weap_payload.is_null()) {
				for (auto &weapon : weap_payload) {
					if (weapon["state"] == "active") {
						std::string weapon_name = weapon["name"];
						lock();
						equipped_weapon = weapon_name;
						unlock();
						break;
					}
				}
			}
		} catch (json::parse_error) {
			std::cout << "JSON payload did not have appropriate data." << std::endl;
		}
		return crow::response(200);
	});

	steady_hand_server.port(8080).run();
}

void MoverThread::CreateHiddenWindow() {
	WNDCLASS hid_wind_class;
	HINSTANCE h_inst = GetModuleHandle(NULL);

	// code referenced from Microsoft MSDN documentation:
	// https://msdn.microsoft.com/en-us/library/ms633576(v=vs.85).aspx
	hid_wind_class.style = 0;
	hid_wind_class.lpfnWndProc = DefWindowProc;
	hid_wind_class.cbClsExtra = 0;
	hid_wind_class.cbWndExtra = 0;
	hid_wind_class.hInstance = h_inst;
	hid_wind_class.hIcon = NULL;
	hid_wind_class.hCursor = NULL;
	hid_wind_class.hbrBackground = (HBRUSH)COLOR_WINDOWFRAME;
	hid_wind_class.lpszMenuName = NULL;
	hid_wind_class.lpszClassName = kClassName;

	if (RegisterClass(&hid_wind_class)) {
		mouse_mover_wind = CreateWindow(kClassName, kClassName, 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, h_inst, this);
		SetWindowLongPtr(mouse_mover_wind, GWLP_USERDATA, (LONG_PTR)this);
		std::cout << "Window registration successful." << std::endl;
	} else {
		std::cout << "Window registration failed." << std::endl;
	}
}

void MoverThread::RegisterMouse() {
	// code referenced from Microsoft MSDN documentation:
	// https://msdn.microsoft.com/en-us/library/windows/desktop/ms645546(v=vs.85).aspx#example_2
	RAWINPUTDEVICE Rid[2];

	Rid[0].usUsagePage = kDesktopUsage;
	Rid[0].usUsage = kMouseUsage;
	Rid[0].dwFlags = RIDEV_NOLEGACY | RIDEV_INPUTSINK;
	Rid[0].hwndTarget = mouse_mover_wind;

	Rid[1].usUsagePage = kDesktopUsage;
	Rid[1].usUsage = kKeyBoardUsage;
	Rid[1].dwFlags = RIDEV_NOLEGACY | RIDEV_INPUTSINK;
	Rid[1].hwndTarget = mouse_mover_wind;

	if (RegisterRawInputDevices(Rid, 2, sizeof(Rid[0]))) {
		SetWindowLongPtr(mouse_mover_wind, GWLP_WNDPROC, (LONG_PTR)StaticWinProc);
		std::cout << "Device registration successful." << std::endl;
	} else {
		std::cout << "Device registration failed." << std::endl;
	}
}

void MoverThread::SetupMover() {
	mouse_mover_wind = NULL;
	is_m_left_down = false;
	CreateHiddenWindow();
	RegisterMouse();
	std::cout << "setup complete." << std::endl;
}

void MoverThread::threadedFunction() {
	std::cout << "mover thread started." << std::endl;
	MSG message;
	BOOL msg_code;

	// The MS MSDN-recommended way to do message loops:
	// https://msdn.microsoft.com/en-us/library/windows/desktop/ms644928(v=vs.85).aspx#creating_loop
	while (((msg_code = GetMessage(&message, NULL, 0, 0)) != 0) && isThreadRunning()) {
		if (msg_code == -1) {
			stopThread();
			return;
		} else {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}
}

// code derived from:
// https://stackoverflow.com/a/28491549
LRESULT MoverThread::StaticWinProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
	MoverThread *p_this = (MoverThread*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	
	if (!p_this) {
		return DefWindowProc(hwnd, msg, w_param, l_param);
	}

	return p_this->MouseMoverProc(msg, w_param, l_param);
}

LRESULT MoverThread::MouseMoverProc(UINT msg, WPARAM w_param, LPARAM l_param) {
	switch (msg) {
	case WM_INPUT: {
		UINT dw_size;
		GetRawInputData((HRAWINPUT)l_param, RID_INPUT, NULL, &dw_size, sizeof(RAWINPUTHEADER));
		LPBYTE lpb = new BYTE[dw_size];
		if (lpb == NULL) {
			return -1;
		}

		if (GetRawInputData((HRAWINPUT)l_param, RID_INPUT, lpb, &dw_size, sizeof(RAWINPUTHEADER)) != dw_size) {
			std::cout << "GetRawInputData does not return correct size !" << std::endl;
			return -1;
		}
		// size of message/data has been confirmed to be correct.
		RAWINPUT* raw_input = (RAWINPUT*)lpb;

		if (raw_input->header.dwType == RIM_TYPEMOUSE) {
			std::cout << "mouse input received" << std::endl;
			switch (raw_input->data.mouse.usButtonFlags) {
			case 1:
				is_m_left_down = true;
				std::cout << "m_left pressed." << std::endl;
				break;
			case 2:
				is_m_left_down = false;
				std::cout << "m_left released." << std::endl;
				break;
			default:
				break;
			}
		}
		break;
	} default:
		return DefWindowProc(mouse_mover_wind, msg, w_param, l_param);
	}

	return 0;
}
