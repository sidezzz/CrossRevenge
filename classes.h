#pragma once



//FUNC_DrawDebugLine _DrawDebugLine;// = (FUNC_DrawDebugLine)(0x7FF76E7BEB80);

typedef float vec_t;

class Vec3
{
public:
	float x;
	float y;
	float z;
	vec_t* GetBase()
	{
		return (vec_t*)this;
	}
	float Dot(Vec3 rhs)
	{
		return (x * rhs.x
			+ y * rhs.y
			+ z * rhs.z);
	}
	Vec3 operator+(Vec3 f)
	{
		Vec3 help;
		help.x = x + f.x;
		help.y = y + f.y;
		help.z = z + f.z;
		return help;
	}
	Vec3 operator+(float f)
	{
		Vec3 help;
		help.x = x + f;
		help.y = y + f;
		help.z = z + f;
		return help;
	}


	Vec3 operator-(Vec3 f)
	{
		Vec3 help;
		help.x = x - f.x;
		help.y = y - f.y;
		help.z = z - f.z;
		return help;
	}
	Vec3 operator*(float f)
	{
		Vec3 help;
		help.x = f*x;
		help.y = f*y;
		help.z = f*z;
		return help;
	}
	Vec3 operator/(float f)
	{
		Vec3 help;
		help.x = x / f;
		help.y = y / f;
		help.z = z / f;
		return help;
	}
	Vec3 operator-=(Vec3 f)
	{
		Vec3 help;
		help.x = x - f.x;
		help.y = y - f.y;
		help.z = z - f.z;
		return help;
	}
	Vec3 operator-()
	{
		Vec3 help;
		help.x = -x;
		help.y = -y;
		help.z = -z;
		return help;
	}
	operator Vector3()const
	{
		return Vector3(x, y, z);
	}
	bool operator==(Vec3 f)
	{
		if (x == f.x && y == f.y && z == f.z)
			return 1;
		else
			return 0;
	}
	Vec3()
	{
		x = 0.f;
		y = 0.f;
		z = 0.f;
	}
	Vec3(float a, float b, float c)
	{
		x = a;
		y = b;
		z = c;
	}
};



Vec3 ToVec3(Vector3 f)
{
	return Vec3(f.x, f.y, f.z);
}

class LockClass
{
public:
	Vec3 coord;
	int Pid;
};

float GetSquaredLength(Vec3 inVect)
{
	return (inVect.x * inVect.x
		+ inVect.y * inVect.y
		+ inVect.z * inVect.z);
}

Vec3 GetNormal(Vec3 f)
{
	float range = sqrt(powf(f.x, 2) + powf(f.y, 2) + powf(f.z, 2));
	Vec3 help;
	help.x = f.x / range;
	help.y = f.y / range;
	help.z = f.z / range;
	return help;
}

float GetMagnitude(Vec3 f)
{
	float range = sqrt(powf(f.x, 2) + powf(f.y, 2) + powf(f.z, 2));

	return range;
}

float Unitize(Vec3 &f)
{
	float inverseLength = 1.0f / GetMagnitude(f);
	f.x *= inverseLength;
	f.y *= inverseLength;
	f.z *= inverseLength;
	return inverseLength;
}
Vec3 Cross(Vec3 my, Vec3 &rhs)
{

	Vec3 help((my.y * rhs.z) - (my.z * rhs.y),
		(my.z * rhs.x) - (my.x * rhs.z),
		(my.x * rhs.y) - (my.y * rhs.x));
	return help;
}
int distance3D(Vec3 a, Vec3 b)
{
	return abs(sqrt(powf(a.x - b.x, 2) + powf(a.y - b.y, 2) + powf(a.z - b.z, 2)));
}

float distance3Df(Vec3 a, Vec3 b)
{
	return abs(sqrt(powf(a.x - b.x, 2) + powf(a.y - b.y, 2) + powf(a.z - b.z, 2)));
}

struct Vec2
{
	int x;
	int y;
};

int distance2D(Vec3 a, Vec3 b)
{
	int result = (int)abs(sqrt(powf(a.x - b.x, 2) + powf(a.y - b.y, 2)));
	return result;
}






