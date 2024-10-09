#include <vector>

namespace lua
{
	using number_t = double;

	enum class special{
		glob,
		env,
		reg
	};

	enum LuaFindResult
	{
		IHaveNoIdea
	};

	enum class type{
		client,
		server,
		menu
	};

	class ILuaCallback{
	public:

		/*0*/	virtual void* CreateLuaObject() = 0;
		/*1*/	virtual void DestroyLuaObject(void* pObject) = 0;

		/*2*/	virtual void ErrorPrint(const char* strError) = 0;
		/*3*/	virtual void Msg(const char* strMsg) = 0;

		/*4*/	virtual bool CanRunScript(const char* strFilename, unsigned long CRC) = 0;
		/*5*/	virtual void onRunScript(const char* strFilename, bool bRun, const char* strScriptContents) = 0;
	};

	class CLuaInterface{
	public:

		virtual int			Top(void) = 0;
		virtual void		Push(int iStackPos) = 0;
		virtual void		Pop(int iAmt = 1) = 0;
		virtual void		GetTable(int iStackPos) = 0;
		virtual void		GetField(int iStackPos, const char* strName) = 0;
		virtual void		SetField(int iStackPos, const char* strName) = 0;
		virtual void		CreateTable() = 0;
		virtual void		SetTable(int i) = 0;
		virtual void		SetMetaTable(int i) = 0;
		virtual bool		GetMetaTable(int i) = 0;
		virtual void		Call(int iArgs, int iResults) = 0;
		virtual int			PCall(int iArgs, int iResults, int iErrorFunc) = 0;
		virtual int			Equal(int iA, int iB) = 0;
		virtual int			RawEqual(int iA, int iB) = 0;
		virtual void		Insert(int iStackPos) = 0;
		virtual void		Remove(int iStackPos) = 0;
		virtual int			Next(int iStackPos) = 0;
		virtual void*		NewUserdata(unsigned int iSize) = 0;
		virtual void		ThrowError(const char* strError) = 0;
		virtual void		CheckType(int iStackPos, int iType) = 0;
		virtual void		ArgError(int iArgNum, const char* strMessage) = 0;
		virtual void		RawGet(int iStackPos) = 0;
		virtual void		RawSet(int iStackPos) = 0;
		virtual const char*	GetString(int iStackPos = -1, unsigned int* iOutLen = nullptr) = 0;
		virtual double		GetNumber(int iStackPos = -1) = 0;
		virtual bool		GetBool(int iStackPos = -1) = 0;
		virtual void*		GetCFunction(int iStackPos = -1) = 0;
		virtual void*		GetUserdata(int iStackPos = -1) = 0;
		virtual void		PushNil() = 0;
		virtual void		PushString(const char* val, unsigned int iLen = 0) = 0;
		virtual void		PushNumber(double val) = 0;
		virtual void		PushBool(bool val) = 0;
		virtual void		PushCFunction(void* val) = 0;
		virtual void		PushCClosure(void* val, int iVars) = 0;
		virtual void		PushUserdata(void*) = 0;
		virtual int			ReferenceCreate() = 0;
		virtual void		ReferenceFree(int i) = 0;
		virtual void		ReferencePush(int i) = 0;
		virtual void		PushSpecial(special iType) = 0;
		virtual bool		IsType(int iStackPos, int iType) = 0;
		virtual int			GetType(int iStackPos) = 0;
		virtual const char*	GetTypeName(int iType) = 0;
		virtual void		CreateMetaTableType(const char* strName, int iType) = 0;
		virtual const char*	CheckString(int iStackPos = -1) = 0;
		virtual double		CheckNumber(int iStackPos = -1) = 0;
		virtual void* ObjLen(int) = 0;
	/*46*/	virtual void* GetAngle(int) = 0;
	/*47*/	virtual void* GetVector(int) = 0;
	/*48*/	virtual void* PushAngle(QAngle const&) = 0;
	/*49*/	virtual void* PushVector(Vector const&) = 0;
	/*50*/	virtual void* SetState(void*) = 0;
	/*51*/	virtual void* CreateMetaTable(char const*) = 0;
	/*52*/	virtual void* PushMetaTable(int) = 0;
	/*53*/	virtual void* PushUserType(void*, int) = 0;
	/*54*/	virtual void* SetUserType(int, void*) = 0;
	/*55*/	virtual void* Init(ILuaCallback*, bool) = 0;
	/*56*/	virtual void* Shutdown(void) = 0;
	/*57*/	virtual void* Cycle(void) = 0;
	/*58*/	virtual void* Global(void) = 0;
	/*59*/	virtual void* GetObjectt(int) = 0;
	/*60*/	virtual void* PushLuaObject(void*) = 0;
	/*61*/	virtual void* PushLuaFunction(int (*)(void*)) = 0;
	/*62*/	virtual void* LuaError(char const*, int) = 0;
	/*63*/	virtual void* TypeError(char const*, int) = 0;
	/*64*/	virtual void* CallInternal(int, int) = 0;
	/*65*/	virtual void* CallInternalNoReturns(int) = 0;
	/*66*/	virtual void* CallInternalGetBool(int) = 0;
	/*67*/	virtual void* CallInternalGetString(int) = 0;
	/*68*/	virtual void* CallInternalGet(int, void*) = 0;
	/*69*/	virtual void* NewGlobalTable(char const*) = 0;
	/*70*/	virtual void* NewTemporaryObject(void) = 0;
	/*71*/	virtual bool isUserData(int) = 0;
	/*72*/	virtual void* GetMetaTableObject(char const*, int) = 0;
	/*73*/	virtual void* GetMetaTableObject(int) = 0;
	/*74*/	virtual void* GetReturn(int) = 0;
	/*75*/	virtual bool IsServer(void) = 0;
	/*76*/	virtual bool IsClient(void) = 0;
	/*77*/	virtual bool IsMenu(void) = 0;
	/*78*/	virtual void* DestroyObject(void*) = 0;
	/*79*/	virtual void* CreateObject(void) = 0;
	/*80*/	virtual void* SetMember(void*, void*, void*) = 0;
	/*81*/	virtual void* GetNewTable(void) = 0;
	/*82*/	virtual void* SetMember(void*, float) = 0;
	/*83*/	virtual void* SetMember(void*, float, void*) = 0; // ILuaObject
	/*84*/	virtual void* SetMember(void*, char const*) = 0;
	/*85*/	virtual void* SetMember(void*, char const*, void*) = 0; // ILuaObject* key, ILuaObject* value
	/*86*/	virtual void* SetType(unsigned char) = 0;
	/*87*/	virtual void* PushLong(long) = 0;
	/*88*/	virtual void* GetFlags(int) = 0;
	/*89*/	virtual void* FindOnObjectsMetaTable(int, int) = 0;
	/*90*/	virtual void* FindObjectOnTable(int, int) = 0;
	/*91*/	virtual void* SetMemberFast(void*, int, int) = 0;
	/*92*/	virtual void* RunString(char const*, char const*, char const*, bool, bool) = 0;
	/*93*/	virtual bool IsEqual(void*, void*) = 0;
	/*94*/	virtual void* Error(char const*) = 0;
	/*95*/	virtual void* GetStringOrError(int) = 0;
	/*96*/	virtual void* RunLuaModule(char const*) = 0;
	/*97*/	virtual void* FindAndRunScript(char const*, bool, bool, char const*, bool) = 0;
	/*98*/	virtual void* SetPathID(char const*) = 0;
	/*99*/	virtual const char* GetPathID(void) = 0;
	/*100*/	virtual void* ErrorNoHalt(char const*, ...) = 0;
	/*101*/	virtual void* Msg(char const*, ...) = 0;
	/*102*/	virtual void* PushPath(char const*) = 0;
	/*103*/	virtual void* PopPath(void) = 0;
	/*104*/	virtual const char* GetPath(void) = 0;
	/*105*/	virtual void* GetColor(int) = 0;
	/*106*/	virtual void* PushColor(Color) = 0;
	/*107*/	virtual void* GetStack(int, void*) = 0;
	/*108*/	virtual void* GetInfo(char const*, void*) = 0;
	/*109*/	virtual void* GetLocal(void*, int) = 0;
	/*110*/	virtual void* GetUpvalue(int, int) = 0;
	/*111*/	virtual bool  RunStringEx(char const*, char const*, char const*, bool, bool, bool, bool) = 0;
	/*112*/	virtual void* GetDataString(int, void**) = 0;
	/*113*/	virtual void* ErrorFromLua(char const*, ...) = 0;
	/*114*/	virtual void* GetCurrentLocation(void) = 0;
	/*115*/	virtual void* MsgColour(Color const&, char const*, ...) = 0;
	/*116*/	virtual void* GetCurrentFile(std::string, std::string) = 0;
	/*117*/	virtual void* CompileString(void*, std::string const&) = 0;
	/*118*/	virtual void* CallFunctionProtected(int, int, bool) = 0;
	/*119*/	virtual void* Require(char const*) = 0;
	/*120*/	virtual void* GetActualTypeName(int) = 0;
	/*121*/	virtual void* PreCreateTable(int, int) = 0;
	/*122*/	virtual void* PushPooledString(int) = 0;
	/*123*/	virtual void* GetPooledString(int) = 0;
	/*124*/	virtual void* AddThreadedCall(void*) = 0;
	/*125*/	virtual void* AppendStackTrace(char*, unsigned long) = 0;
	/*126*/	virtual void* CreateConVar(char const*, char const*, char const*, int) = 0;
	/*127*/	virtual void* CreateConCommand(char const*, char const*, int, void (*)(void const*), int (*)(char const*, char(*)[128])) = 0;
	};

	class ILuaShared {
	public:
	/*0*/	virtual ~ILuaShared() = 0;
			virtual void* unk() = 0;
			virtual int Init(char*, char*, void*, void*) = 0;
			virtual int Shutdown(void) = 0;
			virtual int DumpStats(void) = 0;
			virtual CLuaInterface* CreateLuaInterface(int, bool) = 0;
			virtual void CloseLuaInterface(CLuaInterface*) = 0;
			virtual CLuaInterface* GetLuaInterface(int) = 0;
			virtual int LoadFile(std::string, std::string, std::string, bool) = 0;
			virtual int GetCache(std::string) = 0;
			virtual int MountLua(const char*) = 0;
			virtual int MountLuaAdd(const char*, const char*) = 0;
			virtual int UnMountLua(const char*) = 0;
			virtual void SetFileContents() = 0; // does legit nothing
			virtual int SetLuaFindHook(void*) = 0;
			virtual int FindScripts() = 0;
			virtual void* GetStackTraces() = 0;
};
}

struct lua_State{
	unsigned char _ignore_this_common_lua_header_[69];
	lua::CLuaInterface*	luabase;
};