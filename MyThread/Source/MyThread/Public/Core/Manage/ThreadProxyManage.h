#pragma once

#include "CoreMinimal.h"
#include "Core/SimpleThreadType.h"
#include "Interface/ThreadManageInterface.h"
#include "ManageBase/ThreadManageBase.h"

//Bind 创建线程并且绑定任务，但不执行 通过Join和Detach来决定是异步执行还是同步执行
//Create 创建线程并且直接执行任务，是最快捷的异步方式
class MYTHREAD_API FThreadProxyManage : public FThreadTemplate<IThreadProxyContainer,FThreadHandle>
{
public:
	virtual ~FThreadProxyManage() override;

	//同步
	bool Join(FThreadHandle Handle);

	//异步
	bool Detach(FThreadHandle Handle);

	//判断当前线程是否可以同步
	EThreadState Joinable(FThreadHandle Handle);
	
};
