struct backtrackData{
    Vector headpos;
    int ticknum;
};

struct pCache{
    C_BasePlayer* pPlayer;
    bool isFriend;
    std::vector<backtrackData> backtracks;
};

struct eCache{
    C_BasePlayer* pEntity;
    std::string entity_class;
};

namespace Globals{
    inline VMatrix worldToScreenMatrix;
    inline bool oRSHooked = false;
    inline bool lualoaded = false;
    inline C_BasePlayer* LocalPlayer;
    inline std::map<int, pCache> playerList;
    inline std::map<int, eCache> entityList;
    inline int screenSizeX;
    inline int screenSizeY;
    inline Vector startPos;
    inline Vector endPos;
    inline bool setposs;
    inline void* AimbotTarget;
}

inline bool worldToScreen( const Vector& origin, Vector& screen ) {
	VMatrix worldToScreenMatrix = pInterfaces->pEngineClient->WorldToScreenMatrix();

	float w = worldToScreenMatrix[3][0] * origin.x
			  + worldToScreenMatrix[3][1] * origin.y
			  + worldToScreenMatrix[3][2] * origin.z
			  + worldToScreenMatrix[3][3];

	if ( w < 0.01f )
		return false;

	float inverseW = 1 / w;

	screen.x = (Globals::screenSizeX/2) + (0.5f * ((worldToScreenMatrix[0][0] * origin.x + worldToScreenMatrix[0][1] * origin.y +
						  			 worldToScreenMatrix[0][2] * origin.z + worldToScreenMatrix[0][3]) * inverseW) * Globals::screenSizeX + 0.5f);

	screen.y = (Globals::screenSizeY/2) - (0.5f * ((worldToScreenMatrix[1][0] * origin.x + worldToScreenMatrix[1][1] * origin.y +
									  worldToScreenMatrix[1][2] * origin.z + worldToScreenMatrix[1][3]) * inverseW) * Globals::screenSizeY + 0.5f);
	return true;
}