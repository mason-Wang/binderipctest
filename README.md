# binderipctest
This a android binder native service sample

这个测试程序主要包含如下功能：
- 客户端通过binder获取一块共享的匿名内存，把内容给打印出来 
- 客户端通过binder获取一个服务端已经打开的文件的文件描述符，写入内容，并在服务端打印出文件的内容 
- 客户端通过binder设置和获取服务端的一个整数成员 
- 客户端通过binder设置和获取服务端一个字符串成员 
- 验证binder的death notification功能 
- 通过dumpsys工具获取服务端的信息 

# 验证方法
把程序放到Android的代码目录下，在程序目录下执行`mm`编译，最后会产生`/system/bin/IpcTestServer`和
`/system/bin/IpcTestClient`两个可执行文件，把这两个文件复制到Android系统中，例如/tmp目录下
先让服务端运行并打印log： 
```
shell@android:/tmp # logcat | grep IpcTest 
shell@android:/tmp # ./IpcTestServer & 
```
可以看到有如下log
```
D/IpcTestService(18751): IpcTestService() is called.
D/IpcTestService(18751): onFirstRef() is called.
```
然后运行客户端： 
`shell@android:/tmp # ./IpcTestClient &`   
log如下：   
```
D/IpcTestService(18751): getMemory() is called.
D/IpcTestClient(18770): The ashmem content is:Hello Ashmem
D/IpcTestService(18751): getFileDescriptor is called.
D/IpcTestService(18751): dumpFile is called.
D/IpcTestService(18751): dumpFile: current
D/IpcTestService(18751): dump() is called.offset is 13.
D/IpcTestService(18751): dumpFile: file size is 13.
D/IpcTestService(18751): dumpFile:Hello World!
D/IpcTestService(18751): setInt:10
D/IpcTestService(18751): getInt is called
D/IpcTestClient(18770): Got int value:10
D/IpcTestService(18751): setString:Hello World!
D/IpcTestService(18751): getString is called.
D/IpcTestClient(18770): Got string:Hello World!
```
接着试一下dumpsys工具的使用   
`shell@android:/tmp # dumpsys android.ipctest `   
log输出如下：   
```
mFd = 12
mIntVal = 10
mStr = Hello World!
```
最后验证binder的death notification功能   
`shell@android:/tmp # kill -9 18751 `   

可以看到log输出如下：    
`D/IpcTestClient(18770): The android.ipctest server is dead!`

