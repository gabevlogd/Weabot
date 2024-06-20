// Copyright The Prototypers, Inc. All Rights Reserved.

#include "Gameplay/CheckpointSystem/Components/CheckpointsManager.h"
#include "Gameplay/CheckpointSystem/Utility/CSUtility.h"
#include "Kismet/GameplayStatics.h"

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

void UCheckpointsManager::SetCheckpoint(const FName CheckpointKey, const FTransform CheckpointTransform) const
{
	CurrentCheckpoints.Add(CheckpointKey, CheckpointTransform);
	OnCheckpointReached.Broadcast();
}

bool UCheckpointsManager::TryGetMapCheckpoint(const FName CheckpointKey, FTransform& OutCheckpointTransform) const
{
	if (CurrentCheckpoints.Contains(CheckpointKey))
	{
		OutCheckpointTransform = CurrentCheckpoints[CheckpointKey];
		return true;
	}

	return false;
}

void UCheckpointsManager::BeginPlay()
{
	Super::BeginPlay();
	UCSUtility::Init(this);
}
