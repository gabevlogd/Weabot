// Copyright The Prototypers, Inc. All Rights Reserved.

#include "Gameplay/CheckpointSystem/Utility/CSUtility.h"

UCheckpointsManager* UCSUtility::CurrCheckpointsManager = nullptr;

void UCSUtility::Init(UCheckpointsManager* CheckpointsManager)
{
	CurrCheckpointsManager = CheckpointsManager;
}

bool UCSUtility::TrySetMapCheckpoint(const FTransform& CheckpointTransform)
{
	if (!Check()) return false;
	
	CurrCheckpointsManager->SetMapCheckpoint(CheckpointTransform);
	return true;
}

bool UCSUtility::TryGetMapCheckpoint(FTransform& OutCheckpointTransform)
{
	if (!Check()) return false;

	return CurrCheckpointsManager->TryGetMapCheckpoint(OutCheckpointTransform);
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
