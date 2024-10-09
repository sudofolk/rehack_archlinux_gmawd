#include "../sdk/globals.hpp"
//#include "esp.h"

void drawPlayer(void* Player, ImDrawList* drawList){
    // do nothing
}


// TODO :
// WORLD TO SCREEN FUNCTION
// OBBMins, Maxs
// Acquire classnames of objects (predictable + m_nRealClassName) or leverage
// the lua libraries.
// >class names of objects

static bool getBox(int entityIndex, int& x, int& y, int& x2, int& y2) {
    C_BasePlayer* entity = pInterfaces->pClientEntityList->GetClientEntity(entityIndex);

    if(entity){
	Vector vOrigin, min, max;
	Vector flb, brt, blb, frt, frb, brb, blt, flt; // think of these as Front-Left-Bottom/Front-Left-Top... Etc.
    //std::cout << "here" << std::endl;

	Vector testing = entity->GetAbsOrigin();

	vOrigin = entity->GetAbsOrigin(); // I"D SAY IT CRASHS HERE
    min = entity->mins() + vOrigin;
    max = entity->maxs() + vOrigin;

    //std::cout << vOrigin.x << std::endl;
    /*std::cout << "beep" << std::endl;
	min = entity->mins() + vOrigin;
    /*
    //std::cout << "here3" << std::endl;
	max = entity->maxs() + vOrigin;*/

	Vector points[] = { Vector( min.x, min.y, min.z ),
						Vector( min.x, max.y, min.z ),
						Vector( max.x, max.y, min.z ),
						Vector( max.x, min.y, min.z ),
						Vector( max.x, max.y, max.z ),
						Vector( min.x, max.y, max.z ),
						Vector( min.x, min.y, max.z ),
						Vector( max.x, min.y, max.z ) };

	// Get screen positions
	if ( !worldToScreen( points[3], flb ) || !worldToScreen( points[5], brt )
		 || !worldToScreen( points[0], blb ) || !worldToScreen( points[4], frt )
		 || !worldToScreen( points[2], frb ) || !worldToScreen( points[1], brb )
		 || !worldToScreen( points[6], blt ) || !worldToScreen( points[7], flt ) )
		return false;

	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	float left = flb.x;
	float top = flb.y;
	float right = flb.x;
	float bottom = flb.y;

	for ( int i = 1; i < 8; i++ ) {
		if (left > arr[i].x)
			left = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
	}

	x = (int)left;
	y = (int)top;
	x2 = (int)right;
	y2 = (int)bottom;

        return true;
    }

	return false;
}

void outlinedText(ImDrawList* drawList, ImVec2 pos, ImColor color, char* text) {
    drawList->AddText(ImVec2(pos.x-1, pos.y), ImColor(0, 0, 0, 255), text);
    drawList->AddText(ImVec2(pos.x+1, pos.y), ImColor(0, 0, 0, 255), text);
    drawList->AddText(ImVec2(pos.x, pos.y-1), ImColor(0, 0, 0, 255), text);
    drawList->AddText(ImVec2(pos.x, pos.y+1), ImColor(0, 0, 0, 255), text);
    drawList->AddText(ImVec2(pos.x, pos.y), color, text);

}

static void drawBox(ImDrawList* drawList, int plyIndex, int x, int y, int x2, int y2, char* topText){
    C_BasePlayer* ply = pInterfaces->pClientEntityList->GetClientEntity(plyIndex);

    drawList->AddRect(ImVec2(x, y), ImVec2(x2, y2), ImColor(0, 0, 255), 0);
    drawList->AddRect(ImVec2(x - 1, y - 1), ImVec2(x2 + 1, y2 + 1), ImColor(0, 0, 0), 0);
    drawList->AddRect(ImVec2(x + 1, y + 1), ImVec2(x2 - 1, y2 - 1), ImColor(0, 0, 0), 0);

	float healthLost = (100 - clamp(ply->health(), 0, 100))/100.f;

	drawList->AddRect(ImVec2(x - 7, y - 1), ImVec2(x - 3, y2 + 1), ImColor(0, 0, 0), 0);
	drawList->AddRectFilled(ImVec2(x - 4, y2), ImVec2(x - 6, y ), ImColor(255, 0, 0), 0);
	drawList->AddRectFilled(ImVec2(x - 4, y2), ImVec2(x - 6, y + ((y2-y)*healthLost)), ImColor(0, 255, 0), 0);

    outlinedText(drawList, ImVec2(x+((x2-x)/2)-(ImGui::CalcTextSize(topText).x/2), y-1-(ImGui::CalcTextSize(topText).y)), ImColor(255, 255, 255, 255), topText);
}


static void ESP::Render(ImDrawList* drawList){
	if(!pInterfaces->pEngineClient->IsInGame())
		return;

	if(Globals::setposs){
		Vector spos, epos;

		if(worldToScreen(Globals::endPos, epos)){
			drawList->AddLine(ImVec2(Globals::screenSizeX/2,Globals::screenSizeY), ImVec2(epos.x, epos.y), ImGui::GetColorU32(ImVec4(255 / 255, 1 / 255, 1 / 255, 255 / 255)), 1.0f);
		}
	}

    for(auto i : Globals::playerList){
        int x, y, x2, y2;

        player_info_t pInfo;

        if(!i.second.pPlayer)
            continue;

        if(i.first != pInterfaces->pEngineClient->GetLocalPlayer() && Globals::LocalPlayer){
            if(pInterfaces->pEngineClient->GetPlayerInfo(i.first, &pInfo)){
				//std::cout << "helly" << std::endl;
				if(getBox(i.first, x, y, x2, y2)){
					drawBox(drawList, i.first, x, y, x2, y2, pInfo.name);
				}
			}

            //std::cout << x << std::endl;
            //std::cout << Globals::screenSizeX << std::endl;
        }
    } 


}
