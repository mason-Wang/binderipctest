LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	IIpcTestService.cpp \
	IpcTestService.cpp \
	main_server.cpp
	
LOCAL_SHARED_LIBRARIES := \
	libutils \
	libcutils \
	liblog \
	libbinder 
	
LOCAL_MODULE:= IpcTestServer
#LOCAL_32_BIT_ONLY := true

include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	IIpcTestService.cpp \
	main_client.cpp
	
LOCAL_SHARED_LIBRARIES:= \
	libutils \
	libcutils \
	liblog \
	libbinder 
	
LOCAL_MODULE:= IpcTestClient
include $(BUILD_EXECUTABLE)