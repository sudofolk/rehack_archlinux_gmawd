#include <cstdint>

namespace Hooks{
    namespace SDL {
        bool Initialize();
        inline uintptr_t* swapWindowAddr;
        inline uintptr_t swapWindow;
        inline uintptr_t pollEventAddr;
        inline std::add_pointer_t<int(SDL_Event*)> pollEvent;
        void SwapWindow(SDL_Window* window);
        int PollEvent(SDL_Event* event);
        bool Unload();
    }
}