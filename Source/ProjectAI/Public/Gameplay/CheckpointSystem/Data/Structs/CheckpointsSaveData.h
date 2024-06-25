#pragma once

#include "CoreMinimal.h"
#include "CheckpointsSaveData.generated.h"

USTRUCT(BlueprintType)
struct FCheckpointsSaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, FTransform> CurrentCheckpoints;
};
