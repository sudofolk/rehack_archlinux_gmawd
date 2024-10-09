#include <iostream>
#include <cstring>
#include <dlfcn.h>

#include "sdk/globals.hpp"

#include "interfaces/interfaces.h"
#include "hooks/hooks.h"

#include "SDL2/SDL.h"

#include "imgui/GL/gl3w.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

#include "util/sdl.h"
#include "util/vmt.h"
#include "menu/menu.h"
#include "menu/roboto.h"