#include <map>
#include <mutex>
#include <stdint.h>

#ifndef Interfaces_H
#define Interfaces_H

typedef void* (*CreateInterfaceFn)(const char *pName, int *pReturnCode);
typedef void* (*InstantiateInterfaceFn)();

class InterfaceReg{
public:
	InterfaceReg(InstantiateInterfaceFn fn, const char *pName);

public:
	InstantiateInterfaceFn	m_CreateFn;
	const char				*m_pName;

	InterfaceReg			*m_pNext;
	static InterfaceReg		*s_pInterfaceRegs;
};

class Interfaces{
public:
	Interfaces();

	bool  			   isInitialized = false;

	uintptr_t 			   GetInterface(const char* filename, const char* version, bool exact = false);
	void  			   DumpInterfaces(const char* filename);

    CEngineClient*     pEngineClient;
    CHLClient* 		   pCHLClient;
	CClientEntityList* pClientEntityList;
	ClientModeShared*  pClientModeShared;
	CUniformRandomStream* pUniformRandomStream;
	CInputSystem* pInputSystem;
	CGlobalVarsBase* pGlobals;
	lua::ILuaShared* pLuaShared;
	lua::CLuaInterface* pLuaInterface;
	IEngineTrace* pEngineTrace;
	IMoveHelper* pMoveHelper;
	CGameMovement* pGameMovement;
	CPrediction* pPrediction;
	void* pEngineVgui;
	int* pPredictionSeed;


};

extern Interfaces* pInterfaces;

#endif