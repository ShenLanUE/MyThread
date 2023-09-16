#pragma once
#include "CoreMinimal.h"

/* 定义线程的类型和存放特殊的数据类型，如：特殊的结构体 */

struct MYTHREAD_API FSimpleThreadHandle : public TSharedFromThis<FSimpleThreadHandle>
{
	//该类用于查询线程状态
	
	FSimpleThreadHandle();

	uint64 RandomSpawnUniqueID();

	bool operator==(const FSimpleThreadHandle& SimpleThreadHandle)
	{
		return
		this->GUIDTypeA == SimpleThreadHandle.GUIDTypeA &&
		this->GUIDTypeB == SimpleThreadHandle.GUIDTypeB &&
		this->GUIDTypeC == SimpleThreadHandle.GUIDTypeC &&
		this->GUIDTypeD == SimpleThreadHandle.GUIDTypeD;
	}

protected:
	uint64 GUIDTypeA;
	uint64 GUIDTypeB;
	uint64 GUIDTypeC;
	uint64 GUIDTypeD;
};

enum class EThreadState
{
	//闲置的
	LEISURELY,
	//工作的
	WORKING,
	//出错
	ERROR
};

typedef TWeakPtr<FSimpleThreadHandle> FThreadHandle;
typedef TFunction<void()> FThreadLambda;
