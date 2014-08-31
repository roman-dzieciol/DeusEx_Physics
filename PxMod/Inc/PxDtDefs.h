/*=============================================================================
	PxDtDefs.h
	Copyright 2006 Roman Dzieciol. All Rights Reserved.
=============================================================================*/


/*-----------------------------------------------------------------------------
	Static Function Detours.
-----------------------------------------------------------------------------*/

#define PX_DETOUR_LOAD(name) name##TR = (name##TY) DetourFunction( (PBYTE)name##TG, (PBYTE)name##DE );
#define PX_DETOUR_UNLOAD(name) DetourRemove((PBYTE)name##TR,(PBYTE)name##DE);

#define PX_DETOUR_TRAMPOLINE(name) name##TY name##TR = NULL;
#define PX_DETOUR_TARGET(name,ptr) name##TY name##TG = ptr;
#define PX_DETOUR_POINTERS(name,ptr) \
	PX_DETOUR_TRAMPOLINE(name) \
	PX_DETOUR_TARGET(name,ptr)


#define PX_DETOUR_LOAD_CP(ignore,name) name##TR = (name##TY) DetourFunction( (PBYTE)name##TG, (PBYTE)name##DE );
#define PX_DETOUR_UNLOAD_CP(ignore,name) DetourRemove((PBYTE)name##TR,(PBYTE)name##DE);
#define PX_DETOUR_POINTERS_CP(class,name) \
	PX_DETOUR_TRAMPOLINE(name) \
	PX_DETOUR_TARGET(name,class::name)


#define PX_DETOUR2_LOAD_MF(ignore,name) name##TR = (name##TY) DetourFunction( (PBYTE)name##TG, (PBYTE)name##DE );
#define PX_DETOUR2_UNLOAD_MF(ignore,name) DetourRemove((PBYTE)name##TR,(PBYTE)name##DE);

#define PX_DETOUR2_TRAMPOLINE_MF(name,ptr) name##_TY name##_TR = ptr;
#define PX_DETOUR2_TARGET_MF(name,ptr) name##_TY name##_TG = ptr;

#define PX_DETOUR2_POINTERS_MF(class,name) \
	PX_DETOUR2_TRAMPOLINE_MF(name,class::name) \
	PX_DETOUR2_TARGET_MF(name,class::name)


/*-----------------------------------------------------------------------------
	Member Function Detours.
-----------------------------------------------------------------------------*/

// C/P helper
#define PX_DETOUR_TP(name) TP_##name

//
#define PX_DETOUR_CDEF(cname,rval,name,params) \
class CL_##cname##name \
{ \
public: \
	rval DE_##name params; \
	rval TP_##name params; \
};

//
#define PX_DETOUR_CDEF_SUPER(cname,rval,name,params) \
	class CL_##cname##name : public cname \
{ \
public: \
	rval DE_##name params; \
	rval TP_##name params; \
};

//
#define PX_DETOUR_CFUNC(cname,rval,name,params) \
rval CL_##cname##name::DE_##name params \
{ static const TCHAR __FUNC_NAME__[] = L#cname L"::" L#name L"()";

// Detour/Trampoline class
#define PX_DETOUR_CLASS(cname,rval,name,params) \
PX_DETOUR_CDEF(cname,rval,name,params) \
DETOUR_TRAMPOLINE_EMPTY ( rval CL_##cname##name::TP_##name params ); \
PX_DETOUR_CFUNC(cname,rval,name,params)

// Detour/Trampoline class
#define PX_DETOUR_CLASS_SUPER(cname,rval,name,params) \
PX_DETOUR_CDEF_SUPER(cname,rval,name,params) \
DETOUR_TRAMPOLINE_EMPTY ( rval CL_##cname##name::TP_##name params ); \
PX_DETOUR_CFUNC(cname,rval,name,params)




// Detour loader
template<class A, class B> void PxDetour(A target, B detour, B trampoline){
	DetourFunctionWithEmptyTrampoline( *(PBYTE*)&trampoline, *(PBYTE*)&target, *(PBYTE*)&detour);}

// Detour member function
#define PX_DETOUR_MF(target,name) PxDetour( &target::name, &CL_##target##name::DE_##name, &CL_##target##name::TP_##name );


/*-----------------------------------------------------------------------------
	Static Function Detours.
-----------------------------------------------------------------------------*/


// C/P helper
#define PX_DETOUR_STC(name) TP_##name

// Detour/Trampoline class
#define PX_DETOUR_STATIC(cname,rval,name,params) \
class CL_##cname##name \
{ \
public: \
	static rval DE_##name params; \
	static rval TP_##name params; \
}; \
DETOUR_TRAMPOLINE_EMPTY ( rval CL_##cname##name::TP_##name params ); \
rval CL_##cname##name::DE_##name params \
{ pxguard(cname::name);

#define PX_DETOUR_ST(target,name) PxDetour( &target::name, &CL_##target##name::DE_##name, &CL_##target##name::TP_##name );

#define PXDT_FINISH } unguard;


/*-----------------------------------------------------------------------------
	Virtual Function Detours.
-----------------------------------------------------------------------------*/

// Traverse vtable jmp's
inline PBYTE PxVTableDeref( PBYTE pbCode )
{
	if( pbCode[0] == 0xE9 )	pbCode += 5 + *(LONG*)&pbCode[1];
	if( pbCode[0] == 0xE9 )	pbCode += 5 + *(LONG*)&pbCode[1];
	if( pbCode[0] == 0xE9 )	pbCode += 5 + *(LONG*)&pbCode[1];
	if( pbCode[0] == 0xE9 )	pbCode += 5 + *(LONG*)&pbCode[1];
	if( pbCode[0] == 0xE9 )	pbCode += 5 + *(LONG*)&pbCode[1];
	if( pbCode[0] == 0xE9 )	pbCode += 5 + *(LONG*)&pbCode[1];
	if( pbCode[0] == 0xE9 )	pbCode += 5 + *(LONG*)&pbCode[1];
	if( pbCode[0] == 0xE9 )	pbCode += 5 + *(LONG*)&pbCode[1];

	// Too many jumps! (?)
	if( pbCode[0] == 0xE9 )	
		_asm int 3;

	return pbCode;
}

// Get virtual function address 
inline PBYTE PxVTableFunc( PBYTE base, int v_offset, int f_offset )
{
	return PxVTableDeref(*(PBYTE*)(*(PBYTE*)(base + v_offset) + f_offset));
}

// Detour virtual function
#define PX_DETOUR_VR(cptr,vofs,fofs,name) PxDetour( PxVTableFunc(cptr,vofs,fofs), &CL_##cptr##name::DE_##name, &CL_##cptr##name::TP_##name );
	


/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/