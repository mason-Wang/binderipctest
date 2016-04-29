#ifndef ANDROID_IIPCTEST_H
#define ANDROID_IIPCTEST_H

#include <stdint.h>
#include <sys/types.h>

#include <utils/RefBase.h>
#include <utils/Errors.h>
#include <binder/IInterface.h>
#include <binder/IMemory.h>
#include "IIpcTestService.h"

namespace android {

class IIpcTestService : public IInterface
{
public:
	DECLARE_META_INTERFACE(IpcTestService);

	virtual sp<IMemory> getMemory() const = 0; 
	virtual int getFileDescriptor() = 0;
	virtual void dumpFile() = 0;
	virtual void setInt(int value) = 0;
	virtual int getInt() = 0;
	virtual void setString(const char* str) = 0;
	virtual char* getString() = 0;
};

class BnIpcTestService : public BnInterface<IIpcTestService> 
{
public:
	virtual status_t onTransact(uint32_t code,
							const Parcel& data,
							Parcel* reply,
							uint32_t flags = 0);
};

};

#endif