class Matrix1
{
public:
	float matrix[4][4];
	Matrix1 operator *(Matrix1 m) //что должен выполнить оператор +
	{
		Matrix1 help;
		for (int i = 0; i < 4; i++)
		{
			for (int n = 0; n < 4; n++)
			{
				help.matrix[i][n] = matrix[i][0] * m.matrix[0][n] + matrix[i][1] * m.matrix[1][n]
					+ matrix[i][2] * m.matrix[2][n] + matrix[i][3] * m.matrix[3][n];
			}
		}
		return help;
	}
	void print()
	{
		std::cout << "\n" << matrix[0][0] << " " << matrix[0][1] << " " << matrix[0][2] << " " << matrix[0][3] << "\n"
			<< matrix[1][0] << " " << matrix[1][1] << " " << matrix[1][2] << " " << matrix[1][3] << "\n"
			<< matrix[2][0] << " " << matrix[2][1] << " " << matrix[2][2] << " " << matrix[2][3] << "\n"
			<< matrix[3][0] << " " << matrix[3][1] << " " << matrix[3][2] << " " << matrix[3][3] << "\n";
	}
};

/*void DrawDebugLine(Vec3 a, Vec3 b, __int32 color)
{
	typedef __int64(__fastcall *FUNC_DrawDebugLine) (__int64,Vec3 ConvertedVec3_1, Vec3 ConvertedVec3_2, __int32 color);
	static FUNC_DrawDebugLine _Draw = (FUNC_DrawDebugLine)(Offsets::FUNC_DrawDebugLine);
	_Draw((*(DWORD64*)Offsets::CRENDER_ADDR)+14192,a, b, color);
}*/

class Crender
{
public:
	char pad_0000[32]; //0x0000
	Matrix Projection; //0x0020
	Matrix ViewMatrix; //0x0060
	static Crender *GetInstance()
	{
		return *(Crender**)(Offsets::CRENDER_ADDR);
	}

};



bool WorldToScreen(const Vec3 & v3, Vec3& out)
{
	Crender* render = Crender::GetInstance();
	if (!IsValidPtr(render))
		return 0;

	//std::cout << render << "\n";
	Matrix viewProj = render->ViewMatrix*render->Projection;
	float w = viewProj._14 * v3.x + viewProj._24 * v3.y + viewProj._34 * v3.z + viewProj._44;
	if (w < 0.19)
		return false;
	float x = viewProj._11 * v3.x + viewProj._21 * v3.y + viewProj._31 * v3.z + viewProj._41;
	float y = viewProj._12 * v3.x + viewProj._22 * v3.y + viewProj._32 * v3.z + viewProj._42;
	out.x = Features::ScreenX / 2 + Features::ScreenX / 2 * x / w;
	out.y = Features::ScreenY / 2 - Features::ScreenY / 2 * y / w;
	out.z = 0;
	//std::cout << out.x << " " << out.y << " " << out.z << "\n";
	return 1;
}

void DrawLine(const ImVec2& from, const ImVec2& to, float* color, float thickness)
{
	//ImGuiWindow* window = ImGui::GetCurrentWindow();


	//float a = (color >> 24) & 0xff;
	//float r = (color >> 16) & 0xff;
	//float g = (color >> 8) & 0xff;
	//float b = (color) & 0xff;

	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->PushClipRectFullScreen();
	draw_list->AddLine(from, to, ImGui::GetColorU32(ImVec4(color[0], color[1], color[2], color[3])), thickness);
}


void DrawLine(const ImVec2& from, const ImVec2& to, float* color, float thickness,bool f)
{
	//ImGuiWindow* window = ImGui::GetCurrentWindow();


	//float a = (color >> 24) & 0xff;


	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->PushClipRectFullScreen();
	draw_list->AddLine(from, to, ImGui::GetColorU32(ImVec4(color[0], color[1], color[2], 1.f)), thickness);
}




void DrawDebugLine(Vec3 a, Vec3 b, float *color)
{
	Vec3 tmp1, tmp2;
	if (WorldToScreen(a, tmp1) && WorldToScreen(b, tmp2))
	{
		

		DrawLine(ImVec2(tmp1.x, tmp1.y), ImVec2(tmp2.x, tmp2.y), color, 1.f,1);
	}
	
}

class Speed
{
public:
	char pad_0000[144]; //0x0000
	Vector3 Velocity; //0x0090
	char pad_009C[944]; //0x009C
}; //Size: 0x044C

class BoneParts
{
public:
	BoneParts* GetAddress()
	{
		return this;
	}

