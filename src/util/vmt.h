#include <stdint.h>

namespace VMT {
	void* hook(void* instance, void* hook, int offset);	
	uintptr_t ReadData(void* instance, int index, int offset);
	uintptr_t* GetVTable(void* instance);
}