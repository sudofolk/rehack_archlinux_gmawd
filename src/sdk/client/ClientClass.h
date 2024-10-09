class RecvTable;
class RecvProp;

typedef enum
{
	DPT_Ifnt=0,
	DPT_Flfoat,
	DPT_Vegctor,
	DPT_VegctorXY, // Only encodes the XY of a vector, ignores Z
	DPT_Stfring,
	DPT_Array=5,	// An array of the base types (can't be of datatables).
	DPT_DaftaTable,
#if 0 // We can't ship this since it changes the size of DTVariant to be 20 bytes instead of 16 and that breaks MODs!!!
	DPT_Quatehrnion,
#endif
};

class CRecvProxyData
{
public:
	const RecvProp	*m_pRecvProp;		// The property it's receiving.

	int		m_Value;			// The value given to you to store.

	int				m_iElement;			// Which array element you're getting.

	int				m_ObjectID;			// The object being referred to.
};

typedef void* (*CreateClientClassFn)(int entnum, int serialNum);
typedef void* (*CreateEventFn)();
typedef void (*ArrayLengthRecvProxyFn)( void *pStruct, int objectID, int currentArrayLength );
typedef void (*RecvVarProxyFn)( const CRecvProxyData *pData, void *pStruct, void *pOut );
typedef void (*DataTableRecvVarProxyFn)(const RecvProp *pProp, void **pOut, void *pData, int objectID);

class RecvProp{
public:
	RecvProp();

	void					InitArray(int nElements, int elementStride);

	int						GetNumElements() const;
	void					SetNumElements(int nElements);

	int						GetElementStride() const;
	void					SetElementStride(int stride);

	int						GetFlags() const;

	const char* GetName() const;
	void* GetType() const;

	RecvTable* GetDataTable() const;
	void					SetDataTable(RecvTable* pTable);

	void* GetProxyFn() const;
	void					SetProxyFn(void* fn);

	void* GetDataTableProxyFn() const;
	void					SetDataTableProxyFn(void* fn);

	int						GetOffset() const;
	void					SetOffset(int o);

	// Arrays only.
	RecvProp* GetArrayProp() const;
	void					SetArrayProp(RecvProp* pProp);

	// Arrays only.
	void					SetArrayLengthProxy(void* proxy);
	void* GetArrayLengthProxy() const;

	bool					IsInsideArray() const;
	void					SetInsideArray();

	// Some property types bind more data to the prop in here.
	const void* GetExtraData() const;
	void				SetExtraData(const void* pData);

	// If it's one of the numbered "000", "001", etc properties in an array, then
	// these can be used to get its array property name for debugging.
	const char* GetParentArrayPropName();
	void				SetParentArrayPropName(const char* pArrayPropName);

public:

	const char* m_pVarName;
	int			m_RecvType;
	int						m_Flags;
	int						m_StringBufferSize;


private:

	bool					m_bInsideArray;		// Set to true by the engine if this property sits inside an array.

	// Extra data that certain special property types bind to the property here.
	const void* m_pExtraData;

	// If this is an array (DPT_Array).
	RecvProp* m_pArrayProp;
	void*	m_ArrayLengthProxy;

	void* m_ProxyFn;
	void* m_DataTableProxyFn;	// For RDT_DataTable.

	RecvTable* m_pDataTable;		// For RDT_DataTable.
	int						m_Offset;

	int						m_ElementStride;
	int						m_nElements;

	// If it's one of the numbered "000", "001", etc properties in an array, then
	// these can be used to get its array property name for debugging.
	const char* m_pParentArrayPropName;
};
class RecvTable
{
public:

	typedef RecvProp	PropType;

	RecvTable();
	RecvTable(RecvProp* pProps, int nProps, const char* pNetTableName);
	~RecvTable();

	void		Construct(RecvProp* pProps, int nProps, const char* pNetTableName);

	int			GetNumProps();
	RecvProp* GetProp(int i);

	const char* GetName();

	// Used by the engine while initializing array props.
	void		SetInitialized(bool bInitialized);
	bool		IsInitialized() const;

	// Used by the engine.
	void		SetInMainList(bool bInList);
	bool		IsInMainList() const;


public:

	// Properties described in a table.
	RecvProp* m_pProps;
	int				m_nProps;

	// The decoder. NOTE: this covers each RecvTable AND all its children (ie: its children
	// will have their own decoders that include props for all their children).
	void* m_pDecoder;

