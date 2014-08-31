/*=============================================================================
	PxScene.h
	Copyright 2006 Roman Dzieciol. All Rights Reserved.
=============================================================================*/
#ifndef PXSCENE_H
#define PXSCENE_H

#pragma pack(push, 8)

/*-----------------------------------------------------------------------------
	FPxScene.
-----------------------------------------------------------------------------*/
class FPxScene
{
public:
	NxScene*						mScene;				//
	ULevel*							mLevel;				//
	const NxDebugRenderable*		mDebugRen;			//
	NxReal							mTime;				//
	NxSceneDesc						mSceneDesc;			//
	bool							mSimulating;		//
	NxBounds3						mBounds;			//
	NxSceneLimits					mSceneLimits;		//

	friend class FPxPhysics;

public:
	FPxScene();

public:
	virtual void Tick( FLOAT DeltaTime );
	virtual void Render( FSceneNode* Frame );

protected:
	virtual void InitLevel( ULevel* level );
	virtual void InitActor( AActor* uactor );
	virtual void TerminateLevel();
	virtual void TerminateActor( AActor* uactor );

protected:
	virtual void CreateScene();
	virtual void DestroyScene();
	virtual void CreateLevel( ULevel* level );


public:
	virtual NxScene* GetScene() { return mScene; }
	virtual ULevel* GetLevel() { return mLevel; }
};


/*-----------------------------------------------------------------------------
	FPxSceneMain.
-----------------------------------------------------------------------------*/
class FPxSceneMain : public FPxScene
{

public:
	void Tick( FLOAT DeltaTime );
};


#pragma pack(pop)
#endif
/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/
