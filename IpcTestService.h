#ifndef ANDROID_IPCTESTSERVICE_H
#define ANDROID_IPCTESTSERVICE_H

#include <binder/BinderService.h>
#include <binder/IMemory.h>
#include <binder/MemoryHeapBase.h>
#include <binder/MemoryBase.h>
#include "IIpcTestService.h"

namespace android {

    class IpcTestService : 
		public BinderService<IpcTestService>,
		public BnIpcTestService
    {
	public:
		static const char* getServiceName() { return "android.ipctest";}
		IpcTestService();
		virtual status_t dump(int fd, const Vector<String16>& args);
		virtual sp<IMemory> getMemory() const;	
		virtual int getFileDescriptor();
		virtual void dumpFile();
		virtual void setInt(int value);
		virtual int getInt();
		virtual void setString(const char* str);
		virtual char* getString();
		
	private:
		virtual void onFirstRef();
		sp<MemoryBase> mMem;
		int mFd;
		int mIntVal;
		char *mStr;
    };
};

#endif
