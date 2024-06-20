// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/CheckpointSystem/Components/CheckpointsManager.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CSUtility.generated.h"

UCLASS()
class PROJECTAI_API UCSUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	static UCheckpointsManager* CurrCheckpointsManager;
	
public:
	static void Init(UCheckpointsManager* CheckpointsManager);

	UFUNCTION(BlueprintCallable, Category = "Checkpoints")
	static bool TrySetCheckpoint(const FName CheckpointKey, const FTransform& CheckpointTransform);

	UFUNCTION(BlueprintPure, Category = "Checkpoints")
	static bool TryGetCheckpoint(const FName CheckpointKey, FTransform& OutCheckpointTransform);

private:
	static bool Check();
};