	char pad_0000[24]; //0x0000
	class Speed* pSpeed; //0x0018
	char pad_0020[80]; //0x0020
	Vec3 Coord; //0x0070
	char pad_007C[132]; //0x007C
	Matrix1 matrix; //0x0100
	//Matrix matrix;
};


class Bone
{
public:
	char pad_0000[48]; //0x0000
	class Speed* pSpeed; //0x0030
	char pad_0038[72]; //0x0038
	Vector3 Coord; //0x0080
	char pad_008C[120]; //0x008C
	Matrix1 matrix; //0x0104
	char pad_0144[64]; //0x0144
}; //Size: 0x0184




class PartIds
{
public:
	int32_t PartsArray[150];
}; //Size: 0x0108

class Type
{
public:
	char* TypeName; //0x0000
	char pad_0008[128]; //0x0008
}; //Size: 0x0088

class PartName
{
public:
	char pad_0000[372]; //0x0000
	char Name[50]; //0x0174
	char pad_01A6[768]; //0x01A6
}; //Size: 0x04A6



class BaseEntity;
BaseEntity* GetParts(int index);
class BaseEntity
{
public:
	BoneParts* GetBoneByID(int bone)
	{
		typedef BoneParts*(__fastcall* Orig)(__int64, unsigned int);
		static Orig _GetBone = (Orig)(Offsets::FUNC_GETBONE);
		return _GetBone((__int64)this, bone);
	}
	int CarPartsCount()
	{
		if (PartsCount >= 0 && PartsCount < 300)
		{
			return PartsCount;
		}
		else
		{
			if (PartsCount > 300)
			{
				//fprintf(f, "too many parts\n"); fflush(f);
			}
			return -1;
		}
	}

	int32_t CarPartsIndex(int id)
	{
		return pPartsIds->PartsArray[id];
	}

	BaseEntity* GetCarParts(int ID)
	{
		if (ID < this->CarPartsCount() && this->CarPartsCount()!=-1)
		{
			int CarPartsIndex = this->CarPartsIndex(ID);
			BaseEntity* pParts = NULL;
			pParts = GetParts(this->CarPartsIndex(ID));
			if (pParts)
			{
				//std::cout << "yep";
				return pParts;
				//if (pParts->PartsIndex() == CarPartsIndex)
				//{
				//	return pParts->pEntity();
				//}
			}
		}

		return NULL;
	}

	char pad_0000[8]; //0x0000
	class Type* pType; //0x0008
	char pad_0010[64]; //0x0010
	int32_t Id; //0x0050
	char pad_0054[4]; //0x0054
	int32_t NetId; //0x0058
	int32_t ParentId; //0x005C
	char pad_0060[24]; //0x0060
	class PartName* pPartName; //0x0078
	char pad_0080[4]; //0x0080
	char Name[16]; //0x0084
	char pad_0094[80]; //0x0094
	float CurrentHealth; //0x00E4
	float MaxHealth; //0x00E8
	char pad_00EC[4]; //0x00EC
	class Bone* pBone; //0x00F0
	char pad_00F8[56]; //0x00F8
	int32_t TeamNum; //0x0130
	char pad_0134[44]; //0x0134
	Vec3 Min; //0x0160
	char pad_016C[4]; //0x016C
	Vec3 Max; //0x0170
	char pad_017C[2092]; //0x017C
	int32_t PartsCount; //0x09A8
	char pad_09AC[4]; //0x09AC
	class PartIds* pPartsIds; //0x09B0
	char pad_09B8[208]; //0x09B8
}; //Size: 0x0A68


class CGameLogic
{
public:
	char pad_0x0000[0x18];
	__int32 localClientNum;
	BaseEntity* GetPlayerFromIndex(int idx)
	{
		typedef __int64(__fastcall *FUNC_GetPlayerFromIndex)(__int64 a1, int a2);
		static FUNC_GetPlayerFromIndex _Get = (FUNC_GetPlayerFromIndex)(Offsets::FUNC_GetPlayerFromIndex);
		return (BaseEntity*)_Get((__int64)this, idx);
	}
	BaseEntity* GetLocalPlayer()
	{
		return GetPlayerFromIndex(localClientNum);
	}
	static CGameLogic* GetInstance()
	{
		return (CGameLogic*)(Offsets::CGAMELOGIC_ADDR);
	}
};


class EntityList;


class EntityContainer
{
public:
	
