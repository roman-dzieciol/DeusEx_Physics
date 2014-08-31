/*=============================================================================
	PxTriangulator.cpp
	Copyright 2006 Roman Dzieciol. All Rights Reserved.
=============================================================================*/
#pragma pack(push, 4)

#include "PxTriangulator.h"


/*-----------------------------------------------------------------------------
	FPxTriangulator
-----------------------------------------------------------------------------*/

/*
class FBspSurf
{
public:

	// Persistent info.
	UTexture*	Texture;		// 4 Texture map.
	DWORD		PolyFlags;		// 4 Polygon flags.
	INT			pBase;			// 4 Polygon & texture base point index (where U,V==0,0).
	INT			vNormal;		// 4 Index to polygon normal.
	INT			vTextureU;		// 4 Texture U-vector index.
	INT			vTextureV;		// 4 Texture V-vector index.
	INT			iLightMap;		// 4 Light mesh.
	INT			iBrushPoly;		// 4 Editor brush polygon index.
	SWORD		PanU;			// 2 U-Panning value.
	SWORD		PanV;			// 2 V-Panning value.
	ABrush*		Actor;			// 4 Brush actor owning this Bsp surface.
	TArray<FDecal>	Decals;		// 12 Array decals on this surface
	TArray<INT>	Nodes;			// 12 Nodes which make up this surface

	// Functions.
	ENGINE_API friend FArchive& operator<<( FArchive& Ar, FBspSurf& Surf );
};*/

struct FPxDirtyIndex
{
	bool bUsed;
	INT Index;
};

bool FPxTriangulator::Triangulate( UModel* model )
{
	pxguard(FPxTriangulator::Triangulate);

	// Triangulate convex polygons

	// Reset
	Points.Empty();
	Triangles.Empty();

	// Get points
	Points = model->Points;

	// Get faces
	FBspSurf surf;
	FBspNode node;
	for( TTransArray<FBspSurf>::TIterator it_surfs(model->Surfs); it_surfs; ++it_surfs )
	{
		surf = *it_surfs;
		FVector vert_adjust = model->Vectors(surf.vNormal)*1.0;

		for( TArray<INT>::TIterator it_nodes(surf.Nodes); it_nodes; ++it_nodes )
		{
			node = model->Nodes(*it_nodes);

			// skip invalid nodes
			int vert_count = node.NumVertices;
			if( vert_count < 3 )
				continue;

			int vert_offset = node.iVertPool;

			unsigned int point1 = model->Verts(vert_offset).pVertex;
			unsigned int point2 = model->Verts(vert_offset+1).pVertex;
			unsigned int point3 = model->Verts(vert_offset+2).pVertex;

			Triangles.AddItem( point1 );
			Triangles.AddItem( point2 );
			Triangles.AddItem( point3 );

			Points( point1 ) += vert_adjust;
			Points( point2 ) += vert_adjust;
			Points( point3 ) += vert_adjust;

			for( int j=3; j!=vert_count; ++j )
			{
				point2 = point3;
				point3 = model->Verts(vert_offset+j).pVertex;

				Triangles.AddItem( point1 );
				Triangles.AddItem( point2 );
				Triangles.AddItem( point3 );

				Points( point3 ) += vert_adjust;
			}
		}
	}

/*
	FBspNode node;
	for( TTransArray<FBspNode>::TIterator it_nodes(model->Nodes); it_nodes; ++it_nodes )
	{
		node = *it_nodes;

		// skip invalid nodes
		int vert_count = node.NumVertices;
		if( vert_count < 3 )
			continue;

		int vert_offset = node.iVertPool;

		unsigned int point1 = model->Verts(vert_offset).pVertex;
		unsigned int point2 = model->Verts(vert_offset+1).pVertex;
		unsigned int point3 = model->Verts(vert_offset+2).pVertex;

		Triangles.AddItem( point1 );
		Triangles.AddItem( point2 );
		Triangles.AddItem( point3 );

		for( int j=3; j!=vert_count; ++j )
		{
			point2 = point3;
			point3 = model->Verts(vert_offset+j).pVertex;
			Triangles.AddItem( point1 );
			Triangles.AddItem( point2 );
			Triangles.AddItem( point3 );
		}
	}*/

	// Remove unused points

	// init translation table
	TArray<int> translation;
	translation.AddZeroed( Points.Num() );
	for( int i=0; i!=Points.Num(); ++i )
		translation(i) = -1;

	// used points list
	TArray<FVector> points_list;

	INT point_old;
	INT point_new;
	INT point_trn;
	for( TArray<INT>::TIterator it_triangle(Triangles); it_triangle; ++it_triangle )
	{
		point_old = *it_triangle;
		point_trn = translation(point_old);

		if( point_trn == -1 )
		{
			// unique index
			point_new = points_list.Num();				// get new index
			translation(point_old) = point_new;			// save index translation
			points_list.AddItem( Points(point_old) );	// save point at new index
			*it_triangle = point_new;					// update Trangles item
		}
		else
		{
			// shared index
			*it_triangle = point_trn;					// update Trangles item
		}
	}

	if( Points.Num() != points_list.Num() )
	{
		PX_LOG( PX_NAME, TEXT("%s !! %s :: CULLED %d UNUSED POINTS! %d->%d"), PX_LOGP, (Points.Num()-points_list.Num()), Points.Num(), points_list.Num() );
	}

	Points = points_list;


	/*
	TArray<FPxDirtyIndex> points_used;
	points_used.AddZeroed( Points.Num() );

	TArray<FVector> points_new;
	TArray<int> triangles_new;

	INT idx_point;
	for( TArray<INT>::TIterator it_triangle(Triangles); it_triangle; ++it_triangle )
	{
		idx_point = *it_triangle;
		if( points_used(idx_point).bUsed == false )
		{
			points_used(idx_point).bUsed = true;
			points_used(idx_point).Index = idx_clean.Num();  

			new_points.AddItem( Points(idx_point) );
		}
		else
		{
			idx_clean.AddItem( Points(points_used(idx_point).Index) );
		}
	}*/

	/*for( TArray<INT>::TIterator it_idxtable(idxtable); it_idxtable; ++it_idxtable )
	{
		if( *it_idxtable == 0 )
		{
			//GLog->Logf( TEXT("UNUSED POINT: %d"), it_idxtable.GetIndex() );
		}
	}*/


/*
	Verts.AddZeroed( faces.Num() );
	for( int it_verts=0; it_verts!=Verts.Num(); ++it_verts )
	{
		Verts(it_verts) = model->Points( faces(it_verts) );
	}


	
	FBspNode node;
	for( TTransArray<FBspNode>::TIterator Itn(model->Nodes); Itn; ++Itn )
	{
		node = *Itn;
		if( node.NumVertices < 3 )
			continue;

		int idx = node.iVertPool;

		unsigned int i1 = allverts(idx+0);
		unsigned int i2 = allverts(idx+1);
		unsigned int i3 = allverts(idx+2);

		alltris.AddItem( i1 );
		alltris.AddItem( i2 );
		alltris.AddItem( i3 );

		for( unsigned int j=3; j<node.NumVertices; ++j )
		{
			i2 = i3;
			i3 = allverts(idx+j);
			alltris.AddItem( i1 );
			alltris.AddItem( i2 );
			alltris.AddItem( i3 );
		}
	}

	TArray<INT> vidx;
	vidx.AddZeroed( allverts.Num() );

	for( TArray<INT>::TIterator Itran(alltris); Itran; ++Itran )
	{
		alltris(i1);
		alltris(i2);
		alltris(i3);
	}*/

	return false;
	unguard;
}


#pragma pack(pop)
/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/