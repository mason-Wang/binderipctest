#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>

#include "IpcTestService.h"

using namespace android;

int main(int argc __unused, char** argv __unused)
{
	sp<ProcessState> proc(ProcessState::self());
	sp<IServiceManager> sm = defaultServiceManager();
	IpcTestService::publishAndJoinThreadPool();
}