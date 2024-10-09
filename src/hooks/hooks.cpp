#include <iostream>

#include "../sdk/globals.hpp"

#include "../sdk/mathlib/mathlib.h"
#include "../sdk/client/usercmd.h"
#include "../sdk/tier1/checksum_md5.h"

enum PaintMode_t{
    PAINT_UIPANELS = (1 << 0),
    PAINT_INGAMEPANELS = (1 << 1),
    PAINT_CURSOR = (1 << 2)
};

int LuaPrint(lua::CLuaInterface* LUA){
	pInterfaces->pLuaInterface->PushSpecial(lua::special::glob); // Fetch the global table
		pInterfaces->pLuaInterface->GetField(-1, "print"); // Fetch the print function
		pInterfaces->pLuaInterface->PushString(pInterfaces->pLuaInterface->GetString(1)); // Push the string argument
		pInterfaces->pLuaInterface->Call(1, 0); // Call the function with 1 argument and no return value
	pInterfaces->pLuaInterface->Pop(); // Pop global table

    return 0;
}

void* findPlayerThatRayHits(Vector start, Vector end) {
    Ray_t ray;
    
    TraceFilterSimple filter = TraceFilterSimple(Globals::LocalPlayer);
    trace_k Trace;
    ray.Init(start, end);

    pInterfaces->pEngineTrace->TraceRay(ray, MASK_SHOT, &filter, &Trace);

    if(Trace.fraction >= 0.98f || (Trace.hitgroup > 0 && Trace.hitgroup < 8)){
        return Trace.m_pEnt;
    }

    return nullptr;
}

void doTriggerBot(CUserCmd* cmd, C_BaseCombatWeapon* gun){
    if(!gun)
        return;

    Vector eyepos = (Globals::LocalPlayer->eyepos());
    C_BasePlayer* ent = findPlayerThatRayHits(eyepos, eyepos + (cmd->viewangles.toVector() * 69696.f));

    if(!ent)
        return;

    static bool shotLastTick = false;

    if(ent->health() > 0){
        if(!shotLastTick){
            cmd->buttons |= IN_ATTACK;

            shotLastTick = true;
        }else{
            shotLastTick = false;
        }
    }
}
float m_flOldCurtime;
float m_flOldFrametime;
CMoveData moveData;
bool inPrediction = false;
void doNoSpread(CUserCmd* cmd, C_BaseCombatWeapon* gun){
    if(!(cmd->buttons & IN_ATTACK))
        return;

    uint uiSeed = MD5_PseudoRandom(cmd->command_number) & 0x7FFFFFFF;
    pInterfaces->pUniformRandomStream->SetSeed(uiSeed & 255);
    
    //std::cout << gun->uses_lua() << std::endl;
    
    QAngle qAngs = cmd->viewangles;
    Vector vSpread = VECTOR_CONE_5DEGREES;

    float x = pInterfaces->pUniformRandomStream->RandomFloat(-0.5, 0.5) + pInterfaces->pUniformRandomStream->RandomFloat(-0.5, 0.5);
    float y = pInterfaces->pUniformRandomStream->RandomFloat(-0.5, 0.5) + pInterfaces->pUniformRandomStream->RandomFloat(-0.5, 0.5);

    QAngle spreadAngles;
    Vector forward, right, up;

    AngleVectors(qAngs, &forward, &right, &up);

    Vector vecDir = forward + (x * vSpread.x * right) + (y * vSpread.y * up);
    VectorAngles(vecDir, spreadAngles);

    spreadAngles -= qAngs;

    cmd->viewangles -= spreadAngles;
}

bool CanSee(void* target, Vector start, Vector end) {
    Ray_t ray;
    
    TraceFilterSimple filter = TraceFilterSimple(Globals::LocalPlayer);
    trace_k Trace;
    ray.Init(start, end);

    pInterfaces->pEngineTrace->TraceRay(ray, MASK_SHOT, &filter, &Trace);

    //std::cout << "Target Eye Positions : " << start.x << " " << start.y << " " << start.z << std::endl;
    //std::cout << "My Eye Positions : " << end.x << " " << end.y << " " << end.z << std::endl;

    Globals::startPos = start;
    Globals::endPos = end;
    Globals::setposs = true;

    if(Trace.m_pEnt == target || Trace.fraction >= 0.98f){
        return true;
    }

    return false;
}

