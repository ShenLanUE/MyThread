#include "Runnable/ThreadRunnableProxy.h"
#include "HAL/RunnableThread.h"

int32 FThreadRunnable::ThreadCount = 0;

FThreadRunnable::FThreadRunnable(bool IsSuspendAtFirst)
	:IThreadProxy()
	,StopTaskCounter(0)
	,bSuspendAtFirst(IsSuspendAtFirst)
	,Thread(nullptr)
{
	
}

FThreadRunnable::~FThreadRunnable()
{
	if (Thread != nullptr)
	{
		delete Thread;
		Thread = nullptr;
	}
}

void FThreadRunnable::CreateSafeThread()
{
	RunnableName = *(TEXT("SimpleThread-") + FString::Printf(TEXT("%i"),ThreadCount));
	Thread = FRunnableThread::Create(this,*RunnableName.ToString(),0,TPri_BelowNormal);

	ThreadCount++;
}

void FThreadRunnable::WakeupThread()
{
	//触发事件，以便激活所有等待的线程。
	ThreadEvent.Trigger();
}

bool FThreadRunnable::IsSuspend()
{
	return ThreadEvent.IsWait();
}

void FThreadRunnable::WaitAndCompleted()
{
	Stop();
	if (ThreadEvent.IsWait())
	{
		ThreadEvent.Trigger();
	
		//阻塞启动线程
		StartUpThreadEvent.Wait();
		FPlatformProcess::Sleep(0.03f);
	}
}

void FThreadRunnable::BlockingAndCompletion()
{
	ThreadEvent.Trigger();
	
	WaitExecuteEvent.Wait();
}

bool FThreadRunnable::Init()
{
	//平台是否支持多线程
	if (!FPlatformProcess::SupportsMultithreading())
	{
		Stop();
	}
	
	return StopTaskCounter.GetValue() == 0;
}

uint32 FThreadRunnable::Run()
{
	while (StopTaskCounter.GetValue() == 0)
	{
		if (bSuspendAtFirst)
		{
			ThreadEvent.Wait(); // 挂起线程
		}
		
		//业务逻辑
		if (ThreadDelegate.IsBound())
		{
			ThreadDelegate.Execute();
			ThreadDelegate.Unbind();
		}

		//激活挂起的启动线程
		WaitExecuteEvent.Trigger();
		
		bSuspendAtFirst = true;
	}
	
	return 0;
}

void FThreadRunnable::Exit()
{
	//激活主线程
	StartUpThreadEvent.Trigger();
}

void FThreadRunnable::Stop()
{
	StopTaskCounter.Increment();
}