	int32_t EntityCount; //0x0000
	char pad_0004[4]; //0x0004
	class BaseEntity** BaseEntitysArray;
	//class EntityList* pEntityList; //0x0008
	char pad_0010[48]; //0x0010
	static EntityContainer* GetInstance()
	{
		return (EntityContainer*)(Offsets::ENTITYCONTAINER_ADDR);
	}
}; //Size: 0x0040


class EntityList
{
public:
	//static EntityList* GetInstance()
	//{
	//	return EntityContainer::GetInstance()->pEntityList;
	//}
	EntityList* GetAdr()
	{
		return this;
	}
	class BaseEntity** BaseEntitysArray; //0x0000
	char pad_0018[64]; //0x0018
}; //Size: 0x0058

class Aimer
{
public:
	float Yaw; //0x0000
	float Pitch; //0x0004
	char pad_0008[56]; //0x0008
	static Aimer* GetInstance()
	{
		return (Aimer*)(Offsets::AIMER_ADDR);
	}
}; //Size: 0x0040
class Camera
{
public:
	char pad_0000[48]; //0x0000
	Vec3 CamPos; //0x0030
	char pad_003C[4]; //0x003C
	Vec3 CamKrep; //0x0040
	char pad_004C[1024]; //0x004C
	static Camera* GetInstance()
	{
		return *(Camera**)(Offsets::LOCALCAMERA);
	}
}; //Size: 0x044C

Vec3 rotated(Matrix1 m, Vec3 pos, Vec3 point1)
{
	float help[4] = { point1.x,point1.y,point1.z,1.f };
	Matrix1 perem = m;
	perem.matrix[3][0] = pos.x;
	perem.matrix[3][1] = pos.y;
	perem.matrix[3][2] = pos.z;
	float help2[4] = { point1.x,point1.y,point1.z,1.f };
	help[0] = help2[0] * perem.matrix[0][0] + help2[1] * perem.matrix[1][0]
		+ help2[2] * perem.matrix[2][0] + help2[3] * perem.matrix[3][0];
	help[1] = help2[0] * perem.matrix[0][1] + help2[1] * perem.matrix[1][1]
		+ help2[2] * perem.matrix[2][1] + help2[3] * perem.matrix[3][1];
	help[2] = help2[0] * perem.matrix[0][2] + help2[1] * perem.matrix[1][2]
		+ help2[2] * perem.matrix[2][2] + help2[3] * perem.matrix[3][2];
	help[3] = help2[0] * perem.matrix[0][3] + help2[1] * perem.matrix[1][3]
		+ help2[2] * perem.matrix[2][3] + help2[3] * perem.matrix[3][3];
	Vec3 ret;
	ret.x = help[0];
	ret.y = help[1];
	ret.z = help[2];
	return ret;
}

void Box2(Vec3 * Cube, float* color)
{
	DrawDebugLine(Cube[2], Cube[3], color);
	DrawDebugLine(Cube[3], Cube[0], color);
	DrawDebugLine(Cube[4], Cube[5], color);
	DrawDebugLine(Cube[5], Cube[6], color);
	DrawDebugLine(Cube[6], Cube[7], color);
	DrawDebugLine(Cube[7], Cube[4], color);

	DrawDebugLine(Cube[4], Cube[0], color);
	DrawDebugLine(Cube[5], Cube[1], color);
	DrawDebugLine(Cube[6], Cube[2], color);
	DrawDebugLine(Cube[7], Cube[3], color);
	return;
}

void Box22(Matrix1 m, Vec3 pos, float coeff, float type, float* color)
{
	Vec3 Cube[8];
	Cube[0].x = coeff;
	Cube[0].y = (-0.5 - coeff)*type;
	Cube[0].z = coeff / type;

	Cube[1].x = -coeff;
	Cube[1].y = (-0.5 - coeff)*type;
	Cube[1].z = coeff / type;

	Cube[2].x = -coeff;
	Cube[2].y = (-0.5 - coeff)*type;
	Cube[2].z = -coeff / type;

	Cube[3].x = coeff;
	Cube[3].y = (-0.5 - coeff)*type;
	Cube[3].z = -coeff / type;


	Cube[4].x = coeff;
	Cube[4].y = (-0.5 + coeff)*type;
	Cube[4].z = coeff / type;

	Cube[5].x = -coeff;
	Cube[5].y = (-0.5 + coeff)*type;
	Cube[5].z = coeff / type;

	Cube[6].x = -coeff;
	Cube[6].y = (-0.5 + coeff)*type;
	Cube[6].z = -coeff / type;

	Cube[7].x = coeff;
	Cube[7].y = (-0.5 + coeff)*type;
	Cube[7].z = -coeff / type;
	for (int i = 0; i < 8; i++)
	{
		Cube[i] = rotated(m, pos, Cube[i]);
	}
	DrawDebugLine(Cube[0], Cube[1], color);
	DrawDebugLine(Cube[1], Cube[2], color);
	Box2(Cube, color);
	return;
}

