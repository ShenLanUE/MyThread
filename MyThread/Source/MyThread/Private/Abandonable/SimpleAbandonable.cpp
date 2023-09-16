#include "Abandonable/SimpleAbandonable.h"

FSimpleAbandonable::FSimpleAbandonable(const FSimpleDelegate& InThreadDelegate)
	:ThreadDelegate(InThreadDelegate)
{
	
}

void FSimpleAbandonable::DoWork()
{
	if(ThreadDelegate.IsBound())
	{
		ThreadDelegate.Execute();
	}
}
