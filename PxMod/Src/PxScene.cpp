/*=============================================================================
	PxScene.cpp
	Copyright 2006 Roman Dzieciol. All Rights Reserved.
=============================================================================*/

// Engine
#include "PxMod.h"
#include "PxPhysics.h"
#include "PxMemBuffer.h"
#include "PxTriangulator.h"

#pragma pack(push, 8)

/*-----------------------------------------------------------------------------
	FPxScene
-----------------------------------------------------------------------------*/
FPxScene::FPxScene() 
: mScene(NULL)
, mLevel(NULL)
, mDebugRen(NULL)
, mTime(0)
, mSimulating(false)
{
	// Setup scene description
	mSceneDesc.gravity				= PX_GRAVITY;;
	mSceneDesc.simType				= NX_SIMULATION_SW;
	mSceneDesc.maxTimestep			= PX_TIMESTEP;
	mSceneDesc.maxIter				= 1;
	mSceneDesc.timeStepMethod		= NX_TIMESTEP_FIXED;
	mSceneDesc.flags			   |= NX_SF_FORCE_CONE_FRICTION;
}

void FPxScene::CreateScene()
{
	pxguard(FPxScene::CreateScene);
	PX_LOG( PX_NAME, TEXT("%s >> %s ::"), PX_LOGP );

	if( mScene )
	{
		PX_ERR( PX_NAME, TEXT("%s !! %s :: Scene already exists"), PX_LOGP );
		return;
	}
	
	// Create scene
	mScene = GPxPhysics.GetSDK().createScene(mSceneDesc);

	// group 31 doesn't collide with anything else
	for( int i=0; i<31; ++i )
	{
		mScene->setGroupCollisionFlag(i,31,false);
	}

	PX_LOG( PX_NAME, TEXT("%s << %s ::"), PX_LOGP );
	unguard;
}

void FPxScene::DestroyScene()
{
	pxguard(FPxScene::DestroyScene);
	PX_LOG( PX_NAME, TEXT("%s >> %s ::"), PX_LOGP );

	if( mScene )
	{
		GPxPhysics.GetSDK().releaseScene(*mScene);
		mScene = NULL;
	}
	
	mLevel = NULL;
	mDebugRen = NULL;

	PX_LOG( PX_NAME, TEXT("%s << %s ::"), PX_LOGP );
	unguard;
}


void FPxScene::InitLevel( ULevel* level )
{
	pxguard(FPxScene::InitLevel);
	PX_LOG( PX_NAME, TEXT("%s >> %s ::"), PX_LOGP );

	if( !level )
	{
		PX_ERR( PX_NAME, TEXT("%s !! %s :: No level provided"), PX_LOGP );
		return;
	}

	if( !mLevel )
	{
		CreateLevel(level);
	}
	else if( mLevel != level )
	{
		PX_LOG( PX_NAME, TEXT("%s !! %s :: Different level already loaded"), PX_LOGP );
		return;
	}

	// Add level actors
	for( INT i=0; i<level->Actors.Num(); i++ )
		if( level->Actors(i) )
			InitActor( level->Actors(i) );

	PX_LOG( PX_NAME, TEXT("%s << %s ::"), PX_LOGP );
	unguard;
}

void FPxScene::TerminateLevel()
{
	pxguard(FPxScene::TerminateLevel);
	PX_LOG( PX_NAME, TEXT("%s >> %s ::"), PX_LOGP );
		
	DestroyScene();

	PX_LOG( PX_NAME, TEXT("%s << %s ::"), PX_LOGP );
	unguard;
}

