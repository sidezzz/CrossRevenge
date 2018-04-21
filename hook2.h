#pragma once
#pragma once
#include "MinHook/include/MinHook.h" //detour x86&x64
const int MultisampleCount = 1;
UINT pssrStartSlot;
D3D11_SHADER_RESOURCE_VIEW_DESC  Descr;
ID3D11ShaderResourceView* ShaderResourceView;

DWORD_PTR* pSwapChainVtable = NULL;
DWORD_PTR* pContextVTable = NULL;
DWORD_PTR* pDeviceVTable = NULL;

typedef HRESULT(__stdcall *D3D11PresentHook) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
D3D11PresentHook phookD3D11Present = NULL;

HRESULT __stdcall PresentHook(IDXGISwapChain* pSwapChain, UINT syncInterval, UINT flags);
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* g_pRenderTargetView = NULL;


typedef INT64(__fastcall *ReturnedFunc)(BYTE **a1, __int64 a2, __int64 a3);
ReturnedFunc Gaben228 = NULL;



typedef INT64(__fastcall *ReturnedFunc2)(__int64 a1, signed int a2);
ReturnedFunc2 Gaben229 = NULL;


typedef char(__fastcall *ReturnedFunc3)(__int64 a1, __int64 a2);
ReturnedFunc3 Gaben230 = NULL;


__int64 __fastcall DetourFunc(BYTE **a1, __int64 a2, __int64 a3)
{
	std::cout << "func - " << a1 << " " << a2 << " " << a3 << " " << **a1 << "\n";
	//AdressToCamera = gabe;
	//std::cout << "adr " << gabe << "\n";
	return 1;
}

__int64 __fastcall DetourFunc2(__int64 a1, signed int a2)
{
	std::cout << "func2 - " << a1 <<" "<<a2<< "\n";
	//AdressToCamera = gabe;
	__int64 result = Gaben229(a1, a2);
	std::cout << "result " << result << "\n";
	//std::cout << "adr " << gabe << "\n";
	return result;
}

char __fastcall DetourFunc3(__int64 a1, __int64 a2)
{
	std::cout << "func2 - " << a1 << " " << a2 << "\n";
	//AdressToCamera = gabe;
	char result = Gaben230(a1, a2);
	std::cout << "result " << result << " "<<(bool)result<<"\n";
	//std::cout << "adr " << gabe << "\n";
	return 0;
}







LRESULT CALLBACK DXGIMsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return DefWindowProc(hwnd, uMsg, wParam, lParam); }
DWORD __stdcall InitializeHook(LPVOID)
{
	//Sleep(5000);

	HMODULE hDXGIDLL = 0;
	do
	{
		hDXGIDLL = GetModuleHandle("dxgi.dll");
		Sleep(8000);
	} while (!hDXGIDLL);
	Sleep(100);

	HMODULE hD3D11DLL = 0;
	do
	{
		hD3D11DLL = GetModuleHandle("d3d11.dll");
		Sleep(100);
	} while (!hD3D11DLL);
	Sleep(1000);

	IDXGISwapChain* pSwapChain;

	WNDCLASSEXA wc = { sizeof(WNDCLASSEX), CS_CLASSDC, DXGIMsgProc, 0L, 0L, GetModuleHandleA(NULL), NULL, NULL, NULL, NULL, "DX", NULL };
	RegisterClassExA(&wc);
	HWND hWnd = CreateWindowA("DX", NULL, WS_OVERLAPPEDWINDOW, 100, 100, 300, 300, NULL, NULL, wc.hInstance, NULL);

	D3D_FEATURE_LEVEL requestedLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1 };
	D3D_FEATURE_LEVEL obtainedLevel;
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11Device* d3dDevice = nullptr;
	ID3D11DeviceContext* d3dContext = nullptr;

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(scd));
	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	scd.OutputWindow = hWnd;
	scd.SampleDesc.Count = MultisampleCount;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Windowed = ((GetWindowLongPtr(hWnd, GWL_STYLE) & WS_POPUP) != 0) ? false : true;

	// LibOVR 0.4.3 requires that the width and height for the backbuffer is set even if
	// you use windowed mode, despite being optional according to the D3D11 documentation.
	//scd.BufferDesc.Width = 1;
	//scd.BufferDesc.Height = 1;
	//scd.BufferDesc.RefreshRate.Numerator = 0;
	//scd.BufferDesc.RefreshRate.Denominator = 1;

	UINT createFlags = 0;
#ifdef _DEBUG
	// This flag gives you some quite wonderful debug text. Not wonderful for performance, though!
	createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	IDXGISwapChain* d3dSwapChain = 0;

	if (FAILED(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		NULL,//createFlags,
		&featureLevel,//requestedLevels,
		1,//sizeof(requestedLevels) / sizeof(D3D_FEATURE_LEVEL),
		D3D11_SDK_VERSION,
		&scd,
		&pSwapChain,
		&pDevice,
		NULL,//&obtainedLevel,
		&pContext)))
	{
		MessageBox(hWnd, "Failed to create directX device and swapchain!", "Error", MB_ICONERROR);
		return NULL;
	}


	pSwapChainVtable = (DWORD_PTR*)pSwapChain;
	pSwapChainVtable = (DWORD_PTR*)pSwapChainVtable[0];

	pContextVTable = (DWORD_PTR*)pContext;
	pContextVTable = (DWORD_PTR*)pContextVTable[0];

	pDeviceVTable = (DWORD_PTR*)pDevice;
	pDeviceVTable = (DWORD_PTR*)pDeviceVTable[0];

	if (MH_Initialize() != MH_OK) { return 1; }
	if (MH_CreateHook((DWORD_PTR*)pSwapChainVtable[8], PresentHook, reinterpret_cast<void**>(&phookD3D11Present)) != MH_OK) { return 1; }
	if (MH_EnableHook((DWORD_PTR*)pSwapChainVtable[8]) != MH_OK) { return 1; }


	//MH_CreateHook((DWORD_PTR*)(0x7FF6A1EA1980), DetourFunc, reinterpret_cast<void**>(&Gaben228));
	//MH_EnableHook((DWORD_PTR*)(0x7FF6A1EA1980));


	//MH_CreateHook((DWORD_PTR*)(0x7FF6A1C52F90), DetourFunc2, reinterpret_cast<void**>(&Gaben229));
	//MH_EnableHook((DWORD_PTR*)(0x7FF6A1C52F90));


	//MH_CreateHook((DWORD_PTR*)(0x7FF6A1B5E4F0), DetourFunc3, reinterpret_cast<void**>(&Gaben230));
	//MH_EnableHook((DWORD_PTR*)(0x7FF6A1B5E4F0));

	//if (MH_CreateHook((DWORD_PTR*)pDeviceVTable[24], hookD3D11CreateQuery, reinterpret_cast<void**>(&phookD3D11CreateQuery)) != MH_OK) { return 1; }
	//if (MH_EnableHook((DWORD_PTR*)pDeviceVTable[24]) != MH_OK) { return 1; }
	//if (MH_CreateHook((DWORD_PTR*)pContextVTable[8], hookD3D11PSSetShaderResources, reinterpret_cast<void**>(&phookD3D11PSSetShaderResources)) != MH_OK) { return 1; }
	//if (MH_EnableHook((DWORD_PTR*)pContextVTable[8]) != MH_OK) { return 1; }


	DWORD dwOld;
	VirtualProtect(phookD3D11Present, 2, PAGE_EXECUTE_READWRITE, &dwOld);

	while (true) {
		Sleep(10);
	}

	pDevice->Release();
	pContext->Release();
	pSwapChain->Release();

	return NULL;
}