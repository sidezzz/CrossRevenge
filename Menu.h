#pragma once


#ifdef _AMD64_
void* ptr_max_value = ((PVOID)0x000F000000000000);
#else
constexpr void* ptr_max_value = reinterpret_cast< void* >(0xFFF00000);
#endif

template <typename T>
bool IsValidPtr(T p)
{
	return reinterpret_cast< void* >(p) >= reinterpret_cast< void* >(0x10000) && reinterpret_cast< void* >(p) < ptr_max_value;
}


class CvarArray
{
public:
	class Cvar* Cvars[2000]; //0x0000
	char pad_1F40[528]; //0x1F40
	static CvarArray* GetInstance()
	{
		return (CvarArray*)Offsets::CVARARRAY;
	}
}; //Size: 0x2150


class Cvar
{
public:
	float Value;
	char pad_0000[20]; //0x0000
	int32_t Type; //0x0018 48 bool 49 bool 53 int 50 int 52 float 55 float
	char pad_001C[20]; //0x001C
	class Cvar* NextCvar; //0x0030
	char pad_0038[8]; //0x0038
	char* CvarName; //0x0040
	char pad_0048[16]; //0x0048
	int32_t min; //0x0058
	int32_t max; //0x005C
	char pad_0060[32]; //0x0060
}; //Size: 0x0080


class Features
{
public:
	static float LockColor[4];
	static float BoxColor[3];
	static float TextColor[4];
	static float TextColorMy[4];

	static float TeslaColor[3];
	static float MandrakeColor[3];
	static float BigGunsColor[3];
	static float MinigunColor[3];
	static float MeleeColor[3];
	static float DronesColor[3];
	static float HammerColor[3];
	static float AegisColor[3];
	static float ScorpionColor[3];
	static float OtherColor[3];

	static bool ESP;
	static bool Boxes;
	static bool RangeToTarget;
	static bool DrawStrings;
	static bool DrawStringsForAlly;

	static float AimSpeed;
	static bool LockMode;
	static float SightDistance;
	static float Yoffset;
	static bool Predict;
	static int PredictSpeed;
	static bool Gravity;
	static float GravityLevel;
	static bool PredictWeapon;

	static int ScreenX;
	static int ScreenY;
	static bool SelfRender;
};

float Features::BoxColor[3]={ 1,0,0 };
float Features::LockColor[4] = { 1,0,0,1 };
float Features::TextColor[4] = { 1,1,1,1 };
float Features::TextColorMy[4] = { 0,0,1,1 };


float Features::TeslaColor[3]={ 0.282,0.819,0.8 };
float Features::MandrakeColor[3] = { 0.545,0.27,0.06};
float Features::BigGunsColor[3] = { 1,0.843,0 };
float Features::MinigunColor[3] = { 0.698,0.133,0.133 };
float Features::MeleeColor[3] = { 0.415,0.352,0.803 };
float Features::DronesColor[3] = { 0,0,1 };
float Features::HammerColor[3] = { 0.5,0.5,0 };
float Features::AegisColor[3] = { 1,0.411,0.705 };
float Features::ScorpionColor[3] = { 1,1,1 };
float Features::OtherColor[3] = { 0.196,0.803,0.196 };

bool Features::PredictWeapon = 0;
float Features::AimSpeed = 3;
bool Features::LockMode = 1;
float Features::SightDistance = 350;
float Features::Yoffset = 0.5;
bool Features::Predict = 0;
int Features::PredictSpeed = 700;
bool Features::Gravity = 1;
float Features::GravityLevel = 17.5;

bool Features::ESP = 1;
bool Features::Boxes = 0;
bool Features::RangeToTarget= 1;
bool Features::DrawStrings = 1;
bool Features::DrawStringsForAlly =0;

bool MainMenu = true;

int Features::ScreenX = 1920;
int Features::ScreenY = 1080;
bool Features::SelfRender = 0;

char* Crypt2(char * str, int key)
{
	//char buff[255] = { 0 };
	//strcpy_s(buff, str);
	for (int a = 0; a < strlen(str); a++)
	{
		str[a] = str[a] ^ key;
	}
	//std::cout << &str << " - crypt2 str\n";
	//std::cout << buff << std::endl;
	return str;
}
char menuname1[] = "@qlppQfufmdf#elq#vmhmltm`kfbwp-nf";

char menuez[] = "obkkt";

UINT64 CheckSum(char*str)
{
	UINT64 f = 0;
	for (int a = 0; a < strlen(str); a++)
	{
		f += (int)str[a];
	}
	return f;
}
#define menuname1check 3278

int id = 0;
int idtype = 0;
char bufsearch[256] = { 0 };