void FPxScene::CreateLevel( ULevel* level )
{
	pxguard(FPxScene::CreateLevel);
	PX_LOG( PX_NAME, TEXT("%s >> %s ::"), PX_LOGP );

	if( !level )
	{
		PX_ERR( PX_NAME, TEXT("%s !! %s :: No level provided"), PX_LOGP );
		return;
	}

	if( mLevel )
	{
		PX_ERR( PX_NAME, TEXT("%s !! %s :: Level already loaded"), PX_LOGP );
		return;
	}

	mLevel = level;

	// Triangulate level mesh
	PX_LOG( PX_NAME, TEXT("%s .. %s :: Triangulating Level"), PX_LOGP );
	FPxTriangulator tor;
	tor.Triangulate(mLevel->Model);
	TArray<INT>& tris = tor.Triangles;
	TArray<NxVec3> verts;
	for( TArray<FVector>::TIterator it_point(tor.Points); it_point; ++it_point )
	{
		verts.AddItem( ToNVS(*it_point) );
	}

    // Build physical model
    NxTriangleMeshDesc levelDesc;
    levelDesc.numVertices = verts.Num();
    levelDesc.numTriangles = tris.Num() / 3;
    levelDesc.pointStrideBytes = sizeof(NxVec3);
    levelDesc.triangleStrideBytes = 3*sizeof(INT);
    levelDesc.points = verts.GetData();
    levelDesc.triangles = tris.GetData();
    levelDesc.flags = NX_MF_FLIPNORMALS;

    // Cooking
	PX_LOG( PX_NAME, TEXT("%s .. %s :: Cooking Level"), PX_LOGP );
    NxInitCooking();
    FPxMemWriteBuffer buf;
    bool status = NxCookTriangleMesh(levelDesc, buf);
    NxTriangleMeshShapeDesc levelShapeDesc;
    levelShapeDesc.meshData = GPxPhysics.GetSDK().createTriangleMesh(FPxMemReadBuffer(buf.data));
	NxCloseCooking();

	// Calc level bounding box
	NxVec3 bmin, bmax;
	NxComputeBounds( bmin, bmax, verts.Num(), static_cast<NxVec3*>(verts.GetData()) );
	mBounds.set(bmin,bmax);

	// Update scene limits
	mSceneLimits.maxNbActors = 1;
	mSceneLimits.maxNbBodies = 1;
	mSceneLimits.maxNbStaticShapes = 1;
	mSceneLimits.maxNbDynamicShapes = 0;
	mSceneLimits.maxNbJoints = 0;

	// Update scene descriptor
	mSceneDesc.limits = &mSceneLimits;
	mSceneDesc.maxBounds = &mBounds;
	mSceneDesc.upAxis = 1; // Y
	mSceneDesc.staticStructure = NX_PRUNING_STATIC_AABB_TREE;
	mSceneDesc.dynamicStructure = NX_PRUNING_NONE; //NX_PRUNING_DYNAMIC_AABB_TREE;
	
	// create scene
	if( !mScene )
	{
		CreateScene();
	}
	if( !mScene )
	{
		PX_ERR( PX_NAME, TEXT("%s !! %s :: Scene could not be created"), PX_LOGP );
		return;
	}

	// default material
	NxMaterial* defaultMaterial = mScene->getMaterialFromIndex(0); 
	defaultMaterial->setRestitution(0.1f);
	defaultMaterial->setStaticFriction(0.9f);
	defaultMaterial->setDynamicFriction(0.85f);

	// Create static level actor
	PX_LOG( PX_NAME, TEXT("%s .. %s :: Creating level mesh"), PX_LOGP );
    NxActorDesc actorDesc;
    actorDesc.shapes.pushBack(&levelShapeDesc);

	// Add level actor to the scene
	PX_LOG( PX_NAME, TEXT("%s .. %s :: Creating level actor"), PX_LOGP );
    NxActor* actor = mScene->createActor(actorDesc);
    actor->userData = (void*)level->GetLevelInfo();

	PX_LOG( PX_NAME, TEXT("%s << %s ::"), PX_LOGP );
	unguard;
}

void FPxScene::Tick( FLOAT DeltaTime )
{
	pxguard(FPxScene::Tick);

	if( mScene )
	{
		mDebugRen = mScene->getDebugRenderable();
	}

	unguard;
}

