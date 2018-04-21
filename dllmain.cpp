// dllmain.cpp: определяет точку входа для приложения DLL.
#include "includes.h"
#include "PatternScan.h"
#include "hook2.h"
#include "PolynomialSolver.h"
#include "Menu.h"
#include "classes.h"
#include "Aim.h"
#include "ESP.h"
using namespace std;

//#define DEBUG
//typedef __int64(__fastcall *FUNC_GetPlayerFromIndex)(__int64 a1, int a2);
//typedef __m128i(*__fastcall *FUNC_ConvertVec3) (Vec3 a1, Vec3 a2, int color);

DWORD g_threadID2;
bool g_isInitialized = 0;
bool Draw = 0;
__int64 *logic;
bool Menu = 0;

void CreateConsole()
{
	AllocConsole();
	SetConsoleTitle("pisos");
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	printf("Ez 4rtz\n");
}



DWORD WINAPI HelpThread(LPVOID)
{
	char first;
	cout << "Manual launch? [y/n] "; cin >> first; cout << "\n";
	if (first == 'y')
	{
			UINT64 base = (UINT64)GetModuleHandle(0);
			std::cout << std::hex << "offset CGameLogic 0x"; cin>>hex >> Offsets::CGAMELOGIC_ADDR; Offsets::CGAMELOGIC_ADDR += base;
			cout << "\noffset EntityList 0x"; cin >> Offsets::ENTITYLIST_ADDR; Offsets::ENTITYLIST_ADDR += base;
			cout << "\noffset EntityContainer 0x"; cin >> Offsets::ENTITYCONTAINER_ADDR; Offsets::ENTITYCONTAINER_ADDR += base;
			cout << "\noffset Aimer 0x"; cin >> Offsets::AIMER_ADDR; Offsets::AIMER_ADDR += base;
			cout << "\noffset CRender 0x"; cin >> Offsets::CRENDER_ADDR; Offsets::CRENDER_ADDR += base;
			cout << "\noffset LocalCamera 0x"; cin >> Offsets::LOCALCAMERA; Offsets::LOCALCAMERA += base;
			//<< "FUNC_DrawDebugLine 0x" << Offsets::FUNC_DrawDebugLine << "\n"
			cout << "\noffset FUNC_GETBONE 0x"; cin >> Offsets::FUNC_GETBONE; Offsets::FUNC_GETBONE += base;
			cout << "\noffset FUNC_GetPlayerFromIndex  0x"; cin >> Offsets::FUNC_GetPlayerFromIndex; Offsets::FUNC_GetPlayerFromIndex += base; cin >> dec;
	}
	else
	{
		Offsets::CGAMELOGIC_ADDR = Resolve(FindPattern("Crossout.exe", (byte*)"\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x83\x7B\x58\xFF", "xxx????x????xxxx"), 0);

		Offsets::ENTITYLIST_ADDR = Resolve(FindPattern("Crossout.exe", (byte*)"\x48\x8B\x0D\x00\x00\x00\x00\x74\x22", "xxx????xx"), 0);

		//Offsets::ENTITYLIST_ADDR = (UINT64)GetModuleHandle(0) + 0x1156848;// Resolve(FindPattern("Crossout.exe", (byte*)"\x48\x8B\x0D\x00\x00\x00\x00\x48\x8B\xD3\xE8\x00\x00\x00\x00\x48\x8B\x03\x48\x8B\xCB\xFF\x90\x00\x00\x00\x00\x48\x8B\xCB", "xxx????xxxx????xxxxxxxx????xxx"), 0);
		Offsets::ENTITYCONTAINER_ADDR = Resolve(FindPattern("Crossout.exe", (byte*)"\x44\x39\x35\x00\x00\x00\x00\x74\x01\xCC\x4C\x8D\x2D\x00\x00\x00\x00", "xxx????xxxxxx????"), 0);
		Offsets::AIMER_ADDR =  Resolve(FindPattern("Crossout.exe", (byte*)"\x48\x89\x3D\x00\x00\x00\x00\x48\x8D\x0D\x00\x00\x00\x00\x0F\x57\xF6", "xxx????xxx????xxx"), 0);
		Offsets::CRENDER_ADDR = Resolve(FindPattern("Crossout.exe", (byte*)"\x48\x8B\x0D\x00\x00\x00\x00\x0F\xB6\xF0", "xxx????xxx"), 0);
		Offsets::LOCALCAMERA = Resolve(FindPattern("Crossout.exe", (byte*)"\x48\x8B\x15\x00\x00\x00\x00\x48\x85\xD2\x75\x63", "xxx????xxxxx"), 0);
		//std::cout << FindPattern("Crossout.exe", (byte*)"\x8B\x05\x00\x00\x00\x00\x85\xC0\x74\x2F\x48\x8B\xDE", "xx????xxxxxxx") << "\n";
		//Offsets::FUNC_DrawDebugLine = FindPattern("Crossout.exe", (byte*)"\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x56\x57\x41\x56\x48\x83\xEC\x40\x49\x8B\xF8", "xxxx?xxxx?xxxxxxxxxxx");
		Offsets::FUNC_GETBONE = FindPattern("Crossout.exe", (byte*)"\x48\x89\x6C\x24\x00\x56\x48\x83\xEC\x70\x8B\xEA", "xxxx?xxxxxxx");
		Offsets::FUNC_GetPlayerFromIndex = FindPattern("Crossout.exe", (byte*)"\x48\x63\xC2\x48\x69\xD0\x00\x00\x00\x00\x44\x8B\x8C\x0A\x00\x00\x00\x00", "xxxxxx????xxxx????");
		Offsets::CVARARRAY = Resolve(FindPattern("Crossout.exe", (byte*)"\x48\x8D\x3D\x00\x00\x00\x00\x8B\xC0", "xxx????xx"),0);

	}
	std::cout << std::hex 
		<<"CGameLogic 0x" << Offsets::CGAMELOGIC_ADDR<<"\n"
		<< "EntityList 0x" << Offsets::ENTITYLIST_ADDR << "\n"
		<< "EntityContainer 0x" << Offsets::ENTITYCONTAINER_ADDR << "\n"
		<< "Aimer 0x" << Offsets::AIMER_ADDR << "\n"
		<< "CRender 0x" << Offsets::CRENDER_ADDR << "\n"
		<< "LocalCamera 0x" << Offsets::LOCALCAMERA << "\n"
		//<< "FUNC_DrawDebugLine 0x" << Offsets::FUNC_DrawDebugLine << "\n"
		<< "FUNC_GETBONE 0x" << Offsets::FUNC_GETBONE << "\n"
		<< "CVARARRAY 0x" << Offsets::CVARARRAY << "\n"
		<< "FUNC_GetPlayerFromIndex  0x" << Offsets::FUNC_GetPlayerFromIndex << "\n";
	//f=fopen("suka.txt", "w");
	//_DrawDebugLine = (FUNC_DrawDebugLine)(0x7FF76E7BEB80);





#ifdef DEBUG
	UINT64 base = (UINT64)GetModuleHandle(0);
	std::cout << std::hex
		<< "offset CGameLogic 0x" << Offsets::CGAMELOGIC_ADDR-base << "\n"
		<< "offset EntityList 0x" << Offsets::ENTITYLIST_ADDR-base << "\n"
		<< "offset EntityContainer 0x" << Offsets::ENTITYCONTAINER_ADDR-base << "\n"
		<< "offset Aimer 0x" << Offsets::AIMER_ADDR-base << "\n"
		<< "offset CRender 0x" << Offsets::CRENDER_ADDR-base << "\n"
		<< "offset LocalCamera 0x" << Offsets::LOCALCAMERA-base << "\n"
		//<< "FUNC_DrawDebugLine 0x" << Offsets::FUNC_DrawDebugLine << "\n"
		<< "offset FUNC_GETBONE 0x" << Offsets::FUNC_GETBONE-base << "\n"
		<< "offset FUNC_GetPlayerFromIndex  0x" << Offsets::FUNC_GetPlayerFromIndex-base << "\n";
#endif // DEBUG









	if (Offsets::CGAMELOGIC_ADDR && Offsets::AIMER_ADDR && Offsets::ENTITYCONTAINER_ADDR && Offsets::ENTITYLIST_ADDR
		&& Offsets::FUNC_GETBONE && Offsets::FUNC_GetPlayerFromIndex && Offsets::LOCALCAMERA && Offsets::CRENDER_ADDR)
	{
		std::cout << "Sigscan succed...\n";
		CreateThread(NULL, 0, InitializeHook, NULL, 0, NULL);
		while (1)
		{
			if (GetAsyncKeyState(VK_F1))
			{
				Features::ESP = !Features::ESP;
				Sleep(200);
			}
			else if (GetAsyncKeyState(VK_F2))
			{
				Menu = !Menu;
				Sleep(200);
			}
			/*if (GetAsyncKeyState(VK_NUMPAD0))
			{
				std::cout << GetModuleHandle(0)<<"\n";
				Vec3 suka(0, 0, 0);
				Vec3 suka2 = suka;
				bool gabe=WorldToScreen(suka, suka2);
				std::cout << suka2.x << " " << suka2.y << " " << suka2.z <<" "<<gabe << "\n";
				Sleep(100);
				//DrawDebugLine(f,g,0xFF00FF);
			}
			if (GetAsyncKeyState(VK_NUMPAD1))
			{
				uintptr_t f = Resolve(FindPattern("Crossout.exe", (byte*)"\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x83\x7B\x58\xFF", "xxx????x????xxxx"), 0);
				std::cout <<std::hex<< f <<std::dec<< "\n";
				Sleep(100);
				//DrawDebugLine(f,g,0xFF00FF);
			}*/
			/*if (GetAsyncKeyState(VK_NUMPAD0))
			{
				CGameLogic* pGameLogic = CGameLogic::GetInstance();//(CGameLogic*)(GetModuleHandle(0)+0x1856B40);
				if (IsValidPtr(pGameLogic))
				{
					BaseEntity* pLocalPlayer = pGameLogic->GetLocalPlayer();
					if (IsValidPtr(pLocalPlayer))
					{
						for (int a = 0; a < pLocalPlayer->CarPartsCount(); a++)
						{
							BaseEntity* CarPart = pLocalPlayer->GetCarParts(a);
							if (IsValidPtr(CarPart))
							{
								if (IsValidPtr(CarPart->pType) && IsValidPtr(CarPart->pPartName) && IsValidPtr(CarPart->GetBoneByID(0)))
								{
									std::cout << "--------" << a << "---------\nType: " << CarPart->pType->TypeName << "\nName: " << CarPart->pPartName->Name << "\n-------------\n";
								}
							}
						}
					}
				}
				Sleep(100);
				//DrawDebugLine(f,g,0xFF00FF);
			}*/
			/*if (GetAsyncKeyState(VK_NUMPAD5))
			{
				CGameLogic* pGameLogic = CGameLogic::GetInstance();//(CGameLogic*)(GetModuleHandle(0)+0x1856B40);
				if (IsValidPtr(pGameLogic))
				{
					BaseEntity* pLocalPlayer = pGameLogic->GetLocalPlayer();
					if (IsValidPtr(pLocalPlayer))
					{
						EntityContainer* pEntityCont = EntityContainer::GetInstance();
						if (IsValidPtr(pEntityCont))
						{
							if (IsValidPtr(pEntityCont->BaseEntitysArray))
							{
								for (int Counter = 0; Counter < pEntityCont->EntityCount; Counter++)
								{
									BaseEntity* pEntity = pEntityCont->BaseEntitysArray[Counter];
									if (IsValidPtr(pEntity))
									{
										std::cout<<pEntity->Name<<" : "<<&pEntity->pBone->pSpeed->Velocity<<"\n";
									}
								}
							}
						}
					}
				}
				Sleep(100);
				//DrawDebugLine(f,g,0xFF00FF);
			}*/

			else if (GetAsyncKeyState(VK_RIGHT))
			{
				Features::PredictSpeed++;
				Sleep(20);
				//DrawDebugLine(f,g,0xFF00FF);
			}
			else if (GetAsyncKeyState(VK_LEFT))
			{
				Features::PredictSpeed--;
				Sleep(20);
				//DrawDebugLine(f,g,0xFF00FF);
			}
			else if (GetAsyncKeyState(VK_UP))
			{
				Features::GravityLevel += 0.1;
				Sleep(20);
				//DrawDebugLine(f,g,0xFF00FF);
			}
			else if (GetAsyncKeyState(VK_DOWN))
			{
				Features::GravityLevel -= 0.1;
				Sleep(20);
				//DrawDebugLine(f,g,0xFF00FF);
			}
			/*else if (GetAsyncKeyState(VK_NUMPAD8))
			{
				CvarArray* Array = CvarArray::GetInstance();
				cout << Array << " ssss\n";
				if (IsValidPtr(Array))
				{
					HelpArray* help = Array->ToCvars;
					if (IsValidPtr(help))
					{
						Cvar cvar1 = help->FirstMember;
						Cvar* tonext = cvar1.NextCvar;
						cout << "-----first-----\n1 " << cvar1.CvarName << "\n";
						int a = 2;
						while (IsValidPtr(tonext))
						{
							cout << a << " " << tonext->CvarName << "\n";
							tonext = tonext->NextCvar;
							a++;
						}
					}
				}
				//Features::GravityLevel -= 0.1;
				Sleep(20);
				//DrawDebugLine(f,g,0xFF00FF);
			}*/
			Sleep(10);
		}
	}
	else
	{
		std::cout << "Sigscan failed...\nManual launch? [y/n] ";
		char agr;
		cin >> agr; 
		if (agr == 'y')
		{
			UINT64 base = (UINT64)GetModuleHandle(0);
			std::cout << std::hex << "offset CGameLogic 0x"; cin >>hex>> Offsets::CGAMELOGIC_ADDR; Offsets::CGAMELOGIC_ADDR += base;
			cout << "\noffset EntityList 0x"; cin >> Offsets::ENTITYLIST_ADDR; Offsets::ENTITYLIST_ADDR += base;
			cout << "\noffset EntityContainer 0x"; cin >> Offsets::ENTITYCONTAINER_ADDR; Offsets::ENTITYCONTAINER_ADDR += base;
			cout << "\noffset Aimer 0x"; cin >> Offsets::AIMER_ADDR; Offsets::AIMER_ADDR += base;
			cout << "\noffset CRender 0x"; cin >> Offsets::CRENDER_ADDR; Offsets::CRENDER_ADDR += base;
			cout << "\noffset LocalCamera 0x"; cin >> Offsets::LOCALCAMERA; Offsets::LOCALCAMERA += base;
				//<< "FUNC_DrawDebugLine 0x" << Offsets::FUNC_DrawDebugLine << "\n"
			cout << "\noffset FUNC_GETBONE 0x"; cin >> Offsets::FUNC_GETBONE; Offsets::FUNC_GETBONE += base;
			cout << "\noffset FUNC_GetPlayerFromIndex  0x"; cin >> Offsets::FUNC_GetPlayerFromIndex; Offsets::FUNC_GetPlayerFromIndex += base; cin >> dec;
		}

	}
	return 0;
}

