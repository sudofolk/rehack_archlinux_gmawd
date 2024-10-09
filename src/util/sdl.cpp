#include <dlfcn.h>
#include <iostream>

#include "SDL2/SDL.h"
#include "../menu/menu.h"

#include "sdl.h"

template <typename T>
static constexpr auto relativeToAbsolute(std::uintptr_t address) noexcept{
    return (T)(address + 4 + *reinterpret_cast<std::int32_t*>(address));
}

int Hooks::SDL::PollEvent(SDL_Event* event) {
    const auto result = pollEvent(event);
    
    if (Menu::initialised) {
        Menu::onPollEvent(event, result);
    }
    
    return result;
    return 1;
}

void Hooks::SDL::SwapWindow(SDL_Window* window) {
    Menu::onSwapWindow(window);

    reinterpret_cast<void(*)(SDL_Window*)>(swapWindow)(window);
}

/* Initialise SDL hooks */
bool Hooks::SDL::Initialize() {
    std::cout << "Hooking SDL2.." << std::endl;

    const auto libSDL = dlopen("libSDL2-2.0.so.0.3000.2", RTLD_LAZY | RTLD_NOLOAD);

    swapWindowAddr = relativeToAbsolute<uintptr_t*>(dlsym(libSDL, "SDL_GL_SwapWindow") + 6);

    if (swapWindowAddr) {
        swapWindow = *reinterpret_cast<decltype(swapWindow)*>(swapWindowAddr);
        *reinterpret_cast<decltype(SwapWindow)**>(swapWindowAddr) = SwapWindow;
    }else {
        std::cout << "SDL_GL_SwapWindow not found, unable to hook function." << std::endl;
        return false;
    }

    pollEventAddr = relativeToAbsolute<uintptr_t>(uintptr_t(dlsym(libSDL, "SDL_PollEvent")) + 6);
    if (pollEventAddr) {
        pollEvent = *reinterpret_cast<decltype(pollEvent)*>(pollEventAddr);
        *reinterpret_cast<decltype(PollEvent)**>(pollEventAddr) = PollEvent;
    }else {
        std::cout << "SDL_PollEvent not found, unable to hook function." << std::endl;
        return false;
    }

    std::cout << "SDL_GL_SwapWidow and SDL_PollEvent hooked successfully!" << std::endl;

    return true;
}

/* Unload SDL hooks */
bool Hooks::SDL::Unload() {
    std::cout << "Unhooking SDL2 functions, reversing VTable to original.." << std::endl;

    *reinterpret_cast<decltype(swapWindow)*>(swapWindowAddr) = swapWindow;
    *reinterpret_cast<decltype(pollEvent)*>(pollEventAddr) = pollEvent;
    if (*reinterpret_cast<decltype(swapWindow)*>(swapWindowAddr)!=swapWindow || *reinterpret_cast<decltype(pollEvent)*>(pollEventAddr)!=pollEvent) {
        std::cout << "Failed to hook SDL2" << std::endl;
        return false;
    }
    return true;
}