void FPxScene::Render( FSceneNode* Frame )
{
	pxguard(FPxScene::Render);

	if( !mDebugRen || !GRenderDevice || !GPxPhysics.mDrawPhysX )
		return;

	FPlane linecolor;

    // Render points
	{
		static const NxReal ps = 4.0f;
		const NxDebugPoint* Points = mDebugRen->getPoints();
		NxU32 NbPoints = mDebugRen->getNbPoints();
		while(NbPoints--)
		{
			linecolor = ToColor(Points->color);
			GRenderDevice->Draw3DLine( Frame, linecolor, 0, ToFVS(Points->p)-FVector(ps,0,0), ToFVS(Points->p)+FVector(ps,0,0) );
			GRenderDevice->Draw3DLine( Frame, linecolor, 0, ToFVS(Points->p)-FVector(0,ps,0), ToFVS(Points->p)+FVector(0,ps,0) );
			GRenderDevice->Draw3DLine( Frame, linecolor, 0, ToFVS(Points->p)-FVector(0,0,ps), ToFVS(Points->p)+FVector(0,0,ps) );
			Points++;
		}
	}

    
    // Render lines
	{
		const NxDebugLine* Lines = mDebugRen->getLines();
		NxU32 NbLines = mDebugRen->getNbLines();
		while(NbLines--)
		{
			linecolor = ToColor(Lines->color);
			GRenderDevice->Draw3DLine( Frame, linecolor, 0, ToFVS(Lines->p0), ToFVS(Lines->p1) );
			Lines++;
		}
	}
    
    // Render triangles
	{
		const NxDebugTriangle* Triangles = mDebugRen->getTriangles();
		NxU32 NbTris = mDebugRen->getNbTriangles();
		while(NbTris--)
		{
			linecolor = ToColor(Triangles->color);
			GRenderDevice->Draw3DLine( Frame, linecolor, 0, ToFVS(Triangles->p0), ToFVS(Triangles->p1) );
			GRenderDevice->Draw3DLine( Frame, linecolor, 0, ToFVS(Triangles->p1), ToFVS(Triangles->p2) );
			GRenderDevice->Draw3DLine( Frame, linecolor, 0, ToFVS(Triangles->p2), ToFVS(Triangles->p0) );
			Triangles++;
		}
	}
	
	unguard;
}


void FPxScene::InitActor( AActor* uactor )
{
	pxguard(FPxScene::InitActor);
	
	// Only mesh decorations for now
	if( !uactor || !Cast<ADecoration>(uactor) || !uactor->Mesh  )
		return;

	PX_LOG( PX_NAME, TEXT("%s >> %s :: %s"), PX_LOGP, uactor->GetName() );

	uactor->setPhysics(PHYS_None);
	//uactor->SetCollisionSize( 0, 0 );

/*	// Convert mesh
	PX_LOG( PX_NAME, TEXT("%s .. %s :: Converting mesh"), PX_LOGP );
	TArray<INT> tris;
	for( TArray<FMeshTri>::TIterator it_tri(uactor->Mesh->Tris); it_tri; ++it_tri )
	{
		tris.AddItem( (*it_tri).iVertex[0] );
		tris.AddItem( (*it_tri).iVertex[1] );
		tris.AddItem( (*it_tri).iVertex[2] );
	}
	TArray<NxVec3> verts;
	for( TArray<FMeshVert>::TIterator it_vert(uactor->Mesh->Verts); it_vert; ++it_vert )
	{
		verts.AddItem( ToNVS((*it_vert).Vector()) );
	}

    // Build physical model
    NxTriangleMeshDesc meshDesc;
    meshDesc.numVertices = verts.Num();
    meshDesc.numTriangles = tris.Num() / 3;
    meshDesc.pointStrideBytes = sizeof(NxVec3);
    meshDesc.triangleStrideBytes = 3*sizeof(INT);
    meshDesc.points = verts.GetData();
    meshDesc.triangles = tris.GetData();
    meshDesc.flags = 0;

    // Cooking
	PX_LOG( PX_NAME, TEXT("%s .. %s :: Cooking mesh"), PX_LOGP );
    NxInitCooking();
    FPxMemWriteBuffer buf;
    bool status = NxCookTriangleMesh(meshDesc, buf);
    NxTriangleMeshShapeDesc meshShapeDesc;
    meshShapeDesc.meshData = GPxPhysics.GetSDK().createTriangleMesh(FPxMemReadBuffer(buf.data));
	NxCloseCooking();*/


	NxBodyDesc BodyDesc;
	//BodyDesc.angularDamping	= 0.5f;
	//BodyDesc.maxAngularVelocity	= 10.0f;
	//BodyDesc.flags |= NX_BF_KINEMATIC;

	NxBoxShapeDesc BoxDesc;
	BoxDesc.dimensions		= ToNVS(FVector(16.1f,16.1f,16.1f));

	NxActorDesc ActorDesc;
	ActorDesc.shapes.pushBack(&BoxDesc);
	ActorDesc.body			= &BodyDesc;
	ActorDesc.density		= 5.0f;
	ActorDesc.globalPose	= ToMat34(uactor->Rotation,uactor->Location);
	//ActorDesc.flags		   |= NX_BF_KINEMATIC | NX_BF_DISABLE_GRAVITY;


		 


	NxActor* xactor = mScene->createActor(ActorDesc);
	if( xactor )
	{
		xactor->userData = (void*)uactor;
		//uactor->NxActorPtr = (int)xactor;

		//xactor->raiseBodyFlag( NX_BF_KINEMATIC );
		//xactor->raiseBodyFlag( NX_BF_DISABLE_GRAVITY );

		GPxPhysics.MapActor(uactor,xactor);
	}

	PxDtUMesh(uactor->Mesh);

	
	PX_LOG( PX_NAME, TEXT("%s << %s :: %s"), PX_LOGP, uactor->GetName() );
	unguard;
}