	const char* m_pNetTableName;	// The name matched between client and server.


private:

	bool			m_bInitialized;
	bool			m_bInMainList;
};

inline int RecvTable::GetNumProps()
{
	return m_nProps;
}

inline RecvProp* RecvTable::GetProp( int i )
{
	//Assert( i >= 0 && i < m_nProps ); 
	return &m_pProps[i]; 
}

inline const char* RecvTable::GetName()
{
	return m_pNetTableName; 
}

inline void RecvTable::SetInitialized( bool bInitialized )
{
	m_bInitialized = bInitialized;
}

inline bool RecvTable::IsInitialized() const
{
	return m_bInitialized;
}

inline void RecvTable::SetInMainList( bool bInList )
{
	m_bInMainList = bInList;
}

inline bool RecvTable::IsInMainList() const
{
	return m_bInMainList;
}


inline void RecvProp::InitArray( int nElements, int elementStride )
{
	m_RecvType = DPT_Array;
	m_nElements = nElements;
	m_ElementStride = elementStride;
}

inline int RecvProp::GetNumElements() const
{
	return m_nElements;
}

inline void RecvProp::SetNumElements( int nElements )
{
	m_nElements = nElements;
}

inline int RecvProp::GetElementStride() const
{
	return m_ElementStride;
}

inline void RecvProp::SetElementStride( int stride )
{
	m_ElementStride = stride;
}

inline int RecvProp::GetFlags() const
{
	return m_Flags;
}

inline const char* RecvProp::GetName() const
{
	return m_pVarName; 
}

inline void* RecvProp::GetType() const
{
	return m_RecvType; 
}

inline RecvTable* RecvProp::GetDataTable() const 
{
	return m_pDataTable; 
}

inline void RecvProp::SetDataTable( RecvTable *pTable )
{
	m_pDataTable = pTable; 
}

inline void* RecvProp::GetProxyFn() const 
{
	return m_ProxyFn; 
}

inline void RecvProp::SetProxyFn( void* fn )
{
	m_ProxyFn = fn; 
}

inline void* RecvProp::GetDataTableProxyFn() const
{
	return m_DataTableProxyFn; 
}

inline void RecvProp::SetDataTableProxyFn( void* fn )
{
	m_DataTableProxyFn = fn; 
}

inline int RecvProp::GetOffset() const	
{
	return m_Offset; 
}

inline void RecvProp::SetOffset( int o )
{
	m_Offset = o; 
}

inline RecvProp* RecvProp::GetArrayProp() const
{
	return m_pArrayProp;
}

inline void RecvProp::SetArrayProp( RecvProp *pProp )
{
	m_pArrayProp = pProp;
}

inline void RecvProp::SetArrayLengthProxy( void* proxy )
{
	m_ArrayLengthProxy = proxy;
}

inline void* RecvProp::GetArrayLengthProxy() const
{
	return m_ArrayLengthProxy;
}

inline bool RecvProp::IsInsideArray() const
{
	return m_bInsideArray;
}

inline void RecvProp::SetInsideArray()
{
	m_bInsideArray = true;
}

inline const void* RecvProp::GetExtraData() const
{
	return m_pExtraData;
}

inline void RecvProp::SetExtraData( const void *pData )
{
	m_pExtraData = pData;
}

inline const char* RecvProp::GetParentArrayPropName()
{
	return m_pParentArrayPropName;
}

inline void	RecvProp::SetParentArrayPropName( const char *pArrayPropName )
{
	m_pParentArrayPropName = pArrayPropName;
}


class ClientClass
{
public:
	ClientClass(const char* pNetworkName, CreateClientClassFn createFn, CreateEventFn createEventFn, RecvTable* pRecvTable)
	{
		m_pNetworkName = pNetworkName;
		m_pCreateFn = createFn;
		m_pCreateEventFn = createEventFn;
		m_pRecvTable = pRecvTable;

		// Link it in
		//m_pNext = g_pClientClassHead;
		//g_pClientClassHead = this;
	}

	const char* GetName()
	{
		return m_pNetworkName;
	}

public:
	CreateClientClassFn		m_pCreateFn;
	CreateEventFn			m_pCreateEventFn;	// Only called for event objects.
	const char* m_pNetworkName;
	RecvTable* m_pRecvTable;
	ClientClass* m_pNext;
	int						m_ClassID;	// Managed by the engine.
};
