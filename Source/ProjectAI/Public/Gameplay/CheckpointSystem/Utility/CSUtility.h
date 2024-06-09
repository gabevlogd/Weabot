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
	static bool TrySetCurrentCheckpointTransform(const FTransform& Transform);

	UFUNCTION(BlueprintPure, Category = "Checkpoints")
	static bool TryGetCurrentCheckpointTransform(FTransform& OutTransform, bool& OutHasEverReachedCheckpoint);

private:
	static bool Check();
};
