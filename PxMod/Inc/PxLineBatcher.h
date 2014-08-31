/*=============================================================================
	PxLineBatcher.h
	Copyright 2007 Roman Dzieciol. All Rights Reserved.
=============================================================================*/



/*-----------------------------------------------------------------------------
	FPxLineBatcher.
-----------------------------------------------------------------------------*/


struct PXMOD_API FPxLine
{
	FVector End;
	FVector Start;
	FColor Color;

	FPxLine( const FVector& end, const FVector& start, const FColor& color )
		: End(end), Start(start), Color(color){}
};

struct PXMOD_API FPxLineBatcher
{
	TArray<FPxLine> Lines;
	bool bEnabled;

	FPxLineBatcher() : bEnabled(true)
	{
	}

	void Toggle()
	{
		bEnabled = !bEnabled;
	}

	void Add( const FPxLine& line ) { Lines.AddItem(line); }
	void Add( const FVector& end, const FVector& start, BYTE r, BYTE g, BYTE b ) { Lines.AddItem(FPxLine(end,start,FColor(r,g,b))); }
	void Empty() { Lines.Empty(); }
	
	void Render( FSceneNode* Frame )
	{
		guard(FPxLineBatcher::Render);

		if( !bEnabled )
			return;

		static float lw = 4;

		for( TArray<FPxLine>::TIterator Itb(GPxLineBatcher.Lines); Itb; ++Itb )
		{
			FVector vr = VRand();
			FVector start = (*Itb).Start+vr;
			FVector end = (*Itb).End+vr;
			GRenderDevice->Draw3DLine( Frame, (*Itb).Color.Plane(), LINE_DepthCued, end, start );
			GRenderDevice->Draw3DLine( Frame, (*Itb).Color.Plane(), LINE_DepthCued, start+FVector(-1,0,0)*lw, start+FVector(1,0,0)*lw );
			GRenderDevice->Draw3DLine( Frame, (*Itb).Color.Plane(), LINE_DepthCued, start+FVector(0,-1,0)*lw, start+FVector(0,1,0)*lw );
			GRenderDevice->Draw3DLine( Frame, (*Itb).Color.Plane(), LINE_DepthCued, start+FVector(0,0,-1)*lw, start+FVector(0,0,1)*lw );
		}

		unguard;
	}
};



/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/
