/*=============================================================================
	PxMod.h
	Copyright 2007 Roman Dzieciol. All Rights Reserved.
=============================================================================*/
#ifndef PXMOD_H
#define PXMOD_H


/*-----------------------------------------------------------------------------
	Libs.
-----------------------------------------------------------------------------*/
#pragma comment(lib, "Core")
#pragma comment(lib, "Engine")
#pragma comment(lib, "Render")
#pragma comment(lib, "IpDrv")
#pragma comment(lib, "Extension")
#pragma comment(lib, "DeusEx")
#pragma comment(lib, "PhysXLoader")
#pragma comment(lib, "NxCooking")
#pragma comment(lib, "NxCharacter")
#pragma comment(lib, "NxExtensions")


/*-----------------------------------------------------------------------------
	Headers.
-----------------------------------------------------------------------------*/

// PhysX
#pragma pack(push, 8)
	#define NOMINMAX
	#include "NxPhysics.h"
	#include "NxCooking.h"
#pragma pack(pop)

// Detours
#include <windows.h>
#include "detours.h"

// Unreal engine
#include "Engine.h"
//#include "UnLinker.h"
#include "UnRender.h"
#include "UnNet.h"

// Deus Ex
#include "Extension.h"
#include "DeusEx.h"
#include "DeusExGameEngine.h"


/*-----------------------------------------------------------------------------
	PxMod definitions.
-----------------------------------------------------------------------------*/

// Macros
#define PXMOD_API DLL_EXPORT
#define PX_NAME (EName)PXMOD_PxMod.GetIndex()
#define PX_SEP FString(TEXT(" | "))
#define pxguard(text) guard(text##())
#define PX_LOG GLog->Logf
#define PX_ERR GError->Logf
#define PX_LOGP *xTime(), __FUNC_NAME__

// Globals
PXMOD_API extern struct FPxLineBatcher	GPxLineBatcher;
PXMOD_API extern class UPxGameEngine*	GPxEngine;
PXMOD_API extern class FPxPhysics		GPxPhysics;

// Functions
extern void RegisterPxModNames();


/*-----------------------------------------------------------------------------
	PxMod includes
-----------------------------------------------------------------------------*/
#include "PxScript.h"
#include "PxUtil.h"
#include "PxLineBatcher.h"
#include "PxGameEngine.h"
#include "PxDetours.h"


#endif
/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/
