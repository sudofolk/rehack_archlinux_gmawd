#include <cstring>
#include <iostream>
#include <dlfcn.h>

#include "../sdk/globals.hpp"
#include "../util/vmt.h"

typedef ClientModeShared* (*getClientModeFunc)();

uintptr_t Interfaces::GetInterface(const char* filename, const char* version, bool exact = false){
	void* library = dlopen(filename, RTLD_NOLOAD | RTLD_NOW | RTLD_LOCAL);

	if (!library)
		return 0;

	void* interfaces_sym = dlsym(library, "CreateInterface");
	void* interfaces_list_sym = dlsym(library, "s_pInterfaceRegs");

	////std::cout << std::hex << interfaces_list_sym << std::endl;

	if (!interfaces_sym)
	{
		dlclose(library);
		return 0;
	}

	dlclose(library);

	InterfaceReg* interface_list = *reinterpret_cast<InterfaceReg**>(interfaces_list_sym);

    uintptr_t* Interface;
	for (InterfaceReg* current = interface_list; current != nullptr; current = current->m_pNext){
        if (strstr(current->m_pName, version) && (strlen(current->m_pName) - strlen(version)) < 4) {
			Interface = current->m_CreateFn();

			printf("Found interface (%s => 0x%X)\n", current->m_pName, Interface);

			break;
		} 

    	//printf("%s => 0x%X \n", current->m_pName, current->m_CreateFn());
	}

	return (uintptr_t)Interface;
}

void Interfaces::DumpInterfaces(const char* filename){
	std::cout << "Dumping interfaces from " << filename << std::endl;

	void* library = dlopen(filename, RTLD_NOLOAD | RTLD_NOW | RTLD_LOCAL);

	if (!library){
        return nullptr;
    }

	void* interfaces_sym = dlsym(library, "CreateInterface");
	void* interfaces_list_sym = dlsym(library, "s_pInterfaceRegs");

	if (!interfaces_sym || !interfaces_list_sym){
		dlclose(library);
		return;
	}

	dlclose(library);

	InterfaceReg* interface_list = *reinterpret_cast<InterfaceReg**>(interfaces_list_sym);

	for (InterfaceReg* current = interface_list; current != nullptr; current = current->m_pNext){
    	std::cout << current->m_pName << " => " << std::hex << current->m_CreateFn() << std::endl;
	}
}

Interfaces::Interfaces(){
    std::cout << "Interface initialization begin.." << std::endl;

	this->pCHLClient = reinterpret_cast<CHLClient*>(GetInterface("/home/capias/gmod/GarrysMod/bin/linux64/client_client.so", "VClient"));
	this->pEngineClient = reinterpret_cast<CEngineClient*>(GetInterface("/home/capias/.local/share/Steam/steamapps/common/GarrysMod/bin/linux64/engine_client.so", "VEngineClient"));
	this->pClientEntityList = reinterpret_cast<CClientEntityList*>(GetInterface("/home/capias/gmod/GarrysMod/bin/linux64/client_client.so", "VClientEntityList"));
	this->pLuaShared = reinterpret_cast<lua::ILuaShared*>(GetInterface("/home/capias/.local/share/Steam/steamapps/common/GarrysMod/bin/linux64/lua_shared_client.so", "LUASHARED003"));
	this->pInputSystem = reinterpret_cast<CInputSystem*>(GetInterface("/home/capias/.local/share/Steam/steamapps/common/GarrysMod/bin/linux64/inputsystem_client.so", "InputSystemVersion"));
	this->pEngineTrace = reinterpret_cast<IEngineTrace*>(GetInterface("/home/capias/.local/share/Steam/steamapps/common/GarrysMod/bin/linux64/engine_client.so", "EngineTraceClient"));
	this->pEngineVgui = reinterpret_cast<void*>(GetInterface("/home/capias/.local/share/Steam/steamapps/common/GarrysMod/bin/linux64/engine_client.so", "VEngineVGui"));
	this->pGameMovement = reinterpret_cast<CGameMovement*>(GetInterface("/home/capias/.local/share/Steam/steamapps/common/GarrysMod/bin/linux64/client_client.so", "GameMovement"));
	this->pPrediction = reinterpret_cast<CPrediction*>(GetInterface("/home/capias/.local/share/Steam/steamapps/common/GarrysMod/bin/linux64/client_client.so", "VClientPrediction"));

	// all these offsets and pattersns might be fucked up now... should have taught u How to Do it with Screenshots
	// Hindsight 2020 What Ever
	
	uintptr_t HudProcessInput = reinterpret_cast<uintptr_t>(VMT::GetVTable(this->pCHLClient)[10]);
	uint32_t offset = *reinterpret_cast<uint32_t*>(HudProcessInput + 3); 
	this->pClientModeShared = **reinterpret_cast<ClientModeShared***>(HudProcessInput + offset + 7);
	printf("Found interface (ClientModeShared => 0x%X)\n", this->pClientModeShared);

	uintptr_t InitAddr = reinterpret_cast<uintptr_t>(VMT::GetVTable(this->pCHLClient)[0]); // CHLCLient::Init()
	uint32_t offset2 = *reinterpret_cast<uint32_t*>(InitAddr + 0x87);
	this->pGlobals = *reinterpret_cast<CGlobalVarsBase**>(InitAddr + offset2 + 0x8B);
	printf("Found Pointer (pGlobalVars => 0x%X)\n", this->pGlobals);

	uintptr_t RandomSeed = reinterpret_cast<uintptr_t>(dlsym(dlopen("/home/capias/.local/share/Steam/steamapps/common/GarrysMod/bin/linux64/libvstdlib_client.so", RTLD_NOLOAD | RTLD_NOW | RTLD_LOCAL), "RandomSeed"));
	uint32_t offset3 = *reinterpret_cast<uint32_t*>(RandomSeed + 0x5);
	this->pUniformRandomStream = *reinterpret_cast<CUniformRandomStream**>(RandomSeed + offset3 + 0x9);
	printf("Found Interface (IUniformRandomStream => 0x%X)\n", this->pUniformRandomStream);

	auto setPredSeedAddr = PatternScan::findFirstInModule("/home/capias/gmod/GarrysMod/bin/linux64/client_client.so", "55 48 85 ff 48 89 e5 ? ? 8b ? ? 48 ? ?");
	uint32_t offset4 = *reinterpret_cast<uint32_t*>(setPredSeedAddr + 0xF);
	this->pPredictionSeed = *reinterpret_cast<int**>(setPredSeedAddr + offset4 + 0x13);
	printf("Found Pointer (PredictionSeed => 0x%X)\n", this->pPredictionSeed);

	std::cout << std::hex << this->pPredictionSeed << std::endl;

	auto playSwimSoundAddr = PatternScan::findFirstInModule("/home/capias/gmod/GarrysMod/bin/linux64/client_client.so", "48 8b ? ? ? ? ? 55 48 8b ? ? 48 89 e5 5d");
	uint32_t offset5 = *reinterpret_cast<uint32_t*>(playSwimSoundAddr + 0x3);
	this->pMoveHelper = **reinterpret_cast<IMoveHelper***>(playSwimSoundAddr + offset5 + 0x7);
	printf("Found Interface (IMoveHelper => 0x%X)\n", this->pMoveHelper);

	this->isInitialized = true;

	std::cout << "Interface initialization end." << std::endl;
}
