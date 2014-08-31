/*=============================================================================
	PxUtil.h
	Copyright 2006 Roman Dzieciol. All Rights Reserved.
=============================================================================*/



/*-----------------------------------------------------------------------------
	Files.
-----------------------------------------------------------------------------*/
inline void PxDeleteDirectoryContents( const TCHAR* Path )
{
	pxguard(PxDeleteDirectoryContents);
	if( !appStrlen(Path) )
		return;
	FString Spec = FString(Path) * TEXT("*");
	TArray<FString> List = GFileManager->FindFiles( *Spec, 1, 0 );
	for( INT i=0; i<List.Num(); i++ )
	{
		GFileManager->Delete(*(FString(Path) * List(i)),1,1);
	}
	unguard;
}


/*-----------------------------------------------------------------------------
	Log.
-----------------------------------------------------------------------------*/

inline FString xTime( bool bShort=false )
{	
	INT tYear, tMonth, tDayOfWeek, tDay, tHour, tMin, tSec, tMSec;
	appSystemTime( tYear, tMonth, tDayOfWeek, tDay, tHour, tMin, tSec, tMSec );
	FString Result = FString::Printf( TEXT("[%02d:%02d:%03d]"), tMin, tSec, tMSec );
	return Result;
}

/*-----------------------------------------------------------------------------
	ToStr :: FName
-----------------------------------------------------------------------------*/
inline FString ToStr( const FName p, bool bShort=false )
{
	return *p;
}


/*-----------------------------------------------------------------------------
	ToStr :: FVector
-----------------------------------------------------------------------------*/
inline FString ToStr( const FVector p, bool bShort=false )
{
	return FString::Printf(TEXT("%.02f,%.02f,%.02f"), p.X, p.Y, p.Z); 
}


/*-----------------------------------------------------------------------------
	ToStr :: FRotator
-----------------------------------------------------------------------------*/
inline FString ToStr( const FRotator p, bool bShort=false )
{
	return FString::Printf(TEXT("%d,%d,%d"), p.Pitch, p.Yaw, p.Roll); 
}


/*-----------------------------------------------------------------------------
	ToStr :: UBOOL
-----------------------------------------------------------------------------*/
inline FString ToStr( const UBOOL p, bool bShort=false )
{
	return FString::Printf(TEXT("%d"), p); 
}


/*-----------------------------------------------------------------------------
	ToStr :: UObject
-----------------------------------------------------------------------------*/
inline FString ToStr( UObject* p, bool bShort=false )
{
	FString s = FString::Printf(TEXT("ON:%p"), p); 
	if( p == NULL || bShort )
		return s;
	
	s += PX_SEP + FString::Printf(TEXT("%s"),p->GetFullName());
	s += PX_SEP + FString::Printf(TEXT("%.08x"),p->GetFlags());
	return s;
}


/*-----------------------------------------------------------------------------
	ToStr :: AActor
-----------------------------------------------------------------------------*/
inline FString ToStr( AActor* p, bool bShort=false )
{
	FString s = FString::Printf(TEXT("AC:%p"), p); 
	if( p == NULL || bShort )
		return s;
	
	s += PX_SEP + FString::Printf(TEXT("%s"),p->GetFullName());
	s += PX_SEP + FString::Printf(TEXT("%.08x"),p->GetFlags());
	s += PX_SEP + FString::Printf(TEXT("%d%d%d"),p->Role,p->RemoteRole,p->Level->NetMode);
	
	return s;
}

/*-----------------------------------------------------------------------------
	PxGetName
-----------------------------------------------------------------------------*/
inline FString PxGetName( UObject* p, bool bShort=false )
{
	return p ? p->GetName() : TEXT("None");
}

/*-----------------------------------------------------------------------------
	PxGetPath
-----------------------------------------------------------------------------*/
inline FString PxGetPathName( UObject* p, bool bShort=false )
{
	return p ? p->GetPathName() : TEXT("None");
}

/*-----------------------------------------------------------------------------
	ToStr :: ETravelType
-----------------------------------------------------------------------------*/
inline FString ToStr( ETravelType a, bool bShort=false )
{
	switch( a )
	{
	case TRAVEL_Absolute: return FString(TEXT("ABSOLUTE"));
	case TRAVEL_Partial: return FString(TEXT("PARTIAL"));
	case TRAVEL_Relative: return FString(TEXT("RELATIVE"));
	default: return FString(TEXT("UNKNOWN"));
	}
}


