/*=============================================================================
	PxDt.cpp
	Copyright 2006 Roman Dzieciol. All Rights Reserved.
=============================================================================*/

#include "PxMod.h"
#include "PxDtDefs.h"
#include "PxPhysics.h"

/*-----------------------------------------------------------------------------
	ULevel
-----------------------------------------------------------------------------*/



PX_DETOUR_CLASS( ULevel, void, PostLoad, () )
{
	PX_LOG( PX_NAME, TEXT("%s >> %s ::"), PX_LOGP );
	PX_DETOUR_TP( PostLoad() );
	PX_LOG( PX_NAME, TEXT("%s << %s ::"), PX_LOGP );
}}

PX_DETOUR_CLASS( ULevel, void, Destroy, () )
{
	PX_LOG( PX_NAME, TEXT("%s >> %s ::"), PX_LOGP );
	PX_DETOUR_TP( Destroy() );
	PX_LOG( PX_NAME, TEXT("%s << %s ::"), PX_LOGP );
}}

PX_DETOUR_CLASS( ULevel, void, Tick, ( ELevelTick TickType, FLOAT DeltaSeconds ) )
{
	PX_LOG( PX_NAME, TEXT("%s >> %s :: "), PX_LOGP );
	PX_DETOUR_TP( Tick(TickType,DeltaSeconds) );
	PX_LOG( PX_NAME, TEXT("%s << %s :: "), PX_LOGP );
}}

PX_DETOUR_CLASS( ULevel, void, ReconcileActors, () )
{
	PX_LOG( PX_NAME, TEXT("%s >> %s ::"), PX_LOGP );
	PX_DETOUR_TP( ReconcileActors() );
	PX_LOG( PX_NAME, TEXT("%s << %s ::"), PX_LOGP );
}}

PX_DETOUR_CLASS( ULevel, void, RememberActors, () )
{
	PX_LOG( PX_NAME, TEXT("%s >> %s ::"), PX_LOGP );
	PX_DETOUR_TP( RememberActors() );
	PX_LOG( PX_NAME, TEXT("%s << %s ::"), PX_LOGP );
}}

PX_DETOUR_CLASS( ULevel, void, ShrinkLevel, () )
{
	PX_LOG( PX_NAME, TEXT("%s >> %s ::"), PX_LOGP );
	PX_DETOUR_TP( ShrinkLevel() );
	PX_LOG( PX_NAME, TEXT("%s << %s ::"), PX_LOGP );
}}

PX_DETOUR_CLASS( ULevel, void, CompactActors, () )
{
	PX_LOG( PX_NAME, TEXT("%s >> %s ::"), PX_LOGP );
	PX_DETOUR_TP( CompactActors() );
	PX_LOG( PX_NAME, TEXT("%s << %s ::"), PX_LOGP );
}}

PX_DETOUR_CLASS( ULevel, UBOOL, Listen, ( FString& Error ) )
{
	PX_LOG( PX_NAME, TEXT("%s >> %s :: [%s]"), PX_LOGP, *Error );
	UBOOL r = PX_DETOUR_TP( Listen(Error) );
	PX_LOG( PX_NAME, TEXT("%s << %s :: [%s] "), PX_LOGP, *Error );
	return r;
}}

PX_DETOUR_CLASS( ULevel, UBOOL, DestroyActor, ( AActor* Actor, UBOOL bNetForce ) )
{
	if( Cast<ADecal>(Actor) )
	{
		return PX_DETOUR_TP( DestroyActor(Actor,bNetForce) );
	}
	else
	{
		//PX_LOG( PX_NAME, TEXT("%s >> %s :: [%s],[%d]"), PX_LOGP, *ToStr(Actor), bNetForce );
		UBOOL r = PX_DETOUR_TP( DestroyActor(Actor,bNetForce) );

		
		GPxPhysics.TerminateActor(Actor);

		//PX_LOG( PX_NAME, TEXT("%s << %s :: [%s],[%d] [%d]"), PX_LOGP, *ToStr(Actor),bNetForce, r );
		return r;	
	}
}}

