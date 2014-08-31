/*=============================================================================
	PxDt.cpp
	Copyright 2006 Roman Dzieciol. All Rights Reserved.
=============================================================================*/

#include "PxMod.h"
#include "PxDtDefs.h"
#include "PxPhysics.h"

/*-----------------------------------------------------------------------------
	UMesh
-----------------------------------------------------------------------------*/

PX_DETOUR_CLASS( UMesh, UBOOL, LineCheck, ( FCheckResult& Result, AActor* Owner, FVector End, FVector Start, FVector Size, DWORD ExtraNodeFlags ) )
{
	UBOOL bResult = TRUE;
	NxActor* nActor = GPxPhysics.GetNxActor(Owner);


	if( Cast<ADecoration>(Owner) && nActor )
	{
		NxU32 nbShapes = nActor->getNbShapes();
		if( !nbShapes )
			return TRUE;

		if( Size.IsZero() )
		{
			NxRay ray;
			ray.orig = ToNVS(Start);
			ray.dir = ToNV(FVector(End-Start).SafeNormal());
			NxReal dist = ToNS(FVector(End-Start).Size());
			NxU32 hint = NX_RAYCAST_IMPACT  | NX_RAYCAST_DISTANCE | NX_RAYCAST_FACE_NORMAL;

			NxRaycastHit hit, temphit;
/*
			NxActorGroup oldGroup = nActor->getGroup();
			nActor->setGroup(31);
			
			nActor->setGroup(oldGroup);


			nActor->getScene()->raycastClosestShape( ray, NX_ALL_SHAPES, hit, dist, hint
*/
/*
			mScene
			virtual NxShape* NxScene::raycastClosestShape  (  const NxRay &  worldRay,  
  NxShapesType  shapeType,  
  NxRaycastHit &  hit,  
  NxU32  groups = 0xffffffff,  
  NxReal  maxDist = NX_MAX_F32,  
  NxU32  hintFlags = 0xffffffff,  
  const NxGroupsMask *  groupsMask = NULL,  
  NxShape **  cache = NULL 
 )  const [pure virtual] 
 
*/

			NxShape** slist = (NxShape **)nActor->getShapes();
			for( NxU32 i=0; i<nbShapes; ++i )
			{
				NxShape* shape = slist[i];
				if( shape )
				{
					if( shape->raycast( ray, dist, hint, temphit, true ) )
					{
						if( temphit.distance <= hit.distance || bResult == TRUE )
						{
							hit = temphit;
							hit.shape = shape;
							bResult = FALSE;
						}
					}
				}
			}


			if( bResult == FALSE )
			{
				Result.Location = ToFVS(hit.worldImpact);
				Result.Normal = ToFV(hit.worldNormal);
				Result.Primitive = NULL;
				Result.Time = hit.distance / dist;
				Result.Item = INDEX_NONE;
				Result.Actor = Owner;
			}
		}
		else
		{
			bResult = PX_DETOUR_TP( LineCheck(Result,Owner,End,Start,Size,ExtraNodeFlags) );
			return bResult;
		}

		{
			//GPxLineBatcher.Add(End,Start,255,127,255);
			
			//bResult = PX_DETOUR_TP( LineCheck(Result,Owner,End,Start,Size,ExtraNodeFlags) );

			//if( bResult == 0 )
			//	_asm int 3;
			//PX_LOG( PX_NAME, TEXT("%s .. %s :: [%s],[%s],[%s],[%s],[%d] [%d][%s] "), PX_LOGP, *ToStr(Owner), *ToStr(End), *ToStr(Start), *ToStr(Size), ExtraNodeFlags, bResult, *ToStr(Result.Actor) );
			
			NxVec3 ray = ToNVS(FVector(End-Start));
			NxReal dist = ToNS(FVector(End-Start).Size());


			// check overlap
			NxShape** slist = (NxShape **)nActor->getShapes();
			for( NxU32 i=0; i<nbShapes; ++i )
			{
				NxShape* shape = slist[i];
				if( shape )
				{
					NxBoxShape* boxShape = shape->isBox();
					if( boxShape )
					{
						NxBox wbox;
						boxShape->getWorldOBB(wbox);
						wbox.center = ToNVS(Start);

						if( shape->checkOverlapOBB( wbox ) )
						{
							Result.Location = Start;
							Result.Normal = FVector(Start-End).SafeNormal();
							Result.Primitive = NULL;
							Result.Time = 0;
							Result.Item = INDEX_NONE;
							Result.Actor = Owner;
							bResult = FALSE;
							break;
						}
						else
						{
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
						}
					}
				}
			}
		}
	}
	else
	{
		//PX_LOG( PX_NAME, TEXT("%s >> %s :: [%s],[%s],[%s],[%s],[%s] "), PX_LOGP, *ToStr(Owner), *ToStr(End), *ToStr(Start), *ToStr(Size), *ToStr(ExtraNodeFlags) );
		bResult = PX_DETOUR_TP( LineCheck(Result,Owner,End,Start,Size,ExtraNodeFlags) );
		//PX_LOG( PX_NAME, TEXT("%s << %s :: [%s],[%s],[%s],[%s],[%s] [%s][%s] "), PX_LOGP, *ToStr(Owner), *ToStr(End), *ToStr(Start), *ToStr(Size), *ToStr(ExtraNodeFlags), *ToStr(bResult), *ToStr(&Result) );
	}
	return bResult;
}}


