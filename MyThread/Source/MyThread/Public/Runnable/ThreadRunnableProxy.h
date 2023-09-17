#pragma once

#include "CoreMinimal.h"
#include "Core/SimpleSemaphore.h"
#include "HAL/Runnable.h"
#include "Interface/ProxyInterface.h"
/* 创建线程 */

class FThreadRunnable : public FRunnable,public IThreadProxy
{
public:
	FThreadRunnable(bool IsSuspendAtFirst = true);
	virtual ~FThreadRunnable() override;

	virtual void CreateSafeThread() override;
	virtual void WakeupThread() override;
	virtual bool IsSuspend() override;
	virtual void WaitAndCompleted() override;
	virtual void BlockingAndCompletion() override;

private:
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;
	virtual void Stop() override;

	//线程安全计数
	FThreadSafeCounter StopTaskCounter;
	//是否挂起线程
	bool bSuspendAtFirst;
	//线程实例
	class FRunnableThread* Thread;
	//线程名字
	FName RunnableName;
	//线程挂起事件，让执行它的线程挂起
	FSimpleSemaphore ThreadEvent;
	//启动的事件，挂起主线程
	FSimpleSemaphore StartUpThreadEvent;
	//等待执行事件
	FSimpleSemaphore WaitExecuteEvent;
	//锁
	FCriticalSection Mutex;

	//线程计数
	static int32 ThreadCount;
};