PX_DETOUR_CLASS( ULevel, void, CleanupDestroyed, ( UBOOL bForce ) )
{
	if( bForce )
	{
		PX_LOG( PX_NAME, TEXT("%s >> %s :: [%d]"), PX_LOGP, bForce );
		PX_DETOUR_TP( CleanupDestroyed(bForce) );
		PX_LOG( PX_NAME, TEXT("%s << %s :: [%d]"), PX_LOGP, bForce );
	}
	else
	{
		PX_DETOUR_TP( CleanupDestroyed(bForce) );
	}
}}

PX_DETOUR_CLASS( ULevel, AActor*, SpawnActor, ( UClass* Class, FName InName, AActor* Owner, class APawn* Instigator, FVector Location, FRotator Rotation, AActor* Template, UBOOL bNoCollisionFail, UBOOL bRemoteOwned ) )
{
	//PX_LOG( PX_NAME, TEXT("%s >> %s :: [%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s] "), PX_LOGP, *PxGetName(Class),*ToStr(InName),*PxGetName(Owner),*PxGetName(Instigator),*ToStr(Location),*ToStr(Rotation),*PxGetName(Template),*ToStr(bNoCollisionFail),*ToStr(bRemoteOwned) );
	//PX_LOG( PX_NAME, TEXT("%s .. %s :: [%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s] "), PX_LOGP, *PxGetName(Class),*ToStr(InName),*PxGetName(Owner),*PxGetName(Instigator),*ToStr(Location),*ToStr(Rotation),*PxGetName(Template),*ToStr(bNoCollisionFail),*ToStr(bRemoteOwned) );
	AActor* r = PX_DETOUR_TP( SpawnActor(Class,InName,Owner,Instigator,Location,Rotation,Template,bNoCollisionFail,bRemoteOwned) );
	
	if( r && r->bCollideActors )
		GPxPhysics.InitActor(r);
	//PX_LOG( PX_NAME, TEXT("%s << %s :: [%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s] [%s]"), PX_LOGP, *PxGetName(Class),*ToStr(InName),*PxGetName(Owner),*PxGetName(Instigator),*ToStr(Location),*ToStr(Rotation),*PxGetName(Template),*ToStr(bNoCollisionFail),*ToStr(bRemoteOwned), *ToStr(r) );
	return r;
}}
	
PX_DETOUR_CLASS( ULevel, void, SpawnViewActor, ( UViewport* Viewport ) )
{
	PX_LOG( PX_NAME, TEXT("%s >> %s :: [%s]"), PX_LOGP, *ToStr(Viewport) );
	PX_DETOUR_TP( SpawnViewActor(Viewport) );
	PX_LOG( PX_NAME, TEXT("%s << %s :: [%s]"), PX_LOGP, *ToStr(Viewport) );
}}

PX_DETOUR_CLASS( ULevel, APlayerPawn*, SpawnPlayActor, ( UPlayer* Viewport, ENetRole RemoteRole, const FURL& URL, FString& Error ) )
{
	PX_LOG( PX_NAME, TEXT("%s >> %s :: [%s],[%s],[%s],[%s] "), PX_LOGP, *ToStr(Viewport),*ToStr(RemoteRole),*URL.String(),*Error );
	APlayerPawn* r = PX_DETOUR_TP( SpawnPlayActor(Viewport,RemoteRole,URL,Error) );
	PX_LOG( PX_NAME, TEXT("%s << %s :: [%s],[%s],[%s],[%s] [%s] "), PX_LOGP, *ToStr(Viewport),*ToStr(RemoteRole),*URL.String(),*Error, *ToStr(r) );
	return r;
}}

PX_DETOUR_CLASS( ULevel, void, WelcomePlayer, ( UNetConnection* Connection, TCHAR* Optional ) )
{
	PX_LOG( PX_NAME, TEXT("%s >> %s :: [%s],[%s] "), PX_LOGP, *ToStr(Connection), Optional );
	PX_DETOUR_TP( WelcomePlayer(Connection,Optional) );
	PX_LOG( PX_NAME, TEXT("%s << %s :: [%s],[%s] "), PX_LOGP, *ToStr(Connection), Optional );
}}