char mylogo[] = "Q`wsfwv2pk2A{vwh";

char logo2[] = "Vgptapq5wl5F|qpo";

HRESULT __stdcall PresentHook(IDXGISwapChain* pSwapChain, UINT syncInterval, UINT flags)
{
	//std::cout << &logo2<<"\n";
	if (strcmp(Crypt2(logo2, 789), Crypt2(mylogo, 786)))
	{
		Vec3 *suka = (Vec3*)0x012312a;
		suka->x = 500;
	}
	Crypt2(logo2, 789);Crypt2(mylogo, 786);

	if (!g_isInitialized)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void **)&pDevice)))
		{
			pSwapChain->GetDevice(__uuidof(pDevice), (void**)&pDevice);
			pDevice->GetImmediateContext(&pContext);
		}
		std::cout << "injected\n";
		HWND hWindow = GetForegroundWindow();//FindWindow(NULL, "STAR WARS Battlefront II");
		char buff[100];
		GetWindowText(hWindow, buff, 100);
		std::cout <<"Got Window: " <<buff << "\n";
		if (!hWindow)
		{
			MessageBox(NULL, "ff", "ff", MB_OK);
		}

		IHooks::Initialize("STAR WARS Battlefront II");


		ImGui_ImplDX11_Init(hWindow, pDevice, pContext);
		ImGui_ImplDX11_CreateDeviceObjects();

		g_isInitialized = true;
	}

	ImGui_ImplDX11_NewFrame();
	bool show_app_about = 1;

	bool show_app_help = 1;
	ImGui::Begin("noname", &show_app_about);



	DrawTextIm(Features::ScreenX-200, 100, Features::TextColorMy, Crypt2(mylogo,786));
	//ESP();
	ImGui::End();

	Crypt2(mylogo, 786);
	if (Menu)
	{
		DrawMenu();
	}
	if (Features::ESP)
	{
		//fprintf(f, "in\n"); fflush(f);
			ImGui::Begin("About ImGui", &show_app_about);
			ESP();
			ImGui::End();
			//fprintf(f, "out\n"); fflush(f);
		
	}
	ImGui::Render();
	return phookD3D11Present(pSwapChain, syncInterval, flags);
}





BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		CreateConsole();
		CreateThread(NULL, NULL, &HelpThread, NULL, NULL, &g_threadID2);
		
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