int GetClosestToCrosshair(){
    float lowestDistance = FLT_MAX;
	float fovDistance = FLT_MAX;
    int pTarget = 0;

    for(auto i : Globals::playerList){
        C_BasePlayer* pPly = pInterfaces->pClientEntityList->GetClientEntity(i.first);

        if(!pPly)
            continue;

        if(!(pPly->health() > 0) || pPly->GetClientNetworkable()->IsDormant())
            continue;
        
        Vector targetPos = (pPly->eyepos());
        Vector myPos = (Globals::LocalPlayer->eyepos());

        if(!CanSee(pPly, myPos, targetPos))
            continue;
        
        Vector screenPos;
        screenPos.z = 0;

        if (!worldToScreen(targetPos, screenPos))
            continue;

        int DistanceToCenter = Vector(Globals::screenSizeX / 2, Globals::screenSizeY / 2, 0).DistTo(screenPos);

        if (DistanceToCenter < fovDistance)
            fovDistance = DistanceToCenter;
        else
            continue;

        if(pPly->health() > 0){
            //std::cout << std::dec << pPly->health() << std::endl;
            pTarget = i.first;
        }
    }

    return pTarget;
}

void doAimbot(CUserCmd* cmd, C_BaseCombatWeapon* wep){
    if(!wep)
        return;

    int plyIdx = GetClosestToCrosshair();
    C_BasePlayer* target = pInterfaces->pClientEntityList->GetClientEntity(plyIdx);

    if(!target || !target->health())
        return;

    Vector entPos;

    if(Globals::playerList.find(plyIdx)->second.backtracks.size() > 2){
        std::cout << "using backtrack data" << std::endl;

        entPos = Globals::playerList[plyIdx].backtracks.at(2).headpos;
    }else{
        entPos = target->GetBonePosition(6, pInterfaces->pGlobals->curtime);
    }

    if(entPos == Vector(0,0,0))
        return;

/* (Globals::LocalPlayer->getVelocity() * pInterfaces->pGlobals->interval_per_tick*/
    auto mypos = (Globals::LocalPlayer->eyepos() );
    auto calc = (((entPos) - (mypos))).toAngle();

    //cmd->viewangles = calc;

    pInterfaces->pEngineClient->SetViewAngles(calc);
}


void StartPrediction(CUserCmd* cmd){
    inPrediction = true;
    if (Globals::LocalPlayer) {
        *pInterfaces->pPredictionSeed = (MD5_PseudoRandom(cmd->command_number) & 0x7FFFFFFF) & 255;

        m_flOldCurtime = pInterfaces->pGlobals->curtime;
        m_flOldFrametime = pInterfaces->pGlobals->frametime;

        pInterfaces->pGlobals->curtime = Globals::LocalPlayer->getTickBase() * pInterfaces->pGlobals->interval_per_tick;
        pInterfaces->pGlobals->frametime = pInterfaces->pGlobals->interval_per_tick;

        pInterfaces->pGameMovement->StartTrackPredictionErrors(Globals::LocalPlayer);
        memset((void*)&moveData, 0, sizeof(moveData));

        //pInterfaces->pMoveHelper->SetHost(Globals::LocalPlayer);
        pInterfaces->pPrediction->SetupMove(Globals::LocalPlayer, cmd,  pInterfaces->pMoveHelper, &moveData);
        pInterfaces->pGameMovement->ProcessMovement(Globals::LocalPlayer, &moveData);
        pInterfaces->pPrediction->FinishMove(Globals::LocalPlayer, cmd, &moveData);
    }
}

void EndPrediction(CUserCmd* cmd){
    if (Globals::LocalPlayer) {
        pInterfaces->pGameMovement->FinishTrackPredictionErrors(Globals::LocalPlayer);
        //pInterfaces->pMoveHelper->SetHost(0);

        *pInterfaces->pPredictionSeed = -1;

        pInterfaces->pGlobals->curtime = m_flOldCurtime;
        pInterfaces->pGlobals->frametime = m_flOldFrametime;
    }
    inPrediction = false;
}

