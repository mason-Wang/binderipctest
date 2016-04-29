#define LOG_TAG "IpcTestService"

#include "IpcTestService.h"
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

namespace android  {
	IpcTestService::IpcTestService() 
		: mMem(NULL),
		  mFd(-1),
		  mIntVal(0),
		  mStr(NULL)
		  
	{
		ALOGD("IpcTestService() is called.");
	}
	
	void IpcTestService::onFirstRef()
	{
		ALOGD("onFirstRef() is called.");
		mMem = new MemoryBase(new MemoryHeapBase(512, 0, "IpcTestMem"), 0, 512);
		strcpy((char *)mMem->pointer(), "Hello Ashmem");
	}
	
	sp<IMemory> IpcTestService::getMemory() const
	{
		ALOGD("getMemory() is called.");
		return mMem;
	}
	
	status_t IpcTestService::dump(int fd, const Vector<String16>& args  __unused)
	{
		const size_t SIZE = 256;
		char buffer[SIZE];
		String8 result;
		
		ALOGD("dump() is called.");
		snprintf(buffer, SIZE, "mFd = %d\n"
							"mIntVal = %d\n"
							"mStr = %s\n",
							mFd, mIntVal, mStr);
		result.append(buffer);
		write(fd, result.string(), result.size());
		return NO_ERROR;
	}
	
	int IpcTestService::getFileDescriptor()
	{
		ALOGD("getFileDescriptor is called.");
		mFd = open("/tmp/ipctest", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
		return mFd;
	}
	
	void IpcTestService::dumpFile() 
	{
		ALOGD("dumpFile is called.");
		int size, offset;
		char* data;
		if (mFd < 0) return;
		offset = lseek(mFd, 0, SEEK_CUR);
		if (offset < 0) return;
		ALOGD("dumpFile: current offset is %d.", offset);
		size = lseek(mFd, 0, SEEK_END);
		if (size < 0) return;
		if (lseek(mFd, 0, SEEK_SET) < 0) return;
		ALOGD("dumpFile: file size is %d.", size);
		data = (char*)malloc(size);
		if (data == 0) return;
		if (read(mFd, data, size) < 0) {
			free(data);
			return;
		}
		ALOGD("dumpFile:%s", data);
		free(data);
	}
	
	void IpcTestService::setInt(int value)
	{
		ALOGD("setInt:%d", value);
		mIntVal = value;
	}
	
	int IpcTestService::getInt()
	{
		ALOGD("getInt is called");
		return mIntVal;
	}
	
	void IpcTestService::setString(const char *str)
	{
		ALOGD("setString:%s", str);
		int len = strlen(str);
		mStr = (char *)malloc(len+1);
		if (mStr != NULL) strcpy(mStr, str);
	}
	
	char* IpcTestService::getString()
	{
		ALOGD("getString is called.");
		return mStr;
	}
};