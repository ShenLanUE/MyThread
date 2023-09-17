#pragma once

#include "CoreMinimal.h"
#include "Interface/ProxyInterface.h"
#include "Containers/Queue.h"
#include "Core/ThreadCoreMacro.h"
#include "Abandonable/SimpleAbandonable.h"
#include "Coroutines/SimpleCoroutines.h"
#include "Async/TaskGraphInterfaces.h"
#include "Runnable/ThreadRunnableProxy.h"
#include "Engine/StreamableManager.h"


class IThreadContainer
{
public:
	IThreadContainer(){}
	virtual ~IThreadContainer(){}

protected:
	FCriticalSection Mutex;
	
};

//可以自由创建线程,不受限制,创建完毕的线程不会马上销毁,如果有需要可以再次使用
//具有同步和异步功能,一般运用在小场景
class IThreadProxyContainer : public TArray<TSharedPtr<IThreadProxy>>,public IThreadContainer
{
protected:
	typedef TArray<TSharedPtr<IThreadProxy>> ITProxyArray;
public:
	//创建线程并更新线程池，将线程添加到线程池
	IThreadProxyContainer& operator<<(const TSharedPtr<IThreadProxy>& IThreadProxy)
	{
		MUTEX_LOCK;
		
		IThreadProxy->CreateSafeThread();
		this->Add(IThreadProxy);

		return *this;
	}

	//遍历线程池,寻找闲置的线程，如果有就将任务代理绑定到闲置的线程上且立即唤醒线程,如果没有就创建一个新的线程
	FThreadHandle operator>>(const FSimpleDelegate& ThreadProxy)
	{
		FThreadHandle ThreadHandle = nullptr;
		{
			MUTEX_LOCK;
			
			for (const auto& Tmp : *this)
			{
				if (Tmp->IsSuspend() && !Tmp->GetThreadDelegate().IsBound())
				{
					Tmp->GetThreadDelegate() = ThreadProxy;
					Tmp->WakeupThread();
					ThreadHandle = Tmp->GetThreadHandle();

					break;
				}
			}
		}
		
		if (!ThreadHandle.IsValid())
		{
			TSharedPtr<IThreadProxy> Proxy = MakeShareable(new FThreadRunnable(false));
			Proxy->GetThreadDelegate() = ThreadProxy;
			*this << Proxy;
			ThreadHandle = Proxy->GetThreadHandle();
		}
		return ThreadHandle;
	}

	//遍历线程池,寻找闲置的线程,如果有就将任务代理绑定到闲置的线程上,如果没有就创建一个新的线程,此线程在创建后默认为挂起
	FThreadHandle operator<<(const FSimpleDelegate& ThreadProxy)
	{
		FThreadHandle ThreadHandle = nullptr;
		{
			MUTEX_LOCK;
			
			for (const auto& Tmp : *this)
			{
				if (Tmp->IsSuspend() && !Tmp->GetThreadDelegate().IsBound())
				{
					Tmp->GetThreadDelegate() = ThreadProxy;
					ThreadHandle = Tmp->GetThreadHandle();

					break;
				}
			}
		}
		
		if (!ThreadHandle.IsValid())
		{
			ThreadHandle = *this << MakeShareable(new FThreadRunnable(true)) << ThreadProxy;
		}
		return ThreadHandle;
	}

	//根据传入的Handle查询线程池中是否有对应的线程，如果有则返回对应的线程代理接口，否则返回nullptr
	TSharedPtr<IThreadProxy> operator>>(const FThreadHandle& ThreadHandle)
	{
		MUTEX_LOCK;
		for (const auto& Tmp : *this)
		{
			if (Tmp->GetThreadHandle() == ThreadHandle)
			{
				return Tmp;
			}
		}
		
		return nullptr;
	}
};

//线程的任务管理，可以自动管理任务，自动从线程池取出闲置的线程并分配，实现高效的利用线程池的特点
class IThreadTaskContainer : public TQueue<FSimpleDelegate>,public TArray<TSharedPtr<IThreadProxy>>,public IThreadContainer
{
protected:
	typedef TArray<TSharedPtr<IThreadProxy>> ITProxyArray;
	typedef TQueue<FSimpleDelegate> TEventQueue;
	
public:
	//将任务代理添加到任务代理队列
	void operator<<(const FSimpleDelegate& ThreadProxy)
	{
		MUTEX_LOCK;
		this->Enqueue(ThreadProxy);
	}

	//从代理队列末尾取出一个代理
	bool operator<<=(FSimpleDelegate& ThreadProxy)
	{
		MUTEX_LOCK;
		return this->Dequeue(ThreadProxy);
	}

	//将线程添加到线程池并返回自身，支持链式编程！
	IThreadTaskContainer& operator<<(const TSharedPtr<IThreadProxy>& IThreadProxy)
	{
		MUTEX_LOCK;
		this->Add(IThreadProxy);
		IThreadProxy->CreateSafeThread();
		return *this;
	}