void SaveBacktracks(CUserCmd* cmd){
    for(auto i : Globals::playerList){

        if(i.second.pPlayer->health() <=0 || i.second.pPlayer->GetClientNetworkable()->IsDormant())
            continue;

        backtrackData pData;

        pData.headpos = i.second.pPlayer->GetBonePosition(6, 0);
        pData.ticknum = cmd->tick_count;
        

        if(Globals::playerList[i.first].backtracks.size() >= 40)
            Globals::playerList[i.first].backtracks.pop_back();

        Globals::playerList[i.first].backtracks.insert(Globals::playerList[i.first].backtracks.begin(), pData);
    } 
}

typedef bool (__thiscall *CreateMoveFn)(void*, float flInputSampleTime, CUserCmd *cmd);
CreateMoveFn oCreateMove;
bool __fastcall hCreateMove(void* _this, float flInputSampleTime, CUserCmd *cmd ){

    if(!cmd->command_number)
        return oCreateMove(_this, flInputSampleTime, cmd);

    if(Globals::LocalPlayer && pInterfaces->pEngineClient->IsInGame() && pInterfaces->pEngineClient->IsConnected()){
        int flagsbackup = Globals::LocalPlayer->getFlags();

        StartPrediction(cmd);

        SaveBacktracks(cmd);

        if(pInterfaces->pInputSystem->IsButtonDown(KEY_LALT)){
            // findAimbotTarget()
            doAimbot(cmd, reinterpret_cast<C_BaseCombatWeapon*>(Globals::LocalPlayer + 0x2404));
            doTriggerBot(cmd, reinterpret_cast<C_BaseCombatWeapon*>(Globals::LocalPlayer + 0x2404));
            
        }

        doNoSpread(cmd, reinterpret_cast<C_BaseCombatWeapon*>(Globals::LocalPlayer + 0x2404));

        EndPrediction(cmd);

        if(pInterfaces->pInputSystem->IsButtonDown(KEY_SPACE)){
            if(!(flagsbackup & FL_ONGROUND)){
                cmd->buttons &= ~IN_JUMP;
            }
        }        
    }

    return false;
}


typedef void (__thiscall *FrameStageNotifyFn)(CHLClient*, ClientFrameStage_t curStage);
FrameStageNotifyFn oFrameStageNotify;
void __fastcall hFrameStageNotify(CHLClient* _this, ClientFrameStage_t curStage){
    int LocalPlayerIndex = pInterfaces->pEngineClient->GetLocalPlayer();

    // no visual recoil
    //Globals::LocalPlayer->GetViewPunch() = QAngle(0, 0, 0);

    if(pInterfaces->pEngineClient->IsConnected() && pInterfaces->pEngineClient->IsInGame()){
        Globals::LocalPlayer = pInterfaces->pClientEntityList->GetClientEntity(LocalPlayerIndex);
        Globals::screenSizeX = pInterfaces->pCHLClient->GetScreenWidth();
        Globals::screenSizeY = pInterfaces->pCHLClient->GetScreenHeight();

        if(curStage == ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START){
            for(int i = 1; i < pInterfaces->pClientEntityList->GetHighestEntityIndex(); i++){
                if(i == LocalPlayerIndex)
                    continue;

                C_BasePlayer* plyObject = pInterfaces->pClientEntityList->GetClientEntity(i);

                if(!plyObject)
                    continue;

                if(!plyObject->GetClientNetworkable()->IsDormant()){
                    ClientClass* cClass = plyObject->GetClientNetworkable()->GetClientClass();
                    int classID = cClass->m_ClassID;

                    if(classID == 70){
                        if(!(plyObject->health() > 0))
                            continue;

                        pCache plyCache = pCache{plyObject, true, {}};
                    
                        if (Globals::playerList.find(i) != Globals::playerList.end()) {
                            plyCache.backtracks = Globals::playerList[i].backtracks;
                            Globals::playerList[i] = plyCache;
                        }else{
                            Globals::playerList.insert({i, plyCache});
                        }
                    }else{
                        eCache entCache = eCache{plyObject, std::string{"hi"}};

                        if (Globals::entityList.find(i) != Globals::entityList.end()) {
                            Globals::entityList[i] = entCache;
                        }else{
                            Globals::entityList.insert({i, entCache});
                        }
                    }
                }else{
                    if (Globals::playerList.find(i) != Globals::playerList.end()) {
                        Globals::playerList.erase(i);
                    }

                    if (Globals::entityList.find(i) != Globals::entityList.end()) {
                        Globals::entityList.erase(i);
                    }
                }
                
            }
        }
    }

    oFrameStageNotify(_this, curStage);
}

