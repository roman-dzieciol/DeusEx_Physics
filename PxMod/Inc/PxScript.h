/*===========================================================================
    C++ class definitions exported from UnrealScript.
    This is generated manually.
	Update from PxModClasses.h everytime it changes.
===========================================================================*/
#if _MSC_VER
#pragma pack (push,4)
#endif

#ifndef PXMOD_API
#define PXMOD_API DLL_IMPORT
#endif

#ifndef NAMES_ONLY
#define AUTOGENERATE_NAME(name) extern PXMOD_API FName PXMOD_##name;
#define AUTOGENERATE_FUNCTION(cls,idx,name)
#endif


AUTOGENERATE_NAME(PxMod)


#ifndef NAMES_ONLY


class PXMOD_API UPxCanvas : public UCanvas
{
public:
    DECLARE_CLASS(UPxCanvas,UCanvas,0)
    NO_DEFAULT_CONSTRUCTOR(UPxCanvas)
	virtual void Update( FSceneNode* Frame );
};

#endif


#ifndef NAMES_ONLY
#undef AUTOGENERATE_NAME
#undef AUTOGENERATE_FUNCTION
#endif NAMES_ONLY

#if _MSC_VER
#pragma pack (pop)
#endif
