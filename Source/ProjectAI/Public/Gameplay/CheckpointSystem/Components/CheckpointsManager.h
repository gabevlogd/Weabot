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
	static TMap<FName, FTransform> CurrentCheckpoints;
	
public:
	UCheckpointsManager();

	UFUNCTION(BlueprintPure, Category = "Checkpoints")
	static FCheckpointsSaveData CreateSaveData();
	
	UFUNCTION(BlueprintCallable, Category = "Checkpoints")
	void LoadSaveData(FCheckpointsSaveData SaveData);
	
	UFUNCTION(BlueprintCallable, Category = "Checkpoints")
	void SetCheckpoint(const FName CheckpointKey, const FTransform CheckpointTransform) const;

	UFUNCTION(BlueprintPure, Category = "Checkpoints")
	bool TryGetMapCheckpoint(const FName CheckpointKey, FTransform& OutCheckpointTransform) const;

protected:
	virtual void BeginPlay() override;
};
