#include "Core/SimpleThreadType.h"
#include "Interface/ProxyInterface.h"


FSimpleThreadHandle::FSimpleThreadHandle()
	:GUIDTypeA(RandomSpawnUniqueID())
	,GUIDTypeB(RandomSpawnUniqueID())
	,GUIDTypeC(RandomSpawnUniqueID())
	,GUIDTypeD(RandomSpawnUniqueID())
{
	
}

uint64 FSimpleThreadHandle::RandomSpawnUniqueID()
{
	return FMath::Abs(FMath::RandRange(100,1256343) + FDateTime::Now().GetTicks());
}

IThreadProxy::IThreadProxy()
	:SimpleThreadHandle(MakeShareable(new FSimpleThreadHandle))
{
	
}
