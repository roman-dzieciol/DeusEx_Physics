/*=============================================================================
	PxTriangulator.h
	Copyright 2006 Roman Dzieciol. All Rights Reserved.
=============================================================================*/
#ifndef PXTRIANGULATOR_H
#define PXTRIANGULATOR_H
#pragma pack(push, 4)

#include "PxMod.h"


/*-----------------------------------------------------------------------------
	FPxTriangulator
-----------------------------------------------------------------------------*/
class FPxTriangulator
{
public:

	TArray<FVector>		Points;
	TArray<INT>			Triangles;

	bool Triangulate( UModel* model );

};


#pragma pack(pop)
#endif
/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/