/*-----------------------------------------------------------------------------
	ToStr :: ENetRole
-----------------------------------------------------------------------------*/
inline FString ToStr( ENetRole a, bool bShort=false )
{
	switch( a )
	{
	case ROLE_None: return FString(TEXT("ROLE_None"));
	case ROLE_DumbProxy: return FString(TEXT("ROLE_DumbProxy"));
	case ROLE_SimulatedProxy: return FString(TEXT("ROLE_SimulatedProxy"));
	case ROLE_AutonomousProxy: return FString(TEXT("ROLE_AutonomousProxy"));
	case ROLE_Authority: return FString(TEXT("ROLE_Authority"));
	default: return FString(TEXT("UNKNOWN"));
	}
}

/*-----------------------------------------------------------------------------
	ToStr :: FMemStack
-----------------------------------------------------------------------------*/
inline FString ToStr( FMemStack* p, bool bShort=false )
{
	return FString::Printf(TEXT("MS:%p %d"), p, p->GetByteCount()); 
}

/*-----------------------------------------------------------------------------
	ToStr :: FCheckResult
-----------------------------------------------------------------------------*/
inline FString ToStr( FCheckResult* c, bool bShort=false )
{
	FString S;
	int i = 0;

	while( c )
	{
		FString aname;
		if( c->Actor )
			aname = c->Actor->GetFullName();

		FString pname;
		if( c->Primitive )
			pname = c->Primitive->GetFullName();

		S += FString::Printf( TEXT("FCR[%d]< P[%s] L[%s] N[%s] T[%f] I[%d] A[%s] N[%p] > ")
		, i
		, *pname
		, *ToStr(c->Location)
		, *ToStr(c->Normal)
		, c->Time
		, c->Item
		, *aname
		, c->Next
		);


		i++;
		c = c->GetNext();
	}

	return S;
}



/*-----------------------------------------------------------------------------
	ToStr :: UChannel
-----------------------------------------------------------------------------*/
inline FString ToStr( UChannel* p, bool bShort=false )
{
	FString s = FString::Printf(TEXT("CH:%p"), p); 
	if( p == NULL || bShort )
		return s;

	switch( p->ChType )
	{
		case CHTYPE_None:		s += PX_SEP + FString::Printf(TEXT("NONE"));  break;
		case CHTYPE_Control:	s += PX_SEP + FString::Printf(TEXT("CTRL"));  break;
		case CHTYPE_Actor:		s += PX_SEP + FString::Printf(TEXT("ACTOR"));  break;
		case CHTYPE_File:		s += PX_SEP + FString::Printf(TEXT("FILE"));  break;
		default:				s += PX_SEP + FString::Printf(TEXT("UNKNOWN"));  break;
	}

	s += PX_SEP + FString::Printf(TEXT("%p"), p->Connection); 
	s += PX_SEP + FString::Printf(TEXT("%d"), p->OpenAcked); 
	s += PX_SEP + FString::Printf(TEXT("%d"), p->Closing); 
	s += PX_SEP + FString::Printf(TEXT("%d"), p->ChIndex); 
	s += PX_SEP + FString::Printf(TEXT("%d"), p->OpenedLocally); 
	s += PX_SEP + FString::Printf(TEXT("%d"), p->OpenPacketId); 
	s += PX_SEP + FString::Printf(TEXT("%d"), p->OpenTemporary); 
	s += PX_SEP + FString::Printf(TEXT("%d"), p->NumInRec); 
	s += PX_SEP + FString::Printf(TEXT("%d"), p->NumOutRec); 
	s += PX_SEP + FString::Printf(TEXT("%p"), p->InRec); 
	s += PX_SEP + FString::Printf(TEXT("%p"), p->OutRec); 


	switch( p->ChType )
	{
		case CHTYPE_None: 
		{
			break;
		}

		case CHTYPE_Control:
		{
			break;
		}

		case CHTYPE_Actor:
		{
			break;
		}

		case CHTYPE_File: 
		{
			UFileChannel* fc = (UFileChannel*)p; 
			s += PX_SEP + FString::Printf(TEXT("%s"), fc->Filename); 
			s += PX_SEP + FString::Printf(TEXT("%s"), fc->PrettyName); 
			s += PX_SEP + FString::Printf(TEXT("%s"), fc->Error); 
			s += PX_SEP + FString::Printf(TEXT("%p"), fc->FileAr); 
			s += PX_SEP + FString::Printf(TEXT("%d"), fc->Transfered); 
			s += PX_SEP + FString::Printf(TEXT("%d"), fc->PackageIndex); 
			break;
		}

		default:
		{
			break;
		}
	}
	
	return s;
}




