/*=============================================================================
	PxMod.cpp
	Copyright 2007 Roman Dzieciol. All Rights Reserved.
=============================================================================*/

// Includes.
#include "PxMod.h"


/*-----------------------------------------------------------------------------
	The following must be done once per package (.dll).
-----------------------------------------------------------------------------*/

// This is some necessary C++/UnrealScript glue logic.
// If you forget this, you get a VC++ linker errors like:
// SampleClass.obj : error LNK2001: unresolved external symbol "class FName  PXMOD_SampleEvent" (?PXMOD_SampleEvent@@3VFName@@A)
#define NAMES_ONLY
#define AUTOGENERATE_NAME(name) PXMOD_API FName PXMOD_##name;
#define AUTOGENERATE_FUNCTION(cls,idx,name) IMPLEMENT_FUNCTION(cls,idx,name)
#include "PxScript.h"
#undef AUTOGENERATE_FUNCTION
#undef AUTOGENERATE_NAME
#undef NAMES_ONLY


void RegisterPxModNames()
{
	static INT Registered=0;
	if(!Registered++)
	{
		#define NAMES_ONLY
		#define AUTOGENERATE_NAME(name) extern PXMOD_API FName PXMOD_##name; PXMOD_##name=FName(TEXT(#name),FNAME_Intrinsic);
		#define AUTOGENERATE_FUNCTION(cls,idx,name)
		#include "PxScript.h"
		#undef DECLARE_NAME
		#undef NAMES_ONLY
	}
}


/*-----------------------------------------------------------------------------
	Implementation.
-----------------------------------------------------------------------------*/

// Package implementation.
IMPLEMENT_PACKAGE(PxMod);


/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/
