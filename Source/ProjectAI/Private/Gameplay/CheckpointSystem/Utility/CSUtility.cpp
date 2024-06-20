// Copyright The Prototypers, Inc. All Rights Reserved.

#include "Gameplay/CheckpointSystem/Utility/CSUtility.h"

UCheckpointsManager* UCSUtility::CurrCheckpointsManager = nullptr;

void UCSUtility::Init(UCheckpointsManager* CheckpointsManager)
{
	CurrCheckpointsManager = CheckpointsManager;
}

bool UCSUtility::TrySetCheckpoint(const FName CheckpointKey, const FTransform& CheckpointTransform)
{
	if (!Check()) return false;
	
	CurrCheckpointsManager->SetCheckpoint(CheckpointKey, CheckpointTransform);
	return true;
}

bool UCSUtility::TryGetCheckpoint(const FName CheckpointKey, FTransform& OutCheckpointTransform)
{
	if (!Check()) return false;

	return CurrCheckpointsManager->TryGetMapCheckpoint(CheckpointKey, OutCheckpointTransform);
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
