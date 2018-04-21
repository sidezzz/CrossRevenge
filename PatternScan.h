#pragma once
#pragma once
class Offsets
{
public:
	static uintptr_t CGAMELOGIC_ADDR;
	static uintptr_t FUNC_GetPlayerFromIndex;
	static uintptr_t FUNC_DrawDebugLine;
	static uintptr_t LOCALCAMERA;
	static uintptr_t ENTITYCONTAINER_ADDR;
	static uintptr_t FUNC_GETBONE;
	static uintptr_t CRENDER_ADDR;
	static uintptr_t AIMER_ADDR;
	static uintptr_t ENTITYLIST_ADDR;
	static uintptr_t CVARARRAY;
};
uintptr_t Offsets::CGAMELOGIC_ADDR = 0;
uintptr_t Offsets::FUNC_GetPlayerFromIndex = 0;
uintptr_t Offsets::FUNC_DrawDebugLine = 0;
uintptr_t Offsets::LOCALCAMERA = 0;
uintptr_t Offsets::ENTITYCONTAINER_ADDR = 0;
uintptr_t Offsets::FUNC_GETBONE = 0;
uintptr_t Offsets::CRENDER_ADDR = 0;
uintptr_t Offsets::AIMER_ADDR = 0;
uintptr_t Offsets::ENTITYLIST_ADDR = 0;
uintptr_t Offsets::CVARARRAY = 0;


uintptr_t Resolve(uintptr_t address, uint32_t offset)
{
	if (address)
	{
		address += offset;

		if (*reinterpret_cast<byte*>(address) == 0xE9 || *reinterpret_cast<byte*>(address) == 0xE8)
		{
			auto displacement = *reinterpret_cast<uint32_t*>(address + 1);
			auto ret = address + displacement + 5;

			if (displacement & 0x80000000)
				ret -= 0x100000000;

			return ret;
		}
		else if (*reinterpret_cast<byte*>(address + 1) == 0x05)
		{
			auto displacement = *reinterpret_cast<uint32_t*>(address + 2);
			auto ret = address + displacement + 6;

			if (displacement & 0x80000000)
				ret -= 0x100000000;

			return ret;
		}
		else
		{
			auto displacement = *reinterpret_cast<uint32_t*>(address + 3);
			auto ret = address + displacement + 3 + sizeof(uint32_t);

			if (displacement & 0x80000000)
				ret -= 0x100000000;

			return ret;
		}
	}
	else
	{
		return 0;
	}
}


uintptr_t FindPattern(const char* module, byte* pattern, const char* mask)
{
	MODULEINFO moduleInfo;

	GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(nullptr), &moduleInfo, sizeof(MODULEINFO));

	if (module)
		GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(module), &moduleInfo, sizeof(MODULEINFO));

	auto pStartAddr = reinterpret_cast< byte* >(moduleInfo.lpBaseOfDll);
	std::size_t ScanSize = moduleInfo.SizeOfImage;
	//std::cout << ScanSize << "\n";

	for (auto i = 0; i < ScanSize; i++)
	{
		auto pat = pattern;
		auto mask_tmp = mask;
		for (auto mem_it = 0;; mem_it++, mask_tmp++, pat++)
		{
			if (i + mem_it < ScanSize)
			{

				if (!*mask_tmp)
					return reinterpret_cast<uintptr_t>(pStartAddr + i);

				if (*(pStartAddr + i + mem_it) != *pat && *mask_tmp == 'x')
					break;
			}
			else
			{
				return 0;
			}
		}
	}

	return 0;
}