PX_DETOUR_CLASS( ULevel, UBOOL, SinglePointCheck, ( FCheckResult& Hit, FVector Location, FVector Extent, DWORD ExtraNodeFlags, ALevelInfo* Level, UBOOL bActors ) )
{
	PX_LOG( PX_NAME, TEXT("%s >> %s :: [%s],[%s],[%s],[%s],[%s],[%s] "), PX_LOGP, *ToStr(Location),*ToStr(Extent),*ToStr(ExtraNodeFlags),*ToStr(Level),*ToStr(bActors),*ToStr(&Hit) );
	UBOOL r = PX_DETOUR_TP( SinglePointCheck(Hit,Location,Extent,ExtraNodeFlags,Level,bActors) );
	PX_LOG( PX_NAME, TEXT("%s << %s :: [%s],[%s],[%s],[%s],[%s],[%s] [%s] "), PX_LOGP, *ToStr(Location),*ToStr(Extent),*ToStr(ExtraNodeFlags),*ToStr(Level),*ToStr(bActors),*ToStr(&Hit), *ToStr(r) );
	return r;
}}

PX_DETOUR_CLASS( ULevel, UBOOL, SingleLineCheck, ( FCheckResult& Hit, AActor* SourceActor, const FVector& End, const FVector& Start, DWORD TraceFlags, FVector Extent, BYTE NodeFlags ) )
{
	PX_LOG( PX_NAME, TEXT("%s >> %s :: [%s],[%s],[%s],[%s],[%s],[%s],[%s] "), PX_LOGP, *ToStr(SourceActor),*ToStr(End),*ToStr(Start),*ToStr(TraceFlags),*ToStr(Extent),*ToStr(NodeFlags), *ToStr(&Hit) );
	UBOOL r = PX_DETOUR_TP( SingleLineCheck(Hit,SourceActor,End,Start,TraceFlags,Extent,NodeFlags) );
	PX_LOG( PX_NAME, TEXT("%s << %s :: [%s],[%s],[%s],[%s],[%s],[%s],[%s] [%s] "), PX_LOGP,*ToStr(SourceActor),*ToStr(End),*ToStr(Start),*ToStr(TraceFlags),*ToStr(Extent),*ToStr(NodeFlags), *ToStr(&Hit), *ToStr(r) );
	return r;
}}


PX_DETOUR_CLASS( ULevel, FCheckResult*, MultiPointCheck, ( FMemStack& Mem, FVector Location, FVector Extent, DWORD ExtraNodeFlags, ALevelInfo* Level, UBOOL bActors ) )
{
	PX_LOG( PX_NAME, TEXT("%s >> %s :: [%s],[%s],[%s],[%s],[%s],[%s] "), PX_LOGP, *ToStr(&Mem),*ToStr(Location),*ToStr(Extent),*ToStr(ExtraNodeFlags),*ToStr(Level),*ToStr(bActors) );
	FCheckResult* r = PX_DETOUR_TP( MultiPointCheck(Mem,Location,Extent,ExtraNodeFlags,Level,bActors) );
	PX_LOG( PX_NAME, TEXT("%s << %s :: [%s],[%s],[%s],[%s],[%s],[%s] [%s] "), PX_LOGP, *ToStr(&Mem),*ToStr(Location),*ToStr(Extent),*ToStr(ExtraNodeFlags),*ToStr(Level),*ToStr(bActors), *ToStr(r) );
	return r;
}}


PX_DETOUR_CLASS( ULevel, FCheckResult*, MultiLineCheck, ( FMemStack& Mem, FVector End, FVector Start, FVector Size, UBOOL bCheckActors, ALevelInfo* LevelInfo, BYTE ExtraNodeFlags ) )
{
	PX_LOG( PX_NAME, TEXT("%s >> %s :: [%s],[%s],[%s],[%s],[%s],[%s],[%s] "), PX_LOGP, *ToStr(&Mem),*ToStr(End),*ToStr(Start),*ToStr(Size),*ToStr(bCheckActors),*ToStr(LevelInfo),*ToStr(ExtraNodeFlags) );
	FCheckResult* r = PX_DETOUR_TP( MultiLineCheck(Mem,End,Start,Size,bCheckActors,LevelInfo,ExtraNodeFlags) );
	PX_LOG( PX_NAME, TEXT("%s << %s :: [%s],[%s],[%s],[%s],[%s],[%s],[%s] [%s] "), PX_LOGP, *ToStr(&Mem),*ToStr(End),*ToStr(Start),*ToStr(Size),*ToStr(bCheckActors),*ToStr(LevelInfo),*ToStr(ExtraNodeFlags), *ToStr(r) );
	return r;
}}


