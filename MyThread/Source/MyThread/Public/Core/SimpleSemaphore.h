#pragma once
#include "CoreMinimal.h"

struct FSimpleSemaphore
{
	FSimpleSemaphore();
	~FSimpleSemaphore();

	void Wait();
	void Wait(uint32 WaitTime,const bool bIgnoreThreadIdleStats = false);
	void Trigger();

	FORCEINLINE bool IsWait(){ return bWait;}
private:
	//事件
	FEvent* Event;
	//标志线程是否是挂起状态
	uint8 bWait : 1;
};
