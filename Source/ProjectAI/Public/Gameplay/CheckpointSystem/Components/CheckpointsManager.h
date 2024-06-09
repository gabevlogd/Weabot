// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Gameplay/CheckpointSystem/Data/Structs/CheckpointsSaveData.h"
#include "CheckpointsManager.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogCheckpointSystem, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCheckpointReached);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTAI_API UCheckpointsManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Checkpoints")
	FOnCheckpointReached OnCheckpointReached;

private:
	static FTransform CurrentCheckpointTransform;
	static bool bHasEverReachedCheckpoint;
	
public:
	UCheckpointsManager();

	UFUNCTION(BlueprintPure, Category = "Checkpoints")
	FCheckpointsSaveData CreateSaveData();
	
	UFUNCTION(BlueprintCallable, Category = "Checkpoints")
	void LoadSaveData(FCheckpointsSaveData SaveData);
	
	UFUNCTION(BlueprintCallable, Category = "Checkpoints")
	void SetCurrentCheckpointTransform(const FTransform Transform);

	UFUNCTION(BlueprintPure, Category = "Checkpoints")
	FTransform GetCurrentCheckpointTransform(bool& OutHasEverReachedCheckpoint);

protected:
	virtual void BeginPlay() override;
};
