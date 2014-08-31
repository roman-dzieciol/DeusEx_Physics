/*=============================================================================
	PxMemBuffer.cpp
	Copyright 2006 Roman Dzieciol. All Rights Reserved.
=============================================================================*/
#pragma pack(push, 8)

#include "NxPhysics.h"
#include "PxMemBuffer.h"
#include <stdio.h>


/*-----------------------------------------------------------------------------
	FPxMemWriteBuffer
-----------------------------------------------------------------------------*/

FPxMemWriteBuffer::FPxMemWriteBuffer() : currentSize(0), maxSize(0), data(NULL)
	{
	}

FPxMemWriteBuffer::~FPxMemWriteBuffer()
	{
		if(data!=NULL)
		{
			delete[] data;
			data=NULL;
		}
	}

void FPxMemWriteBuffer::clear()
	{
	currentSize = 0;
	}

NxStream& FPxMemWriteBuffer::storeByte(NxU8 b)
	{
	storeBuffer(&b, sizeof(NxU8));
	return *this;
	}
NxStream& FPxMemWriteBuffer::storeWord(NxU16 w)
	{
	storeBuffer(&w, sizeof(NxU16));
	return *this;
	}
NxStream& FPxMemWriteBuffer::storeDword(NxU32 d)
	{
	storeBuffer(&d, sizeof(NxU32));
	return *this;
	}
NxStream& FPxMemWriteBuffer::storeFloat(NxReal f)
	{
	storeBuffer(&f, sizeof(NxReal));
	return *this;
	}
NxStream& FPxMemWriteBuffer::storeDouble(NxF64 f)
	{
	storeBuffer(&f, sizeof(NxF64));
	return *this;
	}
NxStream& FPxMemWriteBuffer::storeBuffer(const void* buffer, NxU32 size)
	{
	NxU32 expectedSize = currentSize + size;
	if(expectedSize > maxSize)
		{
		maxSize = expectedSize + 4096;

		NxU8* newData = new NxU8[maxSize];
		NX_ASSERT(newData!=NULL);

		if(data)
			{
			memcpy(newData, data, currentSize);
			delete[] data;
			}
		data = newData;
		}
	memcpy(data+currentSize, buffer, size);
	currentSize += size;
	return *this;
	}


/*-----------------------------------------------------------------------------
	FPxMemReadBuffer
-----------------------------------------------------------------------------*/
FPxMemReadBuffer::FPxMemReadBuffer(const NxU8* data) : buffer(data)
	{
	}

FPxMemReadBuffer::~FPxMemReadBuffer()
	{
	// We don't own the data => no delete
	}

NxU8 FPxMemReadBuffer::readByte() const
	{
	NxU8 b;
	memcpy(&b, buffer, sizeof(NxU8));
	buffer += sizeof(NxU8);
	return b;
	}

NxU16 FPxMemReadBuffer::readWord() const
	{
	NxU16 w;
	memcpy(&w, buffer, sizeof(NxU16));
	buffer += sizeof(NxU16);
	return w;
	}

NxU32 FPxMemReadBuffer::readDword() const
	{
	NxU32 d;
	memcpy(&d, buffer, sizeof(NxU32));
	buffer += sizeof(NxU32);
	return d;
	}

float FPxMemReadBuffer::readFloat() const
	{
	float f;
	memcpy(&f, buffer, sizeof(float));
	buffer += sizeof(float);
	return f;
	}

double FPxMemReadBuffer::readDouble() const
	{
	double f;
	memcpy(&f, buffer, sizeof(double));
	buffer += sizeof(double);
	return f;
	}

void FPxMemReadBuffer::readBuffer(void* dest, NxU32 size) const
	{
	memcpy(dest, buffer, size);
	buffer += size;
	}


#pragma pack(pop)
/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/