/*=============================================================================
	PxMod.cpp
	Copyright 2007 Roman Dzieciol. All Rights Reserved.
=============================================================================*/

// Includes.
#include "PxMod.h"
#include "PxPhysics.h"


/*-----------------------------------------------------------------------------
	Implementation.
-----------------------------------------------------------------------------*/

// Class implementation.
IMPLEMENT_CLASS(UPxGameEngine);

PXMOD_API class UPxGameEngine*	GPxEngine;


/*=============================================================================
	UPxGameEngine ::
=============================================================================*/

UPxGameEngine::UPxGameEngine() : DDeusExGameEngine()
{
	RegisterPxModNames();
}


/*-----------------------------------------------------------------------------
	UPxGameEngine :: UObject interface.
-----------------------------------------------------------------------------*/

void UPxGameEngine::Destroy(void)
{
	pxguard(UPxGameEngine::Destroy);
	PX_LOG( PX_NAME, TEXT("%s >> %s :: "), PX_LOGP );

	GPxPhysics.ReleaseSDK();
	DDeusExGameEngine::Destroy();

	PX_LOG( PX_NAME, TEXT("%s << %s :: "), PX_LOGP );
	unguard;
}


/*-----------------------------------------------------------------------------
	UPxGameEngine :: UEngine interface.
-----------------------------------------------------------------------------*/

void UPxGameEngine::Init(void)
{
	pxguard(UPxGameEngine::Init);

	RegisterPxModNames();
	GPxEngine = this;

	PX_LOG( PX_NAME, TEXT("%s >> %s :: "), PX_LOGP );

	PxDtStatic();
	//PxDtFFileManager(GFileManager);

	DDeusExGameEngine::Init();

	PX_LOG( PX_NAME, TEXT("%s << %s :: "), PX_LOGP );
	unguard;
}

void UPxGameEngine::Exit()
{
	pxguard(UPxGameEngine::Exit);
	PX_LOG( PX_NAME, TEXT("%s >> %s :: "), PX_LOGP );

	// TODO:: find out when is this called
	DDeusExGameEngine::Exit();

	PX_LOG( PX_NAME, TEXT("%s << %s :: "), PX_LOGP );
	unguard;
}

void UPxGameEngine::Tick( FLOAT DeltaSeconds )
{
	pxguard(UPxGameEngine::Tick);
	//PX_LOG( PX_NAME, TEXT("%s >> %s :: "), PX_LOGP );

	DDeusExGameEngine::Tick(DeltaSeconds);
	GPxPhysics.Tick(DeltaSeconds);

	//PX_LOG( PX_NAME, TEXT("%s << %s :: "), PX_LOGP );
	unguard;
}

UBOOL UPxGameEngine::Exec(const TCHAR* Cmd, FOutputDevice& Ar)
{
	pxguard(UPxGameEngine::Exec);
	//PX_LOG( PX_NAME, TEXT("%s >> %s :: [%s]"), PX_LOGP, Cmd );

	if( DDeusExGameEngine::Exec(Cmd,Ar) )
	{
	}
	else if( ParseCommand(&Cmd,TEXT("lbe")) )
	{
		GPxLineBatcher.Empty();
	}
	else if( ParseCommand(&Cmd,TEXT("lbt")) )
	{
		GPxLineBatcher.Toggle();
	}
	else if( ParseCommand(&Cmd,TEXT("drf")) )
	{
		PxDumpRefs();
	}
	else if( ParseCommand(&Cmd,TEXT("pxdraw")) )
	{
		GPxPhysics.mDrawPhysX = !GPxPhysics.mDrawPhysX;
	}
	else
	{
		//PX_LOG( PX_NAME, TEXT("%s << %s :: [%s]"), PX_LOGP, Cmd );
		return 0;
	}

	//PX_LOG( PX_NAME, TEXT("%s << %s :: [%s]"), PX_LOGP, Cmd );
	return 1;
	unguard;
}


/*-----------------------------------------------------------------------------
	UPxGameEngine :: UGameEngine interface.
-----------------------------------------------------------------------------*/

UBOOL UPxGameEngine::Browse( FURL URL, const TMap<FString,FString>* TravelInfo, FString& Error )
{
	pxguard(UPxGameEngine:Browse);

	PX_LOG( PX_NAME, TEXT("%s >> %s :: [%s] [%s]"), PX_LOGP, *URL.String(), *Error );

	//PxDtULevel(GLevel);
	UBOOL bResult = DDeusExGameEngine::Browse(URL,TravelInfo,Error);

	//PxDumpLevels();

	PxDtULevel(GLevel);
	if( GLevel != GEntry && GEntry != NULL )
		GPxPhysics.InitLevel(GLevel);
	
	PX_LOG( PX_NAME, TEXT("%s << %s :: [%s] [%s]"), PX_LOGP, *URL.String(), *Error );
	return bResult;
	unguard;
}

ULevel* UPxGameEngine::LoadMap( const FURL& URL, UPendingLevel* Pending, const TMap<FString,FString>* TravelInfo, FString& Error )
{
	pxguard(UPxGameEngine::LoadMap);
	PX_LOG( PX_NAME, TEXT("%s >> %s :: [%s] [%s] [%s]"), PX_LOGP, *URL.String(), *Error, *URL.Map );

	//PxDtULevel(GLevel);
	ULevel* r = DDeusExGameEngine::LoadMap(URL,Pending,TravelInfo,Error);
	//PxDtULevel(GLevel);

	PX_LOG( PX_NAME, TEXT("%s << %s :: [%s] [%s]"), PX_LOGP, *URL.String(), *Error );
	return r;
	unguard;
}

void UPxGameEngine::NotifyLevelChange()
{
	pxguard(UPxGameEngine::NotifyLevelChange);
	PX_LOG( PX_NAME, TEXT("%s >> %s :: "), PX_LOGP );

	GPxPhysics.TerminateLevel();
	DDeusExGameEngine::NotifyLevelChange();

	PX_LOG( PX_NAME, TEXT("%s << %s :: "), PX_LOGP );
	unguard;
}


/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/