BaseEntity* GetParts(int index)
{ //MakeCarPartsUndamageable
	if (index > -1)
	{
		__int64 f1 = *(__int64*)(Offsets::ENTITYLIST_ADDR);
		if (IsValidPtr(f1))
		{
			//BaseEntity *g = (BaseEntity *)(f1 + 16i64 * (index & 0xFFF) + 163880);
			if (IsValidPtr(*(BaseEntity **)(f1 + 16i64 * (index & 0xFFF) + 163880)))
			{
				return *(BaseEntity **)(f1 + 16i64 * (index & 0xFFF) + 163880);
			}
			else
			{
				//fprintf(f, "g not valid\n"); fflush(f);
				return NULL;
			}
		}
		else
		{
			//fprintf(f, "f1 not valid\n"); fflush(f);
			return NULL;
		}
		
	}

	return NULL;
}


Vec3 Weapons[400];
LockClass Weapons3D[400];
int weaponsCount = 0;
int TargetLockValue = 0;


enum DumpType{MyWeap,All};
Vector3 MyWeapon;


bool GetPredict(Vec3 Me1, Vec3 Targ1, Vector3 TargetSpeed, float bulletSpeed, Vec3 & out);

void Dumped(DumpType type,Matrix1 m,BaseEntity * pPlayer,BaseEntity* pLocalPlayer)
{
	//std::cout << "in\n";
	for (int a = 0; a < pPlayer->CarPartsCount(); a++)
	{
		BaseEntity* CarPart = pPlayer->GetCarParts(a);
		if (IsValidPtr(CarPart))
		{
			if (IsValidPtr(CarPart->pType) && IsValidPtr(CarPart->pPartName) &&IsValidPtr(CarPart->GetBoneByID(0)))
			{
				if (strstr("AutoGun", CarPart->pType->TypeName) || strstr("ExplodingSpear", CarPart->pType->TypeName) || strstr("ChainSaw", CarPart->pType->TypeName)
					|| strstr("Turret", CarPart->pType->TypeName) || strstr("MultiTargetHomingMissileLauncher", CarPart->pType->TypeName) || strstr("HomingMissileLauncher", CarPart->pType->TypeName)
					|| strstr("SingleShotWeapon", CarPart->pType->TypeName) || strstr("LaserGun", CarPart->pType->TypeName) || strstr("LightningGun", CarPart->pType->TypeName)
					|| strstr("SmartMachineGun", CarPart->pType->TypeName) || strstr("MiniGun", CarPart->pType->TypeName) || strstr("ArtilleryWeapon", CarPart->pType->TypeName)
					|| strstr("DeployerSkill", CarPart->pType->TypeName) || strstr("Quadrocopter", CarPart->pType->TypeName) || strstr("ShieldSkill", CarPart->pType->TypeName))
				{
					Vec3 coord1 = CarPart->GetBoneByID(0)->Coord;
					coord1.y += Features::Yoffset;
					Vec3 tmp = coord1;
					//std::cout << "Coord: x " << coord1.x << " y " << coord1.y << " z " << coord1.z << "\n";
					//std::cout << "tmp: x " << tmp.x << " y " << tmp.y << " z " << tmp.z << "\n";
					//std:: cout << std::hex << CarPart << "\n";
					if (Features::PredictWeapon)
					{
						//Vec3 out;
						GetPredict(pLocalPlayer->GetBoneByID(0)->Coord, coord1, pPlayer->pBone->pSpeed->Velocity, (float)Features::PredictSpeed, coord1);
						/*Vec3 im1, im2;
						WorldToScreen(coord1, im1);
						WorldToScreen(tmp, im2);
						DrawLine(ImVec2(im2.x, im2.y), ImVec2(im1.x, im1.y), Features::OtherColor, 1.f);*/
						DrawDebugLine(tmp, coord1, Features::OtherColor);
					}
					if (type == MyWeap)
					{
						//std::cout <<"weap adr" <<CarPart << "\n";
						MyWeapon = (Vector3)coord1;
						break;
					}

					if (WorldToScreen(coord1, Weapons[weaponsCount]) && !strstr("ShieldSkill", CarPart->pType->TypeName) && !(coord1 == Vec3(0, Features::Yoffset, 0)))
					{
						//fprintf(f, "Name - %s %d %f %f %f\n", pPlayer->Name,a,Weapons3D[weaponsCount].coord.x, Weapons3D[weaponsCount].coord.y, Weapons3D[weaponsCount].coord.z); fflush(f);
						Weapons3D[weaponsCount].coord = coord1;
						Weapons3D[weaponsCount].Pid = CarPart->ParentId;
						weaponsCount++;
					}

					
					if (strstr("LightningGun", CarPart->pType->TypeName))
					{
						Vec3 droneH = CarPart->GetBoneByID(0)->Coord;
						//droneH.y += 0.6;
						Box22(m, droneH, 0.5, 1.2,  Features::TeslaColor);// 0x48D1CC);
					}
					else if (strstr("DeployerSkill", CarPart->pType->TypeName))
					{
						Vec3 droneH = CarPart->GetBoneByID(0)->Coord;
						//droneH.z += 0.6;
						droneH.y -= 0.5;
						Box22(m, droneH, 0.5, 1.2,  Features::DronesColor);//0x0000FF);
					}
					else if (strstr("ArtilleryWeapon", CarPart->pType->TypeName))
					{
						Box22(m, CarPart->GetBoneByID(0)->Coord, 0.8, 0.8,  Features::MandrakeColor);//0x8B4513);
					}
					else if (strstr("MiniGun", CarPart->pType->TypeName))
					{
						Box22(m, CarPart->GetBoneByID(0)->Coord, 0.5, 1.2,  Features::MinigunColor);//0xB22222);
					}
					else if (strstr("ExplodingSpear", CarPart->pType->TypeName) || strstr("ChainSaw", CarPart->pType->TypeName))
					{
						Box22(m, CarPart->GetBoneByID(0)->Coord, 0.5, 1.2,  Features::MeleeColor);//0x6A5ACD);
					}
					else if (strstr("SingleShotWeapon", CarPart->pType->TypeName))
					{
						//cout << "\n*************" << (char*)((*(DWORD *)(v7 + 88))+ 368);
						if ((strstr("CarPart_Gun_SniperCrossbow", CarPart->pPartName->Name)))
						{
							Box22(m, CarPart->GetBoneByID(0)->Coord, 0.7, 1,  Features::ScorpionColor);//0xFFFFFF);
						}
						else
						{
							Box22(m, CarPart->GetBoneByID(0)->Coord, 0.7, 1,  Features::BigGunsColor);//0xFFD700);
						}
					}
					else if (strstr("ShieldSkill", CarPart->pType->TypeName))
					{
						Box22(m, CarPart->GetBoneByID(0)->Coord, 0.4, 1.1,  Features::AegisColor);//0xFF69B4);
					}
					else
					{
						if ((strstr("CarPart_Gun_Shotgun_legend", CarPart->pPartName->Name)))
						{
							Box22(m, CarPart->GetBoneByID(0)->Coord, 0.7, 1,  Features::HammerColor);//0x808000);
						}
						else
						{


							Box22(m, CarPart->GetBoneByID(0)->Coord, 0.4, 1.1,  Features::OtherColor);//0x32CD32);
						}
					}
					
				}
			}
			else
			{
				//fprintf(f, "not valid part type or name\n"); fflush(f);
			}
		}
		
	}
}





