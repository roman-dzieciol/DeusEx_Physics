/*=============================================================================
	PxMemBuffer.h
	Copyright 2006 Roman Dzieciol. All Rights Reserved.
=============================================================================*/
#ifndef PXMEMBUFFER_H
#define PXMEMBUFFER_H
#pragma pack(push, 8)

#include "NxStream.h"


/*-----------------------------------------------------------------------------
	FPxMemWriteBuffer
-----------------------------------------------------------------------------*/
class FPxMemWriteBuffer : public NxStream
{
public:
								FPxMemWriteBuffer();
	virtual						~FPxMemWriteBuffer();
				void			clear();

	virtual		NxU8			readByte()								const	{ NX_ASSERT(0);	return 0;	}
	virtual		NxU16			readWord()								const	{ NX_ASSERT(0);	return 0;	}
	virtual		NxU32			readDword()								const	{ NX_ASSERT(0);	return 0;	}
	virtual		float			readFloat()								const	{ NX_ASSERT(0);	return 0.0f;}
	virtual		double			readDouble()							const	{ NX_ASSERT(0);	return 0.0;	}
	virtual		void			readBuffer(void* buffer, NxU32 size)	const	{ NX_ASSERT(0);				}

	virtual		NxStream&		storeByte(NxU8 b);
	virtual		NxStream&		storeWord(NxU16 w);
	virtual		NxStream&		storeDword(NxU32 d);
	virtual		NxStream&		storeFloat(NxReal f);
	virtual		NxStream&		storeDouble(NxF64 f);
	virtual		NxStream&		storeBuffer(const void* buffer, NxU32 size);

				NxU32			currentSize;
				NxU32			maxSize;
				NxU8*			data;
};

/*-----------------------------------------------------------------------------
	FPxMemReadBuffer
-----------------------------------------------------------------------------*/
class FPxMemReadBuffer : public NxStream
{
public:
								FPxMemReadBuffer(const NxU8* data);
	virtual						~FPxMemReadBuffer();

	virtual		NxU8			readByte()								const;
	virtual		NxU16			readWord()								const;
	virtual		NxU32			readDword()								const;
	virtual		float			readFloat()								const;
	virtual		double			readDouble()							const;
	virtual		void			readBuffer(void* buffer, NxU32 size)	const;

	virtual		NxStream&		storeByte(NxU8 b)							{ NX_ASSERT(0);	return *this;	}
	virtual		NxStream&		storeWord(NxU16 w)							{ NX_ASSERT(0);	return *this;	}
	virtual		NxStream&		storeDword(NxU32 d)							{ NX_ASSERT(0);	return *this;	}
	virtual		NxStream&		storeFloat(NxReal f)						{ NX_ASSERT(0);	return *this;	}
	virtual		NxStream&		storeDouble(NxF64 f)						{ NX_ASSERT(0);	return *this;	}
	virtual		NxStream&		storeBuffer(const void* buffer, NxU32 size)	{ NX_ASSERT(0);	return *this;	}

	mutable		const NxU8*		buffer;
};

#pragma pack(pop)
#endif
/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/