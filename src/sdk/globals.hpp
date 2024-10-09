#include <map>
#include <mutex>
#include <stdint.h>
#include <math.h>
#include <vector>
#include <string>
#include <iostream>

typedef unsigned int UINT;
typedef long LONG;

typedef void* PVOID;

#define FORCEINLINE inline __attribute__ ((always_inline))

#ifndef GLOBALS

#define GLOBALS

#define abstract_class class
#define FORCEINLINE inline __attribute__ ((always_inline))

class bf_write;
class bf_read;
class IPVSNotify;

typedef void* HANDLE;
typedef unsigned int CRC32_t;
typedef struct player_info_s;
typedef unsigned int UINT;
typedef long LONG;
typedef void* PVOID;

typedef void* (*CreateInterfaceFn)(const char *pName, int *pReturnCode);
typedef void* (*InstantiateInterfaceFn)();

#include "tier0/platform.h"


#include "tier0/Color.h"
#include "tier0/Vector.h"
#include "tier0/cutlvector.h"
#include "tier1/KeyValues.h"

#include "../util/util.h"


#include "../sdk/client/ClientClass.h"
#include "../sdk/client/ClientModeShared.h"
#include "../sdk/client/CClientEntityList.h"

//#include "../sdk/engine/iclientrenderable.h"
#include "../sdk/client/CHLClient.h"
#include "../sdk/engine/CEngineClient.h"
#include "../sdk/engine/CVRenderView.h"
#include "../sdk/client/CInputSystem.h"
#include "../sdk/engine/CModelRender.h"
#include "../sdk/engine/CMaterialSystem.h"
#include "../sdk/client/CViewRender.h"
#include "../sdk/client/CUniformRandomStream.h"
#include "../sdk/engine/CModelInfo.h"
#include "../sdk/client/ClientModeShared.h"

#include "../sdk/engine/CModelInfo.h"
#include "../sdk/engine/CIVDebugOverlay.h"
#include "../sdk/engine/CGameEventManager.h"
#include "../sdk/vgui/VPanelWrapper.h"
#include "../sdk/vphysics/CPhysicsSurfaceProps.h"
#include "../sdk/vguimatsurface/CMatSystemSurface.h"
#include "../sdk/client/IPrediction.h"
#include "../sdk/client/IGameMovement.h"


#include "public/networkvar.h"
#include "zip/ihandleentity.h"
#include "zip/iclientunknown.h"
#include "zip/iclientthinkable.h"
#include "zip/iclientrenderable.h"
#include "zip/iclientnetworkable.h"
#include "zip/icliententity.h"
#include "../sdk/client/C_BasePlayer.h"


#include "../sdk/engine/gametrace.h"
#include "../sdk/engine/IEngineTrace.h"
#include "../sdk/client/CMoveHelper.h"

#include "../sdk/lua_shared/CLuaInterface.h"

#include "../sdk/mathlib/mathlib.h"



#include "../interfaces/interfaces.h"
#include "../hooks/hooks.h"
#include "../esp/esp.h"

#include "SDL2/SDL.h"

#include "../imgui/GL/gl3w.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_sdl.h"
#include "../imgui/imgui_impl_opengl3.h"

#include "../util/config.h"
#include "../util/vmt.h"
#include "../util/sdl.h"
#include "../menu/menu.h"
#include "../menu/roboto.h"
#include "../util/globals.h"



#endif

/*

ACQUIRE INPUT SYSTEM POINTER,


*/