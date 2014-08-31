/*=============================================================================
	PxPhysicsUtil.h
	Copyright 2006 Roman Dzieciol. All Rights Reserved.
=============================================================================*/
#ifndef PXPHYSICSUTIL_H
#define PXPHYSICSUTIL_H
#pragma pack(push, 8)



/*-----------------------------------------------------------------------------
	PhysX Conversion functions.
-----------------------------------------------------------------------------*/

inline FLOAT ToUS( NxReal s )	{ return s*PX_SCALE; }
inline NxReal ToNS( FLOAT s )	{ return s/PX_SCALE; }

inline FVector ToFV( NxVec3 v )		{ return FVector(v.x,v.z,v.y); }
inline FVector ToFVR( NxVec3 v )	{ return FVector(v.x,v.y,v.z); }
inline FVector ToFVS( NxVec3 v )	{ return FVector(v.x,v.z,v.y)*PX_SCALE; }

inline NxVec3 ToNV( FVector v )		{ return NxVec3(v.X,v.Z,v.Y); }
inline NxVec3 ToNVR( FVector v )	{ return NxVec3(v.X,v.Y,v.Z); }
inline NxVec3 ToNVS( FVector v )	{ return NxVec3(v.X,v.Z,v.Y)/PX_SCALE; }

inline FRotator ToRotator( NxMat33 m )
{
	return FCoords( FVector(0,0,0), ToFV(m.getColumn(0)), ToFV(m.getColumn(2)), ToFV(m.getColumn(1)) ).OrthoRotation();
}

inline NxMat33 ToMat33( FRotator Rot )
{
	// Z=forward, X=left, Y=up ?
	//GLog->Logf( TEXT("%d %d %d"), Rot.Pitch, Rot.Yaw, Rot.Roll );

	NxQuat q1,q2,q3;
	q1.fromAngleAxisFast( UUTORAD * (float)Rot.Roll,	NxVec3(1,0,0) );	// X = forward = Roll
	q2.fromAngleAxisFast( UUTORAD * (float)Rot.Yaw,		NxVec3(0,-1,0) );	// Y = up = -Yaw
	q3.fromAngleAxisFast( UUTORAD * (float)Rot.Pitch,	NxVec3(0,0,1)) ;	// Z = right = Pitch
	return NxMat33(q2 * q3 * q1);
}

inline NxMat34 ToMat34( FRotator Rot, FVector Loc )
{
	return NxMat34( ToMat33(Rot), ToNVS(Loc) );
}

inline FPlane ToColor( NxU32 color )
{
	return FPlane( NxF32((color>>16)&0xff)/255.0f, NxF32((color>>8)&0xff)/255.0f, NxF32((color)&0xff)/255.0f, 1.0f );
}


/*-----------------------------------------------------------------------------
	Collision Flags.
-----------------------------------------------------------------------------*/

inline TArray<NxCollisionGroup> pushShapesGroup( NxActor* actor, NxCollisionGroup group )
{
    NxShape*const* shapes = actor->getShapes();
    NxU32 nShapes = actor->getNbShapes();

	TArray<NxCollisionGroup> groups(nShapes);

    while (nShapes--)
    {
		groups(nShapes) = shapes[nShapes]->getGroup();
        shapes[nShapes]->setGroup(group);
    }

	return groups;
}


inline void popShapesGroup( NxActor* actor, TArray<NxCollisionGroup> groups )
{
    NxShape*const* shapes = actor->getShapes();
    NxU32 nShapes = actor->getNbShapes();

    while (nShapes--)
    {
        shapes[nShapes]->setGroup(groups(nShapes));
    }
}

inline FString printShapesGroup( NxActor* actor )
{
    NxShape*const* shapes = actor->getShapes();
    NxU32 nShapes = actor->getNbShapes();
	FString S;

    while (nShapes--)
    {
		S += FString::Printf( TEXT("%d,"), shapes[nShapes]->getGroup() );
    }
	return S;
}	



#pragma pack(pop)
#endif
/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/