void DrawMenu()
{
	UINT64 f = CheckSum(menuname1);
	
	//std::cout <<&menuname1<<" "<< std:: dec << f << "\n";
	//if (strstr(menuname1, "obkkt"))
	//{
	//	exit(0);
	//}
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	if (ImGui::Begin(Crypt2(menuname1,3), &MainMenu, ImVec2(505, 600), 0.9f))
	{
		Crypt2(menuname1, 3);
		if (ImGui::CollapsingHeader("ESP"))
		{
			ImGui::Checkbox("Enable ESP", &Features::ESP); ImGui::SameLine(); ImGui::Checkbox("Draw Boxes", &Features::Boxes);
			ImGui::Checkbox("Range to target", &Features::RangeToTarget);
			ImGui::Checkbox("Draw Str", &Features::DrawStrings); ImGui::SameLine(); ImGui::Checkbox("Draw str for ally", &Features::DrawStringsForAlly);
			ImGui::ColorEdit3("Box Color", Features::BoxColor);
			ImGui::ColorEdit4("Lock Color", Features::LockColor);
			ImGui::ColorEdit4("Text Color", Features::TextColor);

			ImGui::Separator();

			ImGui::ColorEdit3("Tesla", Features::TeslaColor);
			ImGui::ColorEdit3("Mandrake", Features::MandrakeColor);
			ImGui::ColorEdit3("Heavy gun", Features::BigGunsColor);
			ImGui::ColorEdit3("Scorpion", Features::ScorpionColor);
			ImGui::ColorEdit3("Minigun", Features::MinigunColor);
			ImGui::ColorEdit3("Shield", Features::AegisColor);
			ImGui::ColorEdit3("Hammerfall", Features::HammerColor);
			ImGui::ColorEdit3("Drones", Features::DronesColor);
			ImGui::ColorEdit3("Melee", Features::MeleeColor);
			ImGui::ColorEdit3("Other", Features::OtherColor);

		}
		if (ImGui::CollapsingHeader("Aim"))
		{
			ImGui::Checkbox("Lock Mode", &Features::LockMode); ImGui::SameLine(); ImGui::Checkbox("Predict", &Features::Predict);
			ImGui::Checkbox("Gravity", &Features::Gravity); ImGui::SameLine(); ImGui::Checkbox("Weapon Predict", &Features::PredictWeapon);
			ImGui::SliderFloat("Aim Speed", &Features::AimSpeed, 0.1, 20);
			ImGui::SliderFloat("Distance from sight", &Features::SightDistance, 10, 1000);
			ImGui::SliderFloat("Yoffset", &Features::Yoffset, -3, 3);
			ImGui::SliderInt("Predict Speed", &Features::PredictSpeed, 0, 1000);
			ImGui::SliderFloat("Gravity Level", &Features::GravityLevel, 0, 20);
		}
		if (ImGui::CollapsingHeader("Screen resolution"))
		{
			ImGui::SliderInt("Screen x", &Features::ScreenX, 0, 4096);
			ImGui::SliderInt("Screen y", &Features::ScreenY, 0, 2048);
			ImGui::Checkbox("Self Render(for catching bugs)", &Features::SelfRender);
		}
		if (ImGui::CollapsingHeader("Cvars"))
		{
			CvarArray* arr = CvarArray::GetInstance();
			if (IsValidPtr(arr))
			{
				int count = 0;
				//std::cout << arr << "\n";
				Cvar* member = arr->Cvars[count];
				//std::cout << member << "\n";
				//std::cout << count << " " << member->CvarName << "\n";
				
				ImGui::InputText("Search str", bufsearch, 256);
				while (member->NextCvar != NULL)
				{
					//std::cout <<count<<" "<< member->CvarName << "\n";
					member = member->NextCvar;
					count++;
				}
				member = arr->Cvars[0];
				char **GayArray = new char*[count];
				int *reloc = new int[count];
				
				//strcpy(GayArray[0], "No Target");
				int counter = 0;
				for (int a = 0; a < count; a++)
				{
					if (bufsearch[0] != 0)
					{
						if (strstr(member->CvarName, bufsearch) != NULL)
						{
							
							reloc[counter] = a;
							GayArray[counter] = member->CvarName;
							counter++;
						}
					}
					else
					{
						reloc[counter] = a;
						GayArray[counter] = member->CvarName;
						//if (IsValidPtr(pEntity))
						//{
						//	if (IsValidPtr(pEntity->Name))
							//{
							//	GayArray[counter] = new char[16];
							//	strcpy(GayArray[counter], pEntity->Name);
						counter++;
					}
					//	}
				//	}
					member = member->NextCvar;

				}
				if (counter)
				{
					if (id >= counter)
					{
						id = 0;
					}
					char** arrsuka = new char*[3];
					//int cnt = 1;
					char types[3][30] = { "Float\0","Int\0","Bool\0" };
					for (int a = 0; a < 3; a++)
					{
						//	std::cout << types[a] << "\n";
						arrsuka[a] = new char[10];
						strcpy(arrsuka[a], types[a]);
						//cnt++;
					}
					ImGui::ListBox("CvarList ", &id, GayArray, counter);
					ImGui::Separator();
					ImGui::ListBox("types ", &idtype, arrsuka, 3);
					if (idtype == 0)
					{
						ImGui::SliderFloat("Value", &arr->Cvars[reloc[id]]->Value, arr->Cvars[reloc[id]]->min, arr->Cvars[reloc[id]]->max);
					}
					else if (idtype == 1)
					{
						float* gab = &arr->Cvars[reloc[id]]->Value;
						int* s = (int*)gab;
						ImGui::SliderInt("Value", s, arr->Cvars[reloc[id]]->min, arr->Cvars[reloc[id]]->max);
					}
					else if (idtype == 2)
					{
						float* gab = &arr->Cvars[reloc[id]]->Value;
						int* s = (int*)gab;
						ImGui::SliderInt("Value", s, 0, 1);
					}
					for (int a = 0; a < 3; a++)
					{
						delete[] arrsuka[a];
					}
					delete[] arrsuka;
				}
				delete[] GayArray;
				delete[] reloc;
				
			}
		}


		ImGui::End();
	}
	if (f != 3278)
	{
		int*u = 0x0;
		*u = 22222;
	}
	
}
















