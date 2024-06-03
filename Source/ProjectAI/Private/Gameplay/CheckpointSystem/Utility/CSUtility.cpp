// Copyright The Prototypers, Inc. All Rights Reserved.

#include "Gameplay/CheckpointSystem/Utility/CSUtility.h"

UCheckpointsManager* UCSUtility::CurrCheckpointsManager = nullptr;

void UCSUtility::Init(UCheckpointsManager* CheckpointsManager)
{
	CurrCheckpointsManager = CheckpointsManager;
}

bool UCSUtility::TrySetCurrentCheckpoint(const ACheckpoint* NewCheckpoint)
{
	if (!Check()) return false;
	
	CurrCheckpointsManager->SetCurrentCheckpointTransform(NewCheckpoint->GetActorTransform());
	return true;
}

bool UCSUtility::TrySetCurrentCheckpointTransform(const FTransform& Transform)
{
	if (!Check()) return false;
	
	CurrCheckpointsManager->SetCurrentCheckpointTransform(Transform);
	return true;
}

bool UCSUtility::TryGetCurrentCheckpointTransform(FTransform& OutTransform, bool& OutHasEverReachedCheckpoint)
{
	if (!Check()) return false;
	
	OutTransform = CurrCheckpointsManager->GetCurrentCheckpointTransform(OutHasEverReachedCheckpoint);
	return true;
}

bool UCSUtility::Check()
{
	if (!CurrCheckpointsManager)
	{
		UE_LOG(LogCheckpointSystem, Error, TEXT("CheckpointsManager is null."));
		return false;
	}

	return true;
}
