#include <iostream>
#include <cstring>
#include <dlfcn.h>

#include "sdk/globals.hpp"

Interfaces* pInterfaces;
HookManager* pHookManager;

/*
 * Given the RecvTable, follow the linked list, dumping all property names and offset values as we go.
\*/
static size_t recursive_scan(RecvTable *table, const char *var_name, size_t accum = 0){
	for(auto i = 0; i < table->GetNumProps(); i++) {
		const auto *prop = table->GetProp(i);
		
		if(strlen(prop->GetName()) > 3)
			printf("%s => 0x%X \n", prop->GetName(), prop->GetOffset());

		if(strcmp(prop->GetName(), var_name) == 0){
			return accum + prop->GetOffset();
		}

		if(prop->GetType() != 6)
			continue;

		auto result = recursive_scan(prop->GetDataTable(),
				var_name,
				accum + prop->GetOffset());
		if(result != 0){
			return result;
		}
			
	}

	return 0;
}

/*
 * Given a class name (ex CBasePlayer) and a netvar name get the netvar offset
 */
size_t get_netvar(const char *class_name, const char *var_name){
	for(auto *list = pInterfaces->pCHLClient->GetAllClasses();list != nullptr;list = list->m_pNext) {
		std::cout << list->GetName() << std::endl;
		if(strcmp(list->GetName(), class_name) == 0)
			return recursive_scan(list->m_pRecvTable, var_name);
	}

	return 0;
}

int __attribute__((constructor)) startup() {

	Config::init();

	std::cout << "hi" << std::endl;

	pInterfaces = new Interfaces();
	std::cout << "hi2" << std::endl;
	pHookManager = new HookManager();
	std::cout << "hi3" << std::endl;
	Hooks::SDL::Initialize();

	//std::cout << get_netvar("CGMOD_Player", "") << std::endl;
	//recursive_scan(pInterfaces->pCHLClient->GetAllClasses()->m_pRecvTable, "");

    return 0;
};

void __attribute__((destructor)) shutdown() {
	std::cout << "Unload gmod_re begin" << std::endl;

	Hooks::SDL::Unload();

	//delete pSDLHook;
	delete pInterfaces;
	delete pHookManager;

    std::cout << "Unloaded!" << std::endl;
}