typedef bool (__thiscall *RunStringExFn)(lua::CLuaInterface*, char const* unk001, char const* unk002, char const* unk003, bool unk004, bool unk005, bool unk006, bool unk007);
RunStringExFn oRunStringEx;
bool __fastcall hRunStringEx(lua::CLuaInterface* _this, char const* filename, char const* path, char const* buf, bool unk004, bool unk005, bool unk006, bool unk007){
    auto ret = oRunStringEx(_this, filename, path, buf, unk004, unk005, unk006, unk007);

    std::cout << "RunStringEx: " << filename << std::endl;
    std::cout << path << std::endl;
    std::cout << buf << std::endl << std::endl;

    if(!Globals::lualoaded && pInterfaces->pLuaInterface){

        // Example C Lua function from gmod wiki, WORKS.
        pInterfaces->pLuaInterface->PushSpecial( lua::special::glob ); // Push the global table
          pInterfaces->pLuaInterface->PushCFunction( LuaPrint ); // Push our function
          pInterfaces->pLuaInterface->SetField( -2, "LuaPrint" ); // Set MyFirstFunction in lua to our C++ function
        pInterfaces->pLuaInterface->Pop(); // Pop the global table off the stack

        std::cout << "Loaded C Lua functions..." << std::endl;

        Globals::lualoaded = true;
    }

    return 1;
}

typedef lua::CLuaInterface* (*CreateLuaInterfaceFn)(lua::CLuaInterface*, int, bool);
CreateLuaInterfaceFn oCreateLuaInterface;
lua::CLuaInterface* hCreateLuaInterface(lua::CLuaInterface* _this, int stateType, bool renew){
    auto LUA = oCreateLuaInterface(_this, stateType, renew);

    std::cout << _this << "  " << stateType << "  " << renew << "  " << std::endl;

    if(stateType != 0){
        return LUA;
    }

    pInterfaces->pLuaInterface = LUA;

    if(!Globals::oRSHooked){
        oRunStringEx = VMT::hook(pInterfaces->pLuaInterface, hRunStringEx, 111);

        Globals::oRSHooked = true;
    }

    return LUA;
}


typedef void (*DestroyLuaInterfaceFn)(void*, lua::CLuaInterface*);
DestroyLuaInterfaceFn oDestroyLuaInterface;
void hDestroyLuaInterface(void* _this, lua::CLuaInterface* iface){
    Globals::playerList.clear();
    Globals::entityList.clear();

    if(Globals::oRSHooked){
        VMT::hook(pInterfaces->pLuaInterface, oRunStringEx, 111);
        oRunStringEx = nullptr;
        pInterfaces->pLuaInterface = 0;
        
        Globals::oRSHooked = false;
        Globals::lualoaded = false;
        Globals::setposs = false;
        Globals::LocalPlayer = nullptr;
    }


    oDestroyLuaInterface(_this, iface);
}


HookManager::HookManager(){
    std::cout << "Hooking functions begin.." << std::endl;

    oCreateMove = VMT::hook(pInterfaces->pClientModeShared, hCreateMove, 22);
    oFrameStageNotify = VMT::hook(pInterfaces->pCHLClient, hFrameStageNotify, 35);
    oCreateLuaInterface = VMT::hook(pInterfaces->pLuaShared, hCreateLuaInterface, 5);
    oDestroyLuaInterface = VMT::hook(pInterfaces->pLuaShared, hDestroyLuaInterface, 6);
    //oPaint = VMT::hook(pInterfaces->pEngineVgui, hPaint, 9);

    std::cout << "Hooking functions end." << std::endl;
}

HookManager::~HookManager(){
    std::cout << "Reverting VMT Tables to originals.." << std::endl;

    // Should unhook all VTable functions here.

    VMT::hook(pInterfaces->pClientModeShared, oCreateMove, 22);
    VMT::hook(pInterfaces->pCHLClient, oFrameStageNotify, 35);
    VMT::hook(pInterfaces->pLuaShared, oCreateLuaInterface, 5);
    VMT::hook(pInterfaces->pLuaShared, oDestroyLuaInterface, 6);
    //VMT::hook(pInterfaces->pEngineVgui, oPaint, 13);

    if(oRunStringEx)
        VMT::hook(pInterfaces->pLuaInterface, oRunStringEx, 111);

    std::cout << "Successfully reverted VMT Tables." << std::endl;
}