void DrawTextIm(float x, float y, float* color, const char *Format, ...)
{
	char Buffer[1024] = { '\0' };
	va_list va_alist;
	va_start(va_alist, Format);
	vsprintf_s(Buffer, Format, va_alist);
	va_end(va_alist);
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->PushClipRectFullScreen();
	draw_list->AddText(ImVec2(x, y), ImGui::GetColorU32(ImVec4(color[0], color[1], color[2], color[3])), Buffer);
}


float GetNeeded(float a, float b, float c, float d, float e, float f, float h, float i, bool Mode) // 1 большее 0 меньшее
{
	float mass[8] = { a,b,c,d ,e,f,h,i };
	if (Mode)
	{
		float max = a;
		for (int g = 0; g < 8; g++)
		{
			if (mass[g] > max)
			{
				max = mass[g];
			}
		}
		return max;
	}
	else
	{
		float min = a;
		for (int g = 0; g < 8; g++)
		{
			if (mass[g] < min)
			{
				min = mass[g];
			}
		}
		return min;
	}
}




void Box2D(BaseEntity * m,float *color)
{
	Vec3 BotLeft;
	Vec3 TopRight;
	Vec3 BotRight;
	Vec3 TopLeft;
	Vec3 BotLeft2;
	Vec3 TopRight2;
	Vec3 BotRight2;
	Vec3 TopLeft2;

    
	Vec3 help1 = rotated(m->GetBoneByID(0)->matrix, m->GetBoneByID(0)->Coord + Vec3(0, 1, 0), m->Min);
	Vec3 help2 = rotated(m->GetBoneByID(0)->matrix, m->GetBoneByID(0)->Coord + Vec3(0, 1, 0), m->Max);
	Vec3 help3 = rotated(m->GetBoneByID(0)->matrix, m->GetBoneByID(0)->Coord + Vec3(0, 1, 0), Vec3(m->Max.x, m->Min.y, m->Min.z));
	Vec3 help4 = rotated(m->GetBoneByID(0)->matrix, m->GetBoneByID(0)->Coord + Vec3(0, 1, 0), Vec3(m->Min.x, m->Max.y, m->Max.z));

	Vec3 help5 = rotated(m->GetBoneByID(0)->matrix, m->GetBoneByID(0)->Coord + Vec3(0, 1, 0), Vec3(m->Min.x, m->Min.y, m->Max.z));
	Vec3 help6 = rotated(m->GetBoneByID(0)->matrix, m->GetBoneByID(0)->Coord + Vec3(0, 1, 0), Vec3(m->Min.x, m->Max.y, m->Min.z));
	Vec3 help7 = rotated(m->GetBoneByID(0)->matrix, m->GetBoneByID(0)->Coord + Vec3(0, 1, 0), Vec3(m->Max.x, m->Min.y, m->Max.z));
	Vec3 help8 = rotated(m->GetBoneByID(0)->matrix, m->GetBoneByID(0)->Coord + Vec3(0, 1, 0), Vec3(m->Max.x, m->Max.y, m->Min.z));


	WorldToScreen(help1, BotLeft);
	WorldToScreen(help2, TopRight);
	WorldToScreen(help3, BotRight);
	WorldToScreen(help4, TopLeft);

	WorldToScreen(help5, BotLeft2);
	WorldToScreen(help6, TopRight2);
	WorldToScreen(help7, BotRight2);
	WorldToScreen(help8, TopLeft2);


	float maxY = GetNeeded(TopLeft.y, TopRight.y, BotLeft.y, BotRight.y, TopLeft2.y, TopRight2.y, BotLeft2.y, BotRight2.y, 1);
	float minY = GetNeeded(TopLeft.y, TopRight.y, BotLeft.y, BotRight.y, TopLeft2.y, TopRight2.y, BotLeft2.y, BotRight2.y, 0);
	float maxX = GetNeeded(TopLeft.x, TopRight.x, BotLeft.x, BotRight.x, TopLeft2.x, TopRight2.x, BotLeft2.x, BotRight2.x, 1);
	float minX = GetNeeded(TopLeft.x, TopRight.x, BotLeft.x, BotRight.x, TopLeft2.x, TopRight2.x, BotLeft2.x, BotRight2.x, 0);
	/*cout << "-----BotLeft-----" << BotLeft.x << " " << BotLeft.y << "\n";
	cout << "----- TopRight-----" << TopRight.x << " " << TopRight.y << "\n";
	cout << "-----m->Min()-----" << m->Min().x << " " << m->Min().y << "\n";
	cout << "----- m->Max()-----" << m->Max().x << " " << m->Max().y << "\n";*/

	//DrawDebugLine(help1, help2, 0xDC14ff);
	//DrawDebugLine(help3, help4, 0xDC14ff);
	

	
	DrawLine(ImVec2(minX, maxY), ImVec2(minX, minY),color,1.f);
	DrawLine(ImVec2(minX, maxY), ImVec2(maxX, maxY), color, 1.f);
	DrawLine(ImVec2(maxX, minY), ImVec2(minX, minY), color, 1.f);
	DrawLine(ImVec2(maxX, minY), ImVec2(maxX, maxY), color, 1.f);
}
