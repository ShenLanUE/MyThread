#pragma once

#include "CoreMinimal.h"
#include "Interface/ThreadManageInterface.h"
#include "Tickable.h"
#include "ManageBase/ThreadManageBase.h"

//Bind 添加到任务队列中 如果有空置的线程可以直接执行该任务
//Create 直接在线程池里面找 如果有闲置的线程 直接运行当前任务
class MYTHREAD_API FThreadTaskManagement : public FThreadTemplate<IThreadTaskContainer>,public FTickableGameObject
{
public:
	FThreadTaskManagement();
	virtual ~FThreadTaskManagement() override;

	void Init(int32 ThreadNum);
	
	//Thick
	
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
};

