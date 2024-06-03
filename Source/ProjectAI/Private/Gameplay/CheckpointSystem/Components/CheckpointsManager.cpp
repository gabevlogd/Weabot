// Copyright The Prototypers, Inc. All Rights Reserved.

#include "Gameplay/CheckpointSystem/Components/CheckpointsManager.h"
#include "Gameplay/CheckpointSystem/Utility/CSUtility.h"

FTransform UCheckpointsManager::CurrentCheckpointTransform = FTransform::Identity;
bool UCheckpointsManager::bHasEverReachedCheckpoint = false;

UCheckpointsManager::UCheckpointsManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

FCheckpointsSaveData UCheckpointsManager::CreateSaveData()
{
	FCheckpointsSaveData SaveData;
	SaveData.CurrentCheckpointTransform = CurrentCheckpointTransform;
	SaveData.bHasEverReachedCheckpoint = bHasEverReachedCheckpoint;
	return SaveData;
}

void UCheckpointsManager::LoadSaveData(const FCheckpointsSaveData SaveData)
{
	CurrentCheckpointTransform = SaveData.CurrentCheckpointTransform;
	bHasEverReachedCheckpoint = SaveData.bHasEverReachedCheckpoint;
}

void UCheckpointsManager::SetCurrentCheckpointTransform(const FTransform Transform)
{
	bHasEverReachedCheckpoint = true;
	CurrentCheckpointTransform = Transform;
	OnCheckpointReached.Broadcast();
}

FTransform UCheckpointsManager::GetCurrentCheckpointTransform(bool& OutHasEverReachedCheckpoint)
{
	OutHasEverReachedCheckpoint = bHasEverReachedCheckpoint;
	return CurrentCheckpointTransform;
}

void UCheckpointsManager::BeginPlay()
{
	Super::BeginPlay();
	UCSUtility::Init(this);
}
