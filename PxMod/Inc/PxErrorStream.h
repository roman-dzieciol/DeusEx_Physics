/*=============================================================================
	PxErrorStream.h
	Copyright 2006 Roman Dzieciol. All Rights Reserved.
=============================================================================*/
#ifndef PXERRORSTREAM_H
#define PXERRORSTREAM_H
#pragma pack(push, 8)


/*-----------------------------------------------------------------------------
	PxErrorStream.
-----------------------------------------------------------------------------*/
class PxErrorStream : public NxUserOutputStream
{
public:
	void reportError (NxErrorCode code, const char *message, const char* file, int line)
	{
		//this should be routed to the application
		//specific error handling. If this gets hit
		//then you are in most cases using the SDK
		//wrong and you need to debug your code!
		//however, code may  just be a warning or
		//information.

		switch(code)
		{
		case NXE_NO_ERROR:
		case NXE_INVALID_PARAMETER:
		case NXE_INVALID_OPERATION:
		case NXE_OUT_OF_MEMORY:
		case NXE_INTERNAL_ERROR:
		case NXE_ASSERTION:
//			PX_LOG( PX_NAME, TEXT("%s !! %s :: CODE:%d MSG:%s FILE:%s LINE:%d"), PX_LOGP, code, appFromAnsi(message), appFromAnsi(file), line  );
			//MessageBox(NULL, message, "SDK Error", MB_OK),
			//exit(1);
			break;

		case NXE_DB_INFO:
		case NXE_DB_WARNING:
		case NXE_DB_PRINT:
//			PX_LOG( PX_NAME, TEXT("%s !! %s :: CODE:%d MSG:%s FILE:%s LINE:%d"), PX_LOGP, code, appFromAnsi(message), appFromAnsi(file), line  );
			break;
		}
	}
	        
	NxAssertResponse reportAssertViolation (const char *message, const char *file,int line)
	{
		//this should not get hit by
		// a properly debugged SDK!
		//assert(0);
//		PX_LOG( PX_NAME, TEXT("%s !! %s :: ASSERT VIOLATION! MSG:%s FILE:%s LINE:%d"), PX_LOGP, appFromAnsi(message), appFromAnsi(file), line  );
		return NX_AR_CONTINUE;
	}
	        
	void print (const char *message)
	{
//		PX_LOG( PX_NAME, TEXT("%s !! %s :: %s"), PX_LOGP, appFromAnsi(message)  );
	}
};



#pragma pack(pop)
#endif
/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/