void FPxScene::TerminateActor( AActor* uactor )
{
	pxguard(FPxScene::TerminateActor);
	//PX_LOG( PX_NAME, TEXT("%s .. %s :: %s"), PX_LOGP, uactor->GetName() );

	GPxPhysics.UnmapActor(uactor);
	
	unguard;
}



/*-----------------------------------------------------------------------------
	FPxSceneMain
-----------------------------------------------------------------------------*/

void FPxSceneMain::Tick( FLOAT DeltaTime )
{
	pxguard(FPxSceneMain::Tick);

	if( mScene )
	{
		//mCharManager->updateControllers();


		// 
		// Kinematic actors move unreal actors
		//
		/*NxActor** actors = mScene->getActors();
		int nbActors = mScene->getNbActors();
		while(nbActors--)
		{
			NxActor* actor = *actors++;
			if( actor->userData && actor->isDynamic() && actor->readBodyFlag( NX_BF_KINEMATIC ) )
			{
				AActor* aptr = (AActor*)actor->userData;

				// update location and rotation
				FCheckResult Hit(1.0f);
				NxMat34 m = actor->getGlobalPose();
				aptr->GetLevel()->MoveActor( aptr, FVector(0,0,0), ToRotator(m.M), Hit );
				aptr->GetLevel()->FarMoveActor( aptr, ToFVS(m.t), 0, 1 );
			}
		}*/

		// 
		// Unreal actors move kinematic actors
		//
		/*NxActor** actors = mScene->getActors();
		int nbActors = mScene->getNbActors();
		while(nbActors--)
		{
			NxActor* actor = *actors++;
			if( actor->userData && actor->isDynamic() && actor->readBodyFlag( NX_BF_KINEMATIC ) )
			{
				AActor* aptr = (AActor*)actor->userData;

				// update location and rotation
				NxMat34 m = ToMat34(aptr->Rotation,aptr->Location);
				actor->setGlobalPose(m);
			}
		}*/

		// 
		// Dynamic actors move unreal actors
		//
		NxActor** actors = mScene->getActors();
		int nbActors = mScene->getNbActors();
		while(nbActors--)
		{
			NxActor* actor = *actors++;
			if( actor->userData && actor->isDynamic() && !actor->readBodyFlag( NX_BF_KINEMATIC ) )
			{
				AActor* aptr = (AActor*)actor->userData;

				// update location and rotation
				FCheckResult Hit(1.0f);
				NxMat34 m = actor->getGlobalPose();
				aptr->GetLevel()->FarMoveActor( aptr, ToFVS(m.t) );
				aptr->GetLevel()->MoveActor( aptr, FVector(0,0,0), ToRotator(m.M), Hit );
			}
		}

		//
		// TODO: optimize timing
		//

		// Minimum acceptable framerate is 5fps
		if( DeltaTime > 0.2f )
		{
			DeltaTime = 0.2f;
		}

		mTime += DeltaTime;
		while( /*!mSimulating &&*/ mTime >= PX_TIMESTEP )
		{
			mTime -= PX_TIMESTEP;
			mScene->simulate(PX_TIMESTEP);
			mScene->flushStream();
			mScene->fetchResults(NX_RIGID_BODY_FINISHED, true);
			//mSimulating = true;
		}


        //renderScene();

        /*if( mSimulating && mScene->checkResults(NX_RIGID_BODY_FINISHED, false) )
        {
            mScene->fetchResults(NX_RIGID_BODY_FINISHED, true);
            mSimulating = false;

            //modifyScene(DeltaTime);
        }*/

				
		// get debug info
		if( GPxPhysics.mDrawPhysX )
			mDebugRen = mScene->getDebugRenderable();
	}

	unguard;
}


#pragma pack(pop)
/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/
