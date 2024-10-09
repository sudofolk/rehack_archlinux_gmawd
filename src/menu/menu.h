#include "SDL2/SDL.h"
#include "../imgui/imgui.h"

namespace Menu{
    inline bool open = true;
    inline bool initialised = false;
    inline int tabSelected = 0;

    inline bool devWindow = false;
    inline bool demoWindow = true;
    inline bool espOn = true;
    
    void drawMenu();
    void onPollEvent(SDL_Event* event, const int result);
    void onSwapWindow(SDL_Window* window);
    void drawOverlay(ImDrawList* drawList);
}