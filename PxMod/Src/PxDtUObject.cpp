/*=============================================================================
	PxDt.cpp
	Copyright 2006 Roman Dzieciol. All Rights Reserved.
=============================================================================*/

#include "PxMod.h"
#include "PxDtDefs.h"

PX_DETOUR_STATIC( UObject, UObject*, StaticConstructObject, ( UClass* Class, UObject* InOuter, FName Name, DWORD SetFlags, UObject* Template, FOutputDevice* Error ) )
{
	//PX_LOG( PX_NAME, TEXT("%s >> %s :: "), PX_LOGP );
	UObject* r = PX_DETOUR_STC( StaticConstructObject(Class,InOuter,Name,SetFlags,Template,Error) );
	if( Cast<USubsystem>(r) || Cast<UClient>(r) )
	{
		PX_LOG( PX_NAME, TEXT("%s ## %s :: [%s]"), PX_LOGP, r->GetFullName() );
	}
	//PX_LOG( PX_NAME, TEXT("%s << %s :: "), PX_LOGP );
	return r;
	PXDT_FINISH;
}


PX_DETOUR_STATIC( UObject, void, CollectGarbage, ( DWORD KeepFlags ) )
{
	PX_LOG( PX_NAME, TEXT("%s >> %s :: [%.8x]"), PX_LOGP, KeepFlags );
	PX_DETOUR_STC( CollectGarbage(KeepFlags) );
	//PxDumpRefs();
	//PxDumpLevels();
	PX_LOG( PX_NAME, TEXT("%s << %s :: [%.8x]"), PX_LOGP, KeepFlags );
	PXDT_FINISH;
}


PX_DETOUR_STATIC( UObject, UBOOL, IsReferenced, ( UObject*& Res, DWORD KeepFlags, UBOOL IgnoreReference ) )
{
	PX_LOG( PX_NAME, TEXT("%s >> %s :: [%s],[%.8x],[%d]"), PX_LOGP, *ToStr(Res),KeepFlags,IgnoreReference );
	UBOOL r = PX_DETOUR_STC( IsReferenced(Res,KeepFlags,IgnoreReference) );
	PX_LOG( PX_NAME, TEXT("%s << %s :: [%s],[%.8x],[%d] [%d]"), PX_LOGP, *ToStr(Res),KeepFlags,IgnoreReference, r );
	return r;
	PXDT_FINISH;
}

	

void PxDtsUObject()
{
	pxguard(PxDtsUObject);
	PX_LOG( PX_NAME, TEXT("%s ## %s :: "), PX_LOGP );
	//PX_DETOUR_MF(UObject,IsReferenced);
	PX_DETOUR_MF(UObject,CollectGarbage);
	PX_DETOUR_MF(UObject,StaticConstructObject);
	unguard;
}


/*

	// Systemwide functions.
	static UObject* StaticFindObject( UClass* Class, UObject* InOuter, const TCHAR* Name, UBOOL ExactClass=0 );
	static UObject* StaticFindObjectChecked( UClass* Class, UObject* InOuter, const TCHAR* Name, UBOOL ExactClass=0 );
	static UObject* StaticLoadObject( UClass* Class, UObject* InOuter, const TCHAR* Name, const TCHAR* Filename, DWORD LoadFlags, UPackageMap* Sandbox );
	static UClass* StaticLoadClass( UClass* BaseClass, UObject* InOuter, const TCHAR* Name, const TCHAR* Filename, DWORD LoadFlags, UPackageMap* Sandbox );
	static UObject* StaticAllocateObject( UClass* Class, UObject* InOuter=(UObject*)GetTransientPackage(), FName Name=NAME_None, DWORD SetFlags=0, UObject* Template=NULL, FOutputDevice* Error=GError, UObject* Ptr=NULL );
	static UObject* StaticConstructObject( UClass* Class, UObject* InOuter=(UObject*)GetTransientPackage(), FName Name=NAME_None, DWORD SetFlags=0, UObject* Template=NULL, FOutputDevice* Error=GError );
	static void StaticInit();
	static void StaticExit();
	static UBOOL StaticExec( const TCHAR* Cmd, FOutputDevice& Ar=*GLog );
	static void StaticTick();
	static UObject* LoadPackage( UObject* InOuter, const TCHAR* Filename, DWORD LoadFlags );
	static UBOOL SavePackage( UObject* InOuter, UObject* Base, DWORD TopLevelFlags, const TCHAR* Filename, FOutputDevice* Error=GError, ULinkerLoad* Conform=NULL );
	static void CollectGarbage( DWORD KeepFlags );
	static void SerializeRootSet( FArchive& Ar, DWORD KeepFlags, DWORD RequiredFlags );
	static UBOOL IsReferenced( UObject*& Res, DWORD KeepFlags, UBOOL IgnoreReference );
	static UBOOL AttemptDelete( UObject*& Res, DWORD KeepFlags, UBOOL IgnoreReference );
	static void BeginLoad();
	static void EndLoad();
	static void InitProperties( BYTE* Data, INT DataCount, UClass* DefaultsClass, BYTE* Defaults, INT DefaultsCount );
	static void ExitProperties( BYTE* Data, UClass* Class );
	static void ResetLoaders( UObject* InOuter, UBOOL DynamicOnly, UBOOL ForceLazyLoad );
	static UPackage* CreatePackage( UObject* InOuter, const TCHAR* PkgName );
	static ULinkerLoad* GetPackageLinker( UObject* InOuter, const TCHAR* Filename, DWORD LoadFlags, UPackageMap* Sandbox, FGuid* CompatibleGuid );
	static void StaticShutdownAfterError();
	static UObject* GetIndexedObject( INT Index );
	static void GlobalSetProperty( const TCHAR* Value, UClass* Class, UProperty* Property, INT Offset, UBOOL Immediate );
	static void ExportProperties( FOutputDevice& Out, UClass* ObjectClass, BYTE* Object, INT Indent, UClass* DiffClass, BYTE* Diff );
	static void ResetConfig( UClass* Class );
	static void GetRegistryObjects( TArray<FRegistryObjectInfo>& Results, UClass* Class, UClass* MetaClass, UBOOL ForceRefresh );
	static void GetPreferences( TArray<FPreferencesInfo>& Results, const TCHAR* Category, UBOOL ForceRefresh );
	static UBOOL GetInitialized();
	static UPackage* GetTransientPackage();
	static void VerifyLinker( ULinkerLoad* Linker );
	static void ProcessRegistrants();
	static void BindPackage( UPackage* Pkg );
	static const TCHAR* GetLanguage();
	static void SetLanguage( const TCHAR* LanguageExt );
  */

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/