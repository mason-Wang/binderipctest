#define LOG_TAG "IpcTestClient"
#include <utils/Log.h>
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include "IIpcTestService.h"
#include <unistd.h>
#include <fcntl.h>

using namespace android;

class ServerDeathRecipient : public IBinder::DeathRecipient
{
	virtual void binderDied(const wp<IBinder>& who __unused)
	{
		ALOGD("The android.ipctest server is dead!");
	}
};

int main(int argc __unused, char** argv __unused)
{
	sp<ProcessState> proc(ProcessState::self());
	sp<IServiceManager> sm = defaultServiceManager();
	sp<IBinder> binder = sm->getService(String16("android.ipctest"));
	sp<IIpcTestService> ipcTestService;
	sp<IMemory> mem;
	
	if (binder == 0) {
		ALOGE("get ipctest service error!");
		exit(1);
	}
	
	// Test death notification
	// Client register a death notifier, when the server is dead, client will be notified
	sp<ServerDeathRecipient> dr = new ServerDeathRecipient();
	binder->linkToDeath(dr); 
	ipcTestService = interface_cast<IIpcTestService>(binder);
	
	// Test Ashmem
	// Get the memory and dump the content
	mem = ipcTestService->getMemory();
	if (mem != 0) {
		ALOGD("The ashmem content is:%s", mem->pointer());
	}
	mem.clear();
	
	// Test file descriptor transfer
	// Get the file descriptor from server
	int fd = ipcTestService->getFileDescriptor();
	if (fd < 0) {
		ALOGE("Bad file descriptor!");
		exit(1);
	}
	// Write a string to the file and let server dump the file content 
	const char *str = "Hello World!";
	write(fd, str, strlen(str) + 1);
	ipcTestService->dumpFile();
	close(fd);
	
	// Test set/get a integer to/from server
	ipcTestService->setInt(10);
	int value = ipcTestService->getInt();
	ALOGD("Got int value:%d", value);
	
	// Test set/get a string to/from server
	ipcTestService->setString("Hello World!");
	char* string = ipcTestService->getString();
	ALOGD("Got string:%s", string);
	free(string);
	
	proc->startThreadPool();
	IPCThreadState::self()->joinThreadPool();
}