DWORD ConvertColor(float *ImGuiColor)
{
	DWORD result = (DWORD)(ImGuiColor[0] * 255) * 0x10000 + (DWORD)(ImGuiColor[1] * 255) * 0x100 + (DWORD)(ImGuiColor[2] * 255);
	//std::cout << std::hex <<"0x"<< result << " " << (DWORD)(ImGuiColor[0] * 255) * 0x10000 + (DWORD)(ImGuiColor[1] * 255) * 0x100 <<" "<< (DWORD)(ImGuiColor[2] * 255) <<"\n"<< std::dec;
	return result;
}


class IHooks
{
private:

	static HWND tWindow;

public:

	static BOOLEAN KeysPressed[256];

	static void Initialize(const char* Target);
	static void Restore();

	static void HooksInput();
};



LRESULT __stdcall HookedWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

WNDPROC OldWndProc = nullptr;

HWND IHooks::tWindow = nullptr;
BOOLEAN IHooks::KeysPressed[256] = { FALSE };

void IHooks::Initialize(const char* Target)
{
	tWindow = GetForegroundWindow();//FindWindow(0, Target);
	if (tWindow) OldWndProc = (WNDPROC)SetWindowLongPtr(tWindow, GWLP_WNDPROC, (LONG_PTR)HookedWndProc);
}

void IHooks::Restore()
{
	SetWindowLongPtr(tWindow, GWLP_WNDPROC, (LONG_PTR)OldWndProc);
}


LRESULT __stdcall HookedWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	ImGuiIO& io = ImGui::GetIO();
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		io.MouseDown[0] = true;
		break;
	case WM_LBUTTONUP:
		io.MouseDown[0] = false;
		break;
	case WM_RBUTTONDOWN:
		io.MouseDown[1] = true;
		break;
	case WM_RBUTTONUP:
		io.MouseDown[1] = false;
		break;
	case WM_MBUTTONDOWN:
		io.MouseDown[2] = true;
		break;
	case WM_MBUTTONUP:
		io.MouseDown[2] = false;
		break;
	case WM_MOUSEWHEEL:
		io.MouseWheel += GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? 1.0f : -1.0f;
		break;
	case WM_MOUSEMOVE:
		io.MousePos.x = (signed short)(lParam);
		io.MousePos.y = (signed short)(lParam >> 16);
		break;
	case WM_KEYDOWN:
		if (wParam == 52)
			break;
		if (wParam < 256)
			io.KeysDown[wParam] = 1;
		break;
	case WM_KEYUP:
		if (wParam == 52)
			break;
		if (wParam < 256)
			io.KeysDown[wParam] = 0;
		break;
	case WM_CHAR:
		// You can also use ToAscii()+GetKeyboardState() to retrieve characters.
		if (wParam > 0 && wParam < 0x10000)
			io.AddInputCharacter((unsigned short)wParam);
		break;

	}
	return CallWindowProc(OldWndProc, hWnd, uMsg, wParam, lParam);
}