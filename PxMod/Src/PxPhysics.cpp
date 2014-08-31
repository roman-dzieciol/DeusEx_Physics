/*=============================================================================
	PxPhysics.cpp
	Copyright 2006 Roman Dzieciol. All Rights Reserved.
=============================================================================*/

// Engine
#include "PxMod.h"
#include "PxPhysics.h"

#pragma pack(push, 8)


/*-----------------------------------------------------------------------------
	Globals.
-----------------------------------------------------------------------------*/

PXMOD_API FPxPhysics	GPxPhysics;


/*-----------------------------------------------------------------------------
	FPxPhysics.
-----------------------------------------------------------------------------*/

FPxPhysics::FPxPhysics() 
: mSDK(NULL)
, mErrorStream(NULL)
, mDrawPhysX(true)
{
	pxguard(FPxPhysics::FPxPhysics);
	PX_LOG( PX_NAME, TEXT("%s >> %s ::"), PX_LOGP );

	mErrorStream = new PxErrorStream;

	CreateSDK();

	PX_LOG( PX_NAME, TEXT("%s << %s ::"), PX_LOGP );
	unguard;
}

FPxPhysics::~FPxPhysics()
{
	if( mErrorStream )
	{
		delete mErrorStream;
		mErrorStream = NULL;
	}
}

void FPxPhysics::ReleaseSDK()
{
	pxguard(FPxPhysics::ReleaseSDK);
	PX_LOG( PX_NAME, TEXT("%s >> %s ::"), PX_LOGP );
	
	if( mSDK )
	{
		mSceneMain.TerminateLevel();

		mSDK->release();
		mSDK = NULL;
	}

	PX_LOG( PX_NAME, TEXT("%s << %s ::"), PX_LOGP );
	unguard;
}


void FPxPhysics::CreateSDK()
{
	pxguard(FPxPhysics::CreateSDK);
	PX_LOG( PX_NAME, TEXT("%s >> %s ::"), PX_LOGP );

	// Create SDK
	mSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, 0, mErrorStream);
	if(!mSDK)
	{
		PX_ERR( PX_NAME, TEXT("%s !! %s :: Wrong PhysX SDK DLL version?"), PX_LOGP );
		return;
	}
	
    // Set the debug visualization parameters
    mSDK->setParameter(NX_VISUALIZATION_SCALE, 1);
    mSDK->setParameter(NX_VISUALIZE_ACTOR_AXES, 1);
    mSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES, 1);
    mSDK->setParameter(NX_VISUALIZE_COLLISION_FNORMALS, 1);
    //mSDK->setParameter(NX_VISUALIZE_BODY_REDUCED, 1);
    //mSDK->setParameter(NX_VISUALIZE_BODY_SLEEP, 1);
    //mSDK->setParameter(NX_VISUALIZE_JOINT_REDUCED, 1);
    mSDK->setParameter(NX_VISUALIZE_COLLISION_SAP, 1);
	mSDK->setParameter(NX_VISUALIZE_CONTACT_POINT, 1);
	mSDK->setParameter(NX_VISUALIZE_CONTACT_NORMAL, 1);
	mSDK->setParameter(NX_VISUALIZE_COLLISION_SKELETONS, 1);
	mSDK->setParameter(NX_VISUALIZE_COLLISION_CCD, 1);

    // Set scale dependent parameters
    NxReal scale = 1.0f;   // scale is meters per PhysX units
    //mSDK->setParameter(NX_SKIN_WIDTH, 0.01f*(1.0f/scale));
    //mSDK->setParameter(NX_DEFAULT_SLEEP_LIN_VEL_SQUARED, 0.15f*0.15f*(1.0f/scale)*(1.0f/scale));
    //mSDK->setParameter(NX_BOUNCE_THRESHOLD, -2.0f*(1.0f/ scale));
    //DK->setParameter(NX_VISUALIZATION_SCALE, 0.5f*(1.0f/ scale));

	//mSDK->setParameter(NX_MIN_SEPARATION_FOR_PENALTY, -0.01f);
	//mSDK->setParameter(NX_CONTINUOUS_CD, 1);
	//mSDK->setParameter(NX_CCD_EPSILON, 10.01f);

	mSDK->getFoundationSDK().getRemoteDebugger()->connect ("localhost", NX_DBG_DEFAULT_PORT, NX_DBG_EVENTMASK_EVERYTHING);


	PX_LOG( PX_NAME, TEXT("%s << %s ::"), PX_LOGP );
	unguard;
}


void FPxPhysics::Tick( FLOAT DeltaTime )
{
	pxguard(FPxPhysics::Tick);

	mSceneMain.Tick(DeltaTime);

	unguard;
}

void FPxPhysics::Render( FSceneNode* Frame )
{
	pxguard(FPxPhysics::Render);

	mSceneMain.Render(Frame);

	unguard;
}

void FPxPhysics::InitLevel( ULevel* level )
{
	pxguard(FPxPhysics::InitLevel);

	mSceneMain.InitLevel(level);

	unguard;
}

void FPxPhysics::TerminateLevel()
{
	pxguard(FPxPhysics::TerminateLevel);

	mSceneMain.TerminateLevel();

	unguard;
}

void FPxPhysics::InitActor( AActor* p )
{
	pxguard(FPxPhysics::InitActor);

	mSceneMain.InitActor(p);

	unguard;
}

void FPxPhysics::TerminateActor( AActor* p )
{
	pxguard(FPxPhysics::TerminateActor);

	mSceneMain.TerminateActor(p);

	unguard;
}

void FPxPhysics::MapActor( AActor* p, NxActor* n )
{
	pxguard(FPxPhysics::MapActor);

	mActorMap.Set(p,n);

	unguard;
}

void FPxPhysics::UnmapActor( AActor* p )
{
	pxguard(FPxPhysics::UnmapActor);

	mActorMap.Remove(p);

	unguard;
}

NxActor* FPxPhysics::GetNxActor( AActor* p )
{
	pxguard(FPxPhysics::GetNxActor);

	return mActorMap.FindRef(p);

	unguard;
}


#pragma pack(pop)
/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/