/*-----------------------------------------------------------------------------
	ToStr :: UNetConnection
-----------------------------------------------------------------------------*/
inline FString ToStr( UNetConnection* p, bool bShort=false )
{
	FString s = FString::Printf(TEXT("NC:%p"), p); 
	if( p == NULL || bShort )
		return s;

	switch( p->State )
	{
		case USOCK_Invalid:	s += PX_SEP + FString::Printf(TEXT("Invalid")); break;
		case USOCK_Closed:	s += PX_SEP + FString::Printf(TEXT("Closed")); break;
		case USOCK_Pending:	s += PX_SEP + FString::Printf(TEXT("Pending")); break;
		case USOCK_Open:	s += PX_SEP + FString::Printf(TEXT("Open")); break;
		default:			s += PX_SEP + FString::Printf(TEXT("Unknown")); break;
	}

	s += PX_SEP + FString::Printf(TEXT("%s"),*p->URL.String());
	s += PX_SEP + FString::Printf(TEXT("%.8x"),p->UserFlags);
	//s += PX_SEP + FString::Printf(TEXT(" | %s"),*p->LowLevelDescribe());

	for( TArray<UChannel*>::TIterator it=p->OpenChannels; it; ++it )
	{
		UChannel* c = *it;
		s += FString::Printf(TEXT("<%s>"),*ToStr(c));
	}
	
	return s;
}


/*-----------------------------------------------------------------------------
	ToStr :: FPackageInfo
-----------------------------------------------------------------------------*/
inline FString ToStr( FPackageInfo* p, bool bShort=false )
{
	FString s = FString::Printf(TEXT("PI:%p"), p); 
	if( p == NULL || bShort )
		return s;

	s += PX_SEP + FString::Printf(TEXT("%s"),*p->URL);
	s += PX_SEP + FString::Printf(TEXT("%p"),p->Linker);
	//s += PX_SEP + FString::Printf(TEXT("%p"),p->Parent);
	s += PX_SEP + FString::Printf(TEXT("%s"),p->Guid.String());
	s += PX_SEP + FString::Printf(TEXT("%d"),p->FileSize);
	//s += PX_SEP + FString::Printf(TEXT("%d"),p->ObjectBase);
	//s += PX_SEP + FString::Printf(TEXT("%d"),p->ObjectCount);
	//s += PX_SEP + FString::Printf(TEXT("%d"),p->NameBase);
	//s += PX_SEP + FString::Printf(TEXT("%d"),p->NameCount);
	//s += PX_SEP + FString::Printf(TEXT("%d"),p->LocalGeneration);
	//s += PX_SEP + FString::Printf(TEXT("%d"),p->RemoteGeneration);
	//s += PX_SEP + FString::Printf(TEXT("%d"),p->PackageFlags);
	return s;
}

/*-----------------------------------------------------------------------------
	ToStr :: UPackageMap
-----------------------------------------------------------------------------*/
inline FString ToStr( UPackageMap* p, bool bShort=false )
{
	FString s = FString::Printf(TEXT("PM:%p"), p); 
	if( p == NULL || bShort )
		return s;

	/*for( TArray<FPackageInfo>::TIterator it(p->List); it; ++it )
	{
		FPackageInfo* f = &(*it)
		s += TEXT("\n") + FString::Printf(TEXT("%s"),*ToStr(f));
	}*/
	return s;
}


/*-----------------------------------------------------------------------------
	ToStr :: FNetworkNotify
-----------------------------------------------------------------------------*/
inline FString ToStr( FNetworkNotify* p, bool bShort=false )
{
	FString s = FString::Printf(TEXT("NN:%p"), p); 
	if( p == NULL || bShort )
		return s;
	
	return s;
}


/*-----------------------------------------------------------------------------
	ToStr :: ULevel
-----------------------------------------------------------------------------*/
inline FString ToStr( ULevel* p, bool bShort=false )
{
	FString s = FString::Printf(TEXT("LV:%p"), p); 
	if( p == NULL || bShort )
		return s;
	
	s += PX_SEP + FString::Printf(TEXT("%s"),p->GetFullName());
	return s;
}


/*-----------------------------------------------------------------------------
	ToStr :: UPendingLevel
-----------------------------------------------------------------------------*/
inline FString ToStr( UPendingLevel* p, bool bShort=false )
{
	FString s = FString::Printf(TEXT("PL:%p"), p); 
	if( p == NULL || bShort )
		return s;
	
	s += PX_SEP + FString::Printf(TEXT("%s"),p->GetFullName());
	return s;
}


