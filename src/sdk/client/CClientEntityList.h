#pragma once

// Auto reconstructed from vtable block @ 0x0087DCA8
// from "client.dylib", by ida_vtables.idc
// Modified VTable dumper script obviously by t.me/Gaztoof.
class CClientEntityList{
public:
	/*0*/	virtual void* unk00() = 0;
	/*1*/	virtual void* unk01() = 0;
	/*2*/	virtual void* unk02() = 0;
	/*4*/	virtual void* GetClientEntity(int entnum) = 0;
	/*5*/	virtual void* GetClientEntityFromHandle(uintptr_t hEnt) = 0;
	/*6*/	virtual int   NumberOfEntities(bool bIncludeNonNetworkable = false) = 0;
	/*10*/	virtual int   GetHighestEntityIndex(void) = 0;
	/*11*/	virtual void  SetMaxEntities(int maxents) = 0;
	/*12*/	virtual int   GetMaxEntities(void) = 0;
};