	//绑定任务代理，如果线程池有闲置线程则将任务绑定到闲置线程上，如果没有则把任务添加到任务代理队列
	void operator>>(const FSimpleDelegate& ThreadProxy)
	{
		bool bSuccessful = false;
		{
			MUTEX_LOCK;
			
			for (const auto& Tmp : *this)
			{
				if (Tmp->IsSuspend() && !Tmp->GetThreadDelegate().IsBound())
				{
					Tmp->GetThreadDelegate() = ThreadProxy;
					Tmp->WakeupThread();

					bSuccessful = true;
					break;
				}
			}
		}

		if (!bSuccessful)
		{
			*this << ThreadProxy;
		}
	}
};

class IAbandonableContainer : public IThreadContainer
{
protected:
	//实现同步操作
	void operator<<(const FSimpleDelegate& ThreadDelegate)
	{
		FAsyncTask<FSimpleAbandonable>* SimpleAbandonable = new FAsyncTask<FSimpleAbandonable>(ThreadDelegate);
		SimpleAbandonable->StartBackgroundTask(); 
		/*如果是主线程启动，则阻塞主线程 */ 
		SimpleAbandonable->EnsureCompletion();
		delete SimpleAbandonable;
	}

	//异步操作
	void operator>>(const FSimpleDelegate& ThreadDelegate)
	{
		(new FAutoDeleteAsyncTask<FSimpleAbandonable>(ThreadDelegate))->StartBackgroundTask();
		
	}
};

//协程接口容器
class ICoroutinesContainer
{
public:
	ICoroutinesContainer() : TmpTotalTime(0.f){}
	
	virtual ~ICoroutinesContainer(){ICoroutinesObject::Array.Empty();}

	ICoroutinesContainer& operator<<(float TotalTime)
	{
		TmpTotalTime = TotalTime;
		return *this;
	}

	FCoroutinesHandle operator<<(const FSimpleDelegate& ThreadDelegate) const
	{
		ICoroutinesObject::Array.Add(MakeShareable(new FCoroutinesObject(TmpTotalTime,ThreadDelegate)));
		return ICoroutinesObject::Array[ICoroutinesObject::Array.Num() -1];
	}

	FCoroutinesHandle operator>>(const FSimpleDelegate& ThreadDelegate) const
	{
		ICoroutinesObject::Array.Add(MakeShareable(new FCoroutinesObject(ThreadDelegate)));
		return ICoroutinesObject::Array[ICoroutinesObject::Array.Num() -1];
	}

	void operator<<=(const float Time) const
	{
		TArray<TSharedPtr<ICoroutinesObject>> RemoveObject;
		
		for (int32 i = 0; i < ICoroutinesObject::Array.Num();i++)
		{
			FCoroutinesRequest Request(Time);
			ICoroutinesObject::Array[i]->Update(Request);
			if (Request.bCompleteRequest)
			{
				RemoveObject.Add(ICoroutinesObject::Array[i]);
			}
		}

		for (const auto& RemoveInstance : RemoveObject)
		{
			ICoroutinesObject::Array.Remove(RemoveInstance);
		}
	}
private:
	float TmpTotalTime;
};

class IGraphContaniner : public IThreadContainer
{
protected:
	//呼叫主线程
	FGraphEventRef operator << (const FSimpleDelegate& ThreadDelegate)
	{
		MUTEX_LOCK;
		return FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
			ThreadDelegate,
			TStatId(),
			nullptr,
			ENamedThreads::GameThread);
	}

	//呼叫其他线程
	FGraphEventRef operator >> (const FSimpleDelegate& ThreadDelegate)
	{
		MUTEX_LOCK;
		return FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
			ThreadDelegate,
			TStatId(),
			nullptr,
			ENamedThreads::AnyThread);
	}
};

class IStreamableContainer
{
public:
	virtual ~IStreamableContainer(){}

	//存储路径
	IStreamableContainer& operator >> (const TArray<FSoftObjectPath>& InObjectPathArray)
	{
		SetObjectPath(InObjectPathArray);
		return *this;
	}

	//异步完成后执行传入的代理
	//异步加载资源
	TSharedPtr<struct FStreamableHandle> operator >> (const FSimpleDelegate& ThreadDelegate)
	{
		return GetStreamableManager()->RequestAsyncLoad(GetObjectPath(),ThreadDelegate);
	}

	//同步加载资源
	TSharedPtr<struct FStreamableHandle> operator << (const TArray<FSoftObjectPath>& InObjectPathArray)
	{
		return GetStreamableManager()->RequestSyncLoad(InObjectPathArray);
	}

protected:
	virtual void SetObjectPath(const TArray<FSoftObjectPath>& InObjectPathArray) = 0;
	virtual TArray<FSoftObjectPath>& GetObjectPath() = 0;
	virtual FStreamableManager* GetStreamableManager() = 0;
};
