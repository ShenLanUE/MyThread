#pragma once
#include "Misc/ScopeLock.h"
#include"Abandonable/SimpleAbandonable.h"

/* 存放通用的宏 */

#if PLATFORM_WINDOWS
	#include <iostream>
	#include <thread>
	//获取当前机器CPU支持的最大线程数
	#define CPUThreadNumber std::thread::hardware_concurrency()
#else 
	#define CPUThreadNumber 12
#endif

//作用域锁
#define MUTEX_LOCK FScopeLock ScopeLock(&Mutex)

//通过宏编程的方式实现异步
#define USE_UE_THREAD_POOL_ASYNCTASK(ThreadDelegate) \
	(new FAutoDeleteAsyncTask<FSimpleAbandonable>(ThreadDelegate))->StartBackgroundTask();

#define ASYNCTASK_UOBJECT(Object,...) \
USE_UE_THREAD_POOL_ASYNCTASK(FSimpleDelegate::CreateUObject(Object,__VA_ARGS__))

#define ASYNCTASK_Raw(Object,...) \
USE_UE_THREAD_POOL_ASYNCTASK(FSimpleDelegate::CreateRaw(Object,__VA_ARGS__))

#define ASYNCTASK_SP(Object,...) \
USE_UE_THREAD_POOL_ASYNCTASK(FSimpleDelegate::CreateSP(Object,__VA_ARGS__))

#define ASYNCTASK_Lambda(...) \
USE_UE_THREAD_POOL_ASYNCTASK(FSimpleDelegate::CreateLambda(__VA_ARGS__))

#define ASYNCTASK_UFunction(Object,...) \
USE_UE_THREAD_POOL_ASYNCTASK(FSimpleDelegate::CreateUFunction(Object,__VA_ARGS__))

//通过元编程的方式实现同步
#define USE_UE_THREAD_POOL_SYNCTASK(ThreadDelegate) \
	{ \
		FAsyncTask<FSimpleAbandonable>* SimpleAbandonable = new FAsyncTask<FSimpleAbandonable>(ThreadDelegate); \
		SimpleAbandonable->StartBackgroundTask(); \
		/*如果是主线程启动，则阻塞主线程 */  \
		SimpleAbandonable->EnsureCompletion(); \
		delete SimpleAbandonable; \
	}

#define SYNCTASK_UOBJECT(Object,...) \
USE_UE_THREAD_POOL_SYNCTASK(FSimpleDelegate::CreateUObject(Object,__VA_ARGS__))

#define SYNCTASK_Raw(Object,...) \
USE_UE_THREAD_POOL_SYNCTASK(FSimpleDelegate::CreateRaw(Object,__VA_ARGS__))

#define SYNCTASK_SP(Object,...) \
USE_UE_THREAD_POOL_SYNCTASK(FSimpleDelegate::CreateSP(Object,__VA_ARGS__))

#define SYNCTASK_Lambda(...) \
USE_UE_THREAD_POOL_SYNCTASK(FSimpleDelegate::CreateLambda(__VA_ARGS__))

#define SYNCTASK_UFunction(Object,...) \
USE_UE_THREAD_POOL_SYNCTASK(FSimpleDelegate::CreateUFunction(Object,__VA_ARGS__))


#define CALL_THREAD_SYNCH(CallThread,OtherTask,Code) \
{\
	FGraphEventRef NewTaskEventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()->void {Code},TStatId(),OtherTask,CallThread);\
	FTaskGraphInterface::Get().WaitUntilTaskCompletes(NewTaskEventRef);\
}

#define CALL_THREAD_ASYNCH(CallThread,OtherTask,Code) \
FFunctionGraphTask::CreateAndDispatchWhenReady([&]()->void {Code},TStatId(),OtherTask,CallThread);\

//呼叫线程 可以设置前置任务
#define CALL_THREAD(EventRef,InTaskDeletegate,OtherTask) \
EventRef = FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(InTaskDeletegate,TStatId(),OtherTask);\

#define CALL_THREAD_UOBJECT(EventRef,OtherTask,Object,Method,...) \
CALL_THREAD(EventRef,OtherTask,FSimpleDelegate::CreateUObject(Object,Method,__VA_ARGS__))

#define CALL_THREAD_Raw(EventRef,OtherTask,Object,Method,...) \
CALL_THREAD(EventRef,OtherTask,FSimpleDelegate::CreateRaw(Object,Method,__VA_ARGS__))

#define CALL_THREAD_SP(EventRef,OtherTask,Object,Method,...) \
CALL_THREAD(EventRef,OtherTask,FSimpleDelegate::CreateSP(Object,Method,__VA_ARGS__))

#define CALL_THREAD_Lambda(EventRef,OtherTask,Method,...) \
CALL_THREAD(EventRef,OtherTask,FSimpleDelegate::CreateLambda(Method,__VA_ARGS__))

#define CALL_THREAD_UFunction(EventRef,OtherTask,Object,Method,...) \
CALL_THREAD(EventRef,OtherTask,FSimpleDelegate::CreateUFunction(Object,Method,__VA_ARGS__))

#define WAITING_OTHER_THREADS_COMPLETED(EventRef) \
FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);

#define WAITING_OTHER_THREADS_COMPLETED_MULTI(EventRef) \
FTaskGraphInterface::Get().WaitUntilTasksComplete(EventRef);

#pragma region SimulateUERenderThread
#if 0
class FRenderThread
{
public:
	static ENamedThreads::Type GetDesiredThread()
	{
		check(!GIsThreadedRendering || ENamedThreads::GetRenderThread() != ENamedThreads::GameThread)
		return ENamedThreads::GetRenderThread();
	}

	static ESubsequentsMode::Type GetSubsequentsMode()
	{
		/*可用于在启动不依赖于其他任务的任务,节省任务图开销*/
		return ESubsequentsMode::FireAndForget;
	}
};

#define DOTASK_THREAD(Code) \
void DoTask(ENamedThreads::Type CurrentThread.const FGraphEventRef& MyCompletionGraphEvent) \
{\
	FRHICommandListImmediate& RHICmdList = GetImmediateCommandList_ForRenderCommand(); \
	Code; \
}

#define DOTASK_THREAD_ID(TypeName) \
FORCEINLINE TStatId GetStatId() const \
{ \
	RETURN_QUICK_DECLARE_CYCLE_STAT(TypeName, STATGROUP_RenderThreadCommands); \
}

#define CALL_THREAD(Code) \
{
class FRenderExample :public FRenderThread\
{\
public:\
	DOTASK_THREAD(Code)\
	DOTASK_THREAD_ID(FRenderExample)\
};\

{\
	if (ShouldExecuteOnRenderThread())\
	{\
		CheckNotBlockedOnRenderThread()\
		TGraphTask<FRenderExample>::CreateTask().ConstructAndDispatchWhenReady();\
	}\
	else\
	{\
		FRenderExample RenderExample;\
		//范围循环计数器
		FScopeCycleCounter ScopeCycleCounter(RenderExample.GetStatId());\
		RenderExample.DoTask(ENamedThreads::GameThread, FGraphEventRef());\
	}\
}\
}
#endif 0
#pragma endregion