/*-----------------------------------------------------------------------------
	ToStr :: FFileManager
-----------------------------------------------------------------------------*/
inline FString ToStr( FFileManager* p, bool bShort=false )
{
	FString s = FString::Printf(TEXT("FM:%p"), p); 
	if( p == NULL || bShort )
		return s;
	return s;
}



/*-----------------------------------------------------------------------------
	PxExplodePath
-----------------------------------------------------------------------------*/
inline void PxExplodePath( const FString& fullpath, FString& path, FString& file, FString& name, FString& ext )
{
	if( !fullpath.Split(TEXT("\\"),&path,&file,TRUE) )
		file = fullpath;

	if( !file.Split(TEXT("."),&name,&ext,TRUE) )
		name = file;
}

/*-----------------------------------------------------------------------------
	PxDumpLevels
-----------------------------------------------------------------------------*/
inline void PxDumpLevels()
{
	pxguard(PxDumpLevels);
	PX_LOG( PX_NAME, TEXT("%s >> %s ::"), PX_LOGP );

	for( TObjectIterator<ULevel> li; li; ++li )
	{
		PX_LOG( PX_NAME, TEXT("%s -> %s ::"), *xTime(), *ToStr(*li) );
		for( TTransArray<AActor*>::TIterator ai((*li)->Actors); ai; ++ai )
		{
			AActor* ap = *ai;
			if( !ap )
			{
				PX_LOG( PX_NAME, TEXT("%s -- %s :: [%s]"), *xTime(), *ToStr(*li), *ToStr(ap) );
				continue;
			}
			PX_LOG( PX_NAME, TEXT("%s -- %s :: [%s] [%d] [%d]"), *xTime(), *ToStr(*li), *ToStr(ap), ap->bJustTeleported, ap->bTicked  );

		}
		
		PX_LOG( PX_NAME, TEXT("%s <- %s ::"), *xTime(), *ToStr(*li) );
	}

	PX_LOG( PX_NAME, TEXT("%s << %s ::"), PX_LOGP );
	unguard;
}

/*-----------------------------------------------------------------------------
	PxDumpTravelInfo
-----------------------------------------------------------------------------*/
inline void PxDumpTravelInfo( const TMap<FString,FString>* TravelInfo )
{
	pxguard(PxDumpTravelInfo);
	PX_LOG( PX_NAME, TEXT("%s >> %s ::"), PX_LOGP );

	if( TravelInfo )
	{
		TMap<FString,FString> tmap = *TravelInfo;
		for( TMap<FString,FString>::TIterator it(tmap); it; ++it )
		{
			// TravelInfo contains more data than GLog can handle
			FString& s = it.Value();
			int i = s.InStr(TEXT("\r\n"));
			if( i != -1 )
				s = s.Left(i);
			PX_LOG( PX_NAME, TEXT("%s -- %s :: [%s],[%s]"), PX_LOGP, *it.Key(), *s  );
			//break; 
		}
	}

	PX_LOG( PX_NAME, TEXT("%s << %s ::"), PX_LOGP );
	unguard;
}


/*-----------------------------------------------------------------------------
	PxDumpRefs
-----------------------------------------------------------------------------*/


inline void PxDumpRefs()
{
	pxguard(PxDumpRefs);
	PX_LOG( PX_NAME, TEXT("%s >> %s :: Searching for actor references in non-actor objects.."), PX_LOGP );

	int counter=0;

	for( FObjectIterator oi; oi; ++oi )
	{
		UObject* op = *oi;
		
		// ignore actor classes
		UClass* cp = op->GetClass();
		if( !cp || cp->IsChildOf(AActor::StaticClass()) || !cp->RefLink  )
			continue;
		
		for( UObjectProperty* ref=cp->RefLink; ref; ref=ref->NextReference )
		{
			UClass* rcls = ref->PropertyClass;
			
			// only actor references pass through
			if( !rcls->IsChildOf(AActor::StaticClass()) )
				continue;

			// get var value
			TCHAR Temp[1024] = TEXT("");
			ref->ExportText( 0, Temp, (BYTE*)op, (BYTE*)op, PPF_Localized );

			// ignore empty
			static const FString nullstr(TEXT("None"));
			if( nullstr == Temp )
				continue;

			PX_LOG( PX_NAME, TEXT("%s -- %s :: %s.%s = %s"), PX_LOGP, op->GetPathName(), ref->GetName(), Temp );
			++counter;
		}
	}

	PX_LOG( PX_NAME, TEXT("%s << %s :: Found %d references."), PX_LOGP, counter );
	unguard;
}


/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/