PX_DETOUR_CLASS( ULevel, UBOOL, MoveActor, ( AActor *Actor, FVector Delta, FRotator NewRotation, FCheckResult &Hit, UBOOL Test, UBOOL IgnorePawns, UBOOL bIgnoreBases, UBOOL bNoFail ) )
{
	//PX_LOG( PX_NAME, TEXT("%s >> %s :: [%s],[%s],[%s],[%s],[%s],[%s],[%s] "), PX_LOGP, *ToStr(Actor),*ToStr(Delta),*ToStr(NewRotation),*ToStr(Test),*ToStr(IgnorePawns),*ToStr(bIgnoreBases),*ToStr(bNoFail),*ToStr(&Hit) );
	
	UBOOL bMoved = FALSE;
	NxActor* nActor = GPxPhysics.GetNxActor(Actor);

	if( Cast<ADecoration>(Actor) && nActor && !Delta.IsZero() 
	&& (Actor->Physics == PHYS_Falling || Actor->Physics == PHYS_None || Actor->Physics == PHYS_Rolling) )
	{
		// update location and rotation

		//PX_LOG( PX_NAME, TEXT("%s .. %s :: [%s],[%s],[%s],[%s],[%s],[%s],[%s] "), PX_LOGP, *ToStr(Actor),*ToStr(Delta),*ToStr(NewRotation),*ToStr(Test),*ToStr(IgnorePawns),*ToStr(bIgnoreBases),*ToStr(bNoFail),*ToStr(&Hit) );
	
		Actor->setPhysics(PHYS_None);
		nActor->addForce( ToNVS(Delta), NX_SMOOTH_VELOCITY_CHANGE );
		//nActor->raiseBodyFlag(NX_BF_KINEMATIC);
		//NxMat34 m = ToMat34(NewRotation,Actor->Location+Delta);
		//nActor->moveGlobalPose(m);

		//nActor->clearBodyFlag(NX_BF_KINEMATIC);
		bMoved = TRUE;
	
		//nActor->raiseActorFlag(NX_AF_DISABLE_COLLISION);
		/*NxVec3 delta = ToNVS(Delta);

		NxMat34 m = ToMat34(NewRotation,Actor->Location+Delta);
		nActor->setGlobalPose(m);
		UBOOL bMoved = TRUE;*/

		/*NxSweepQueryHit results[100];
		NxSweepQueryHit hit;
		NxU32 nb = nActor->linearSweep(delta, NX_SF_ALL_HITS, NULL, 100, results, NULL);
		if( nb )
		{
			
			AActor* ractor = (AActor*)hit.hitShape->getActor().userData;

			FCheckResult Result;
			Result.Location = ToFVS(hit.point);
			Result.Normal = ToFV(hit.normal);
			Result.Primitive = NULL;
			Result.Time = hit.t;
			Result.Item = INDEX_NONE;
			Result.Actor = ractor;

			PX_LOG( PX_NAME, TEXT("%s .. %s :: [%s] [%s] "), PX_LOGP, *ToStr(Actor), *ToStr(&Result) );
			_asm int 3;
		}
		else
		{
			NxMat34 m = ToMat34(NewRotation,Actor->Location+Delta);
			nActor->setGlobalPose(m);

			Actor->XLevel->MoveActor(Actor,FVector(0,0,0),NewRotation,Hit,Test,IgnorePawns,bIgnoreBases,bNoFail); // rotation
			Actor->XLevel->FarMoveActor(Actor,Actor->Location+Delta,FALSE,TRUE); // location

			UBOOL bMoved = TRUE;
		
		
		//virtual UBOOL FarMoveActor( AActor* Actor, FVector DestLocation, UBOOL Test=0, UBOOL bNoCheck=0 );
	
		}*/


		/*NxVec3 ray = ToNVS(FVector(Delta));
		NxReal dist = ToNS(FVector(Delta).Size());

		NxSweepQueryHit hit;
		NxU32 nb = nActor->linearSweep(ray, NX_SF_ALL_HITS, NULL, 1, &hit, NULL);
		if( nb )
		{
			AActor* ractor = (AActor*)hit.hitShape->getActor().userData;

			Result.Location = ToFVS(hit.point);
			Result.Normal = ToFV(hit.normal);
			Result.Primitive = NULL;
			Result.Time = hit.t/dist;
			Result.Item = INDEX_NONE;
			Result.Actor = ractor;
			PX_LOG( PX_NAME, TEXT("%s .. %s :: [%s],[%s],[%s],[%s],[%d] [%d][%s] "), PX_LOGP, *ToStr(Owner), *ToStr(End), *ToStr(Start), *ToStr(Size), ExtraNodeFlags, bResult, *ToStr(Result.Actor) );

			bResult = FALSE;
		}

		bMoved = TRUE;
		Result.Location = ToFVS(Actor->Location+Delta);
		Result.Normal = FVector(0,0,0);
		Result.Primitive = NULL;
		Result.Time = 0;
		Result.Item = INDEX_NONE;
		Result.Actor = Owner;*/

		//nActor->clearActorFlag(NX_AF_DISABLE_COLLISION);
	}
	else
	{
		bMoved = PX_DETOUR_TP( MoveActor(Actor,Delta,NewRotation,Hit,Test,IgnorePawns,bIgnoreBases,bNoFail) );
	}
	
	//PX_LOG( PX_NAME, TEXT("%s << %s :: [%s],[%s],[%s],[%s],[%s],[%s],[%s] [%s] "), PX_LOGP, *ToStr(Delta),*ToStr(NewRotation),*ToStr(Test),*ToStr(IgnorePawns),*ToStr(bIgnoreBases),*ToStr(bNoFail),*ToStr(&Hit), *ToStr(r) );
	return bMoved;
}}

