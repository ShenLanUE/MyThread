#pragma once

#include "CoreMinimal.h"
#include "Core/Manage/ManageBase/ThreadManageBase.h"
#include "Interface/ThreadManageInterface.h"

struct MYTHREAD_API FThreadGraphManage : public FThreadTemplate<IGraphContaniner,FGraphEventRef>
{
	static void Wait(const FGraphEventRef& EventRef);
	static void Wait(const FGraphEventArray& EventRefArray);
};
