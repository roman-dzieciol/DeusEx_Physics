/*=============================================================================
	PxPhysics.h
	Copyright 2006 Roman Dzieciol. All Rights Reserved.
=============================================================================*/
#ifndef PXPHYSICS_H
#define PXPHYSICS_H

#pragma pack(push, 8)


/*-----------------------------------------------------------------------------
	PhysX Globals.
-----------------------------------------------------------------------------*/

#define UUTORAD (NxPiF32 / 32768.0f)
#define PX_GRAVITY NxVec3(0.0f, -9.81f, 0.0f)
#define PX_SCALE 48.0f
#define PX_TIMESTEP (1.0f/60.0f)


/*-----------------------------------------------------------------------------
	PhysX Includes.
-----------------------------------------------------------------------------*/

#include "PxErrorStream.h"
#include "PxPhysicsUtil.h"
#include "PxScene.h"


/*-----------------------------------------------------------------------------
	FPxPhysics.
-----------------------------------------------------------------------------*/

class FPxPhysics
{
public:
	PxErrorStream*					mErrorStream;		//
	NxPhysicsSDK*					mSDK;				//

	FPxSceneMain					mSceneMain;			// Main scene

	bool							mDrawPhysX;			//

protected:
	TMap<AActor*,NxActor*>			mActorMap;			//

public:
	FPxPhysics();
	~FPxPhysics();

public:
	void CreateSDK();
	void ReleaseSDK();
	void SetSDKParameters();
	
public:
	void InitLevel( ULevel* level );
	void TerminateLevel();
	void InitActor( AActor* p );
	void TerminateActor( AActor* p );

public:
	void MapActor( AActor* p, NxActor* n );
	void UnmapActor( AActor* p );
	NxActor* GetNxActor( AActor* p );

public:
	void Tick( FLOAT DeltaTime );
	void Render( FSceneNode* Frame );


public:
	NxPhysicsSDK& GetSDK() { return *mSDK; }
	FPxSceneMain& GetSceneMain() { return mSceneMain; }
};


#pragma pack(pop)
#endif
/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/
