// Copyright The Prototypers, Inc. All Rights Reserved.

#include "Gameplay/CheckpointSystem/Components/CheckpointsManager.h"
#include "Gameplay/CheckpointSystem/Utility/CSUtility.h"

TMap<FName, FTransform> UCheckpointsManager::CurrentCheckpoints = TMap<FName, FTransform>();

UCheckpointsManager::UCheckpointsManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

FCheckpointsSaveData UCheckpointsManager::CreateSaveData()
{
	FCheckpointsSaveData SaveData;
	SaveData.CurrentCheckpoints = CurrentCheckpoints;
	return SaveData;
}

void UCheckpointsManager::LoadSaveData(const FCheckpointsSaveData SaveData)
{
	CurrentCheckpoints = SaveData.CurrentCheckpoints;
}

void UCheckpointsManager::SetMapCheckpoint(const FTransform CheckpointTransform) const
{
	const FName MapName = GetCurrentMapName();
	CurrentCheckpoints.Add(MapName, CheckpointTransform);
	OnCheckpointReached.Broadcast();
}

bool UCheckpointsManager::TryGetMapCheckpoint(FTransform& OutCheckpointTransform) const
{
	if (CurrentCheckpoints.Contains(GetCurrentMapName()))
	{
		OutCheckpointTransform = CurrentCheckpoints[GetCurrentMapName()];
		return true;
	}

	return false;
}

void UCheckpointsManager::BeginPlay()
{
	Super::BeginPlay();
	UCSUtility::Init(this);
}

FName UCheckpointsManager::GetCurrentMapName() const
{
	return FName(GetWorld()->GetMapName());
}
