#include "Core/Manage/ThreadGraphManage.h"

void FThreadGraphManage::Wait(const FGraphEventRef& EventRef)
{
	FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);
}

void FThreadGraphManage::Wait(const FGraphEventArray& EventRefArray)
{
	FTaskGraphInterface::Get().WaitUntilTasksComplete(EventRefArray);
}