PX_DETOUR_CLASS( ULevel, UBOOL, FarMoveActor, ( AActor* Actor, FVector DestLocation, UBOOL Test, UBOOL bNoCheck ) )
{
	PX_LOG( PX_NAME, TEXT("%s >> %s :: [%s],[%s],[%s],[%s]"), PX_LOGP, *ToStr(Actor),*ToStr(DestLocation),*ToStr(Test),*ToStr(bNoCheck) );
	UBOOL r = PX_DETOUR_TP( FarMoveActor(Actor,DestLocation,Test,bNoCheck) );
	PX_LOG( PX_NAME, TEXT("%s << %s :: [%s],[%s],[%s],[%s] [%s] "), PX_LOGP, *ToStr(Actor),*ToStr(DestLocation),*ToStr(Test),*ToStr(bNoCheck), *ToStr(r) );
	return r;
}}



void PxDtULevel( ULevel* p )
{
	pxguard(PxDtULevel);
	PX_LOG( PX_NAME, TEXT("%s ## %s :: [%s]"), PX_LOGP, *ToStr(p) );
	if( p == NULL )
		return;

	PBYTE ULevel = PBYTE(p);

	PX_DETOUR_VR( ULevel, 0x00, 0x20, PostLoad );
	PX_DETOUR_VR( ULevel, 0x00, 0x24, Destroy );
	//PX_DETOUR_VR( ULevel, 0x00, 0x5C, Tick );
	PX_DETOUR_VR( ULevel, 0x00, 0x6c, ReconcileActors );
	PX_DETOUR_VR( ULevel, 0x00, 0x70, RememberActors );
	PX_DETOUR_VR( ULevel, 0x00, 0x78, ShrinkLevel );
	//PX_DETOUR_VR( ULevel, 0x00, 0x7c, CompactActors );
	PX_DETOUR_VR( ULevel, 0x00, 0x80, Listen );
	PX_DETOUR_VR( ULevel, 0x00, 0x88, MoveActor );
	//PX_DETOUR_VR( ULevel, 0x00, 0x8c, FarMoveActor );
	PX_DETOUR_VR( ULevel, 0x00, 0x94, DestroyActor );
	PX_DETOUR_VR( ULevel, 0x00, 0x98, CleanupDestroyed );
	PX_DETOUR_VR( ULevel, 0x00, 0x9c, SpawnActor );
	PX_DETOUR_VR( ULevel, 0x00, 0xa4, SpawnViewActor );
	PX_DETOUR_VR( ULevel, 0x00, 0xa8, SpawnPlayActor );
	//PX_DETOUR_VR( ULevel, 0x00, 0xbc, SinglePointCheck );
	//PX_DETOUR_VR( ULevel, 0x00, 0xc0, SingleLineCheck );
	//PX_DETOUR_VR( ULevel, 0x00, 0xc4, MultiPointCheck );
	//PX_DETOUR_VR( ULevel, 0x00, 0xc8, MultiLineCheck );
	PX_DETOUR_VR( ULevel, 0x00, 0xe4, WelcomePlayer );

	/*FCheckResult cr;
	FMemStack mem;
	FVector v;
	FRotator r;

	_asm nop;
	_asm int 3;
	p->MoveActor(0,v,r,cr,0,0,0,0); //88
	_asm nop;
	p->FarMoveActor(0,v,0,0); //8c
	_asm nop;*/



	/*PX_DETOUR_VR( ULevel, 0x28, 0x00, NotifyAcceptingConnection );
	PX_DETOUR_VR( ULevel, 0x28, 0x04, NotifyAcceptedConnection );
	PX_DETOUR_VR( ULevel, 0x28, 0x08, NotifyAcceptingChannel );
	PX_DETOUR_VR( ULevel, 0x28, 0x0c, NotifyGetLevel );
	PX_DETOUR_VR( ULevel, 0x28, 0x10, NotifyReceivedText );
	PX_DETOUR_VR( ULevel, 0x28, 0x14, NotifySendingFile );
	PX_DETOUR_VR( ULevel, 0x28, 0x18, NotifyReceivedFile );
	//PX_DETOUR_VR( ULevel, 0x28, 0x1c, NotifyProgress );*/

	//PxDetoursNetDrv(GLevel->NetDriver);
	unguard;
}

	
void PxDtUEntryLevel( ULevel* p )
{
	pxguard(PxDtUEntryLevel);
	PX_LOG( PX_NAME, TEXT("%s ## %s :: [%s]"), PX_LOGP, *ToStr(p) );
	if( p == NULL )
		return;

	PBYTE UEntryLevel = PBYTE(p);

	/*//PX_DETOUR_VR( UEntryLevel, 0x00, 0x5C, Tick );
	PX_DETOUR_VR( UEntryLevel, 0x00, 0x80, Listen );
	PX_DETOUR_VR( UEntryLevel, 0x00, 0xE4, WelcomePlayer );
	
	PX_DETOUR_VR( UEntryLevel, 0x28, 0x00, NotifyAcceptingConnection );
	PX_DETOUR_VR( UEntryLevel, 0x28, 0x04, NotifyAcceptedConnection );
	PX_DETOUR_VR( UEntryLevel, 0x28, 0x08, NotifyAcceptingChannel );
	PX_DETOUR_VR( UEntryLevel, 0x28, 0x0c, NotifyGetLevel );
	PX_DETOUR_VR( UEntryLevel, 0x28, 0x10, NotifyReceivedText );
	PX_DETOUR_VR( UEntryLevel, 0x28, 0x14, NotifySendingFile );
	PX_DETOUR_VR( UEntryLevel, 0x28, 0x18, NotifyReceivedFile );
	//PX_DETOUR_VR( UEntryLevel, 0x28, 0x1c, NotifyProgress );*/

	unguard;
}

