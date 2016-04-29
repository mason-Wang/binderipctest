#include <binder/Parcel.h>
#include "IIpcTestService.h"

namespace android {

enum {
    GET_MEMORY = IBinder::FIRST_CALL_TRANSACTION,
	GET_FD,
	DUMP_FILE,
	SET_INT,
	GET_INT,
	SET_STR,
	GET_STR,
};

class BpIpcTestService : public BpInterface<IIpcTestService>
{
public:
	BpIpcTestService(const sp<IBinder>& impl)
		: BpInterface<IIpcTestService>(impl)
	{
	}
	
	virtual sp<IMemory> getMemory() const
	{
		Parcel data, reply;
		sp<IMemory> mem;
		data.writeInterfaceToken(IIpcTestService::getInterfaceDescriptor());
		status_t status = remote()->transact(GET_MEMORY, data, &reply);
		if (status == NO_ERROR) {
			mem = interface_cast<IMemory>(reply.readStrongBinder());
			if (mem != 0 && mem->pointer() == NULL) {
				mem.clear();
			}
		}
		return mem;
	}
	
	virtual int getFileDescriptor()
	{
		Parcel data, reply;
		int fd;
		data.writeInterfaceToken(IIpcTestService::getInterfaceDescriptor());
		status_t status = remote()->transact(GET_FD, data, &reply);
		if (status == NO_ERROR) {
			//Note:we must dup the got file descriptor, because the file descriptor 
			//will be closed after this function return, the close is did by the ~Parcel()
			fd = dup(reply.readFileDescriptor());
		}
		return fd;
	}
	
	virtual void dumpFile() 
	{
		Parcel data, reply;
		data.writeInterfaceToken(IIpcTestService::getInterfaceDescriptor());
		remote()->transact(DUMP_FILE, data, &reply);
	}
	
	virtual void setInt(int value)
	{
		Parcel data, reply;
		data.writeInterfaceToken(IIpcTestService::getInterfaceDescriptor());
		data.writeInt32(value);
		remote()->transact(SET_INT, data, &reply);
	}
	
	virtual int getInt()
	{
		Parcel data, reply;
		data.writeInterfaceToken(IIpcTestService::getInterfaceDescriptor());
		remote()->transact(GET_INT, data, &reply);
		return reply.readInt32();
	}
	
	virtual void setString(const char* str) 
	{
		Parcel data, reply;
		data.writeInterfaceToken(IIpcTestService::getInterfaceDescriptor());
		data.writeCString(str);
		remote()->transact(SET_STR, data, &reply);
	}
	
	virtual char* getString()
	{
		Parcel data, reply;
		const char* str1;
		char* str2;
		data.writeInterfaceToken(IIpcTestService::getInterfaceDescriptor());
		remote()->transact(GET_STR, data, &reply);
		str1 = reply.readCString();
		str2 = (char *)malloc(strlen(str1) + 1);
		strcpy(str2, str1);
		return str2;
	}
	
};

IMPLEMENT_META_INTERFACE(IpcTestService, "android.test.IIpcTestService");

status_t BnIpcTestService::onTransact(
    uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
{
    switch (code) {
		case GET_MEMORY: {
			CHECK_INTERFACE(IIpcTestService, data, reply);
			reply->writeStrongBinder(getMemory()->asBinder());
			return NO_ERROR;
		}break;
		case GET_FD: {
			CHECK_INTERFACE(IIpcTestService, data, reply);
			reply->writeFileDescriptor(getFileDescriptor());
			return NO_ERROR;
		}break;
		case DUMP_FILE: {
			CHECK_INTERFACE(IIpcTestService, data, reply);
			dumpFile();
			return NO_ERROR;
		}break;
		case SET_INT: {
			CHECK_INTERFACE(IIpcTestService, data, reply);
			setInt(data.readInt32());
			return NO_ERROR;
		}break;
		case GET_INT: {
			CHECK_INTERFACE(IIpcTestService, data, reply);
			reply->writeInt32(getInt());
			return NO_ERROR;
		}break;
		case SET_STR: {
			CHECK_INTERFACE(IIpcTestService, data, reply);
			setString(data.readCString());
			return NO_ERROR;
		}break;
		case GET_STR: {
			CHECK_INTERFACE(IIpcTestService, data, reply);
			reply->writeCString(getString());
			return NO_ERROR;
		}break;
		default:
			return BBinder::onTransact(code, data, reply, flags);
	}
}

};