void PxDtUMesh( UMesh* p )
{
	pxguard(PxDtUMesh);
	PX_LOG( PX_NAME, TEXT("%s ## %s :: [%s]"), PX_LOGP, *ToStr(p) );
	if( p == NULL )
		return;

	PBYTE UMesh = PBYTE(p);
	PX_DETOUR_VR( UMesh, 0x00, 0x58, LineCheck );

/*	FCheckResult cr;
	FVector a,b,c,d;

	_asm nop;
	_asm int 3;
	p->LineCheck(cr,0,a,b,c,9);
	_asm nop;*/

/*
	_asm nop;
	_asm int 3;
	//PBYTE mlvl = (PBYTE)GLevel;			// ULevel data
	PBYTE pvtbl = mlvl + 0x28;			// pointer to FNetworkNotify vtable
	_asm nop;
	PBYTE mvtbl = *(PBYTE*)pvtbl;		// vtable data
	PBYTE pfunc = mvtbl + 0x0C;			// pointer to NotifyGetLevel stub
	_asm nop;
	PBYTE mstub = *(PBYTE*)pfunc;		// NotifyGetLevel stub data
	//PBYTE pfinal = DetourGetFinalCode(mstub,TRUE);
	//PBYTE pfunc = mvtbl + 0x0C;			// pointer to NotifyGetLevel stub
	_asm nop;
	//int* pstub = *(int**)pvtbl + 0x0C;
	_asm nop;
	//GLevel->NotifyGetLevel();
	_asm nop;
*/

	/*PX_DETOUR_VR( ULevel, 0x00, 0x20, PostLoad );
	PX_DETOUR_VR( ULevel, 0x00, 0x24, Destroy );
	//PX_DETOUR_VR( ULevel, 0x00, 0x5C, Tick );
	PX_DETOUR_VR( ULevel, 0x00, 0x6c, ReconcileActors );
	PX_DETOUR_VR( ULevel, 0x00, 0x70, RememberActors );
	PX_DETOUR_VR( ULevel, 0x00, 0x78, ShrinkLevel );
	//PX_DETOUR_VR( ULevel, 0x00, 0x7c, CompactActors );
	PX_DETOUR_VR( ULevel, 0x00, 0x80, Listen );
	PX_DETOUR_VR( ULevel, 0x00, 0x94, DestroyActor );
	PX_DETOUR_VR( ULevel, 0x00, 0x98, CleanupDestroyed );
	PX_DETOUR_VR( ULevel, 0x00, 0x9c, SpawnActor );
	PX_DETOUR_VR( ULevel, 0x00, 0xa4, SpawnViewActor );
	PX_DETOUR_VR( ULevel, 0x00, 0xa8, SpawnPlayActor );
	PX_DETOUR_VR( ULevel, 0x00, 0xe4, WelcomePlayer );*/


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

	

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/