void PxDtUPendingLevel( UPendingLevel* p )
{
	pxguard(PxDtUPendingLevel);
	PX_LOG( PX_NAME, TEXT("%s ## %s :: [%s]"), PX_LOGP, *ToStr(p) );
	if( p == NULL )
		return;

	/*PBYTE UNetPendingLevel = PBYTE(p);

	// Tested with UNetPendingLevel only!
	PX_DETOUR_VR( UNetPendingLevel, 0x28, 0x00, NotifyAcceptingConnection );
	PX_DETOUR_VR( UNetPendingLevel, 0x28, 0x04, NotifyAcceptedConnection );
	PX_DETOUR_VR( UNetPendingLevel, 0x28, 0x08, NotifyAcceptingChannel );
	PX_DETOUR_VR( UNetPendingLevel, 0x28, 0x0c, NotifyGetLevel );
	PX_DETOUR_VR( UNetPendingLevel, 0x28, 0x10, NotifyReceivedText );
	PX_DETOUR_VR( UNetPendingLevel, 0x28, 0x14, NotifySendingFile );
	PX_DETOUR_VR( UNetPendingLevel, 0x28, 0x18, NotifyReceivedFile );
	PX_DETOUR_VR( UNetPendingLevel, 0x28, 0x1c, NotifyProgress );*/
	unguard;
}


/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/