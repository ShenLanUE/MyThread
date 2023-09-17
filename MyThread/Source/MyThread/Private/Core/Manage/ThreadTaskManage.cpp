#include "Core/Manage/ThreadTaskManage.h"

FThreadTaskManagement::FThreadTaskManagement()
{
	Init(CPUThreadNumber);
}

FThreadTaskManagement::~FThreadTaskManagement()
{
	for (const auto& Tmp : *this)
	{
		Tmp->WaitAndCompleted();
	}
	
}

void FThreadTaskManagement::Init(int32 ThreadNum)
{
	for (int32 i = 0; i < ThreadNum;i++)
	{
		*this << MakeShareable(new FThreadRunnable);
	}

	FPlatformProcess::Sleep(0.1f);
}

void FThreadTaskManagement::Tick(float DeltaTime)
{
	TSharedPtr<IThreadProxy> IThreadProxy = nullptr;
	{
		MUTEX_LOCK;
		for (const auto& Tmp :*this)
		{
			if (Tmp->IsSuspend())
			{
				IThreadProxy = Tmp;
				break;
			}
		}
	}

	if (IThreadProxy.IsValid())
	{
		if (!((TEventQueue *)this)->IsEmpty())
		{
			FSimpleDelegate SimpleDelegate;
			if (*this <<= SimpleDelegate)
			{
				MUTEX_LOCK;
				IThreadProxy->GetThreadDelegate() = SimpleDelegate;
				IThreadProxy->WakeupThread();
				
			}
		}
	}
}

TStatId FThreadTaskManagement::GetStatId() const
{
	return TStatId();
}
