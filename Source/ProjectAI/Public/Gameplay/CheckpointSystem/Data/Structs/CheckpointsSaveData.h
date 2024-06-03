#pragma once

#include "CoreMinimal.h"
#include "CheckpointsSaveData.generated.h"

USTRUCT(BlueprintType)
struct FCheckpointsSaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform CurrentCheckpointTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasEverReachedCheckpoint;
};
