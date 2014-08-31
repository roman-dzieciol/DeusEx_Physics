/*=============================================================================
	PxCanvas.cpp
	Copyright 2007 Roman Dzieciol. All Rights Reserved.
=============================================================================*/

// Includes.
#include "PxMod.h"
#include "PxPhysics.h"


/*-----------------------------------------------------------------------------
	Implementation.
-----------------------------------------------------------------------------*/

// Class implementation.
IMPLEMENT_CLASS(UPxCanvas);

PXMOD_API FPxLineBatcher GPxLineBatcher;


/*=============================================================================
	UPxCanvas ::
=============================================================================*/

void UPxCanvas::Update( FSceneNode* Frame )
{
	pxguard(UPxCanvas::Update);

	UCanvas::Update( Frame );

	if( !Frame )	
		return;

	GPxPhysics.Render( Frame );
	GPxLineBatcher.Render( Frame );

	/*if( !Render )	return;
	if( !Viewport )	return;
	if( !Viewport->RenDev )	return;
	if( !Frame->Level )	return;
	if( !Frame->Level->Model )	return;*/

	unguard;
}



/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/
