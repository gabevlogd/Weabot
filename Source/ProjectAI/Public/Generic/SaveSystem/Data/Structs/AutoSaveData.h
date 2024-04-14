// Copyright Denis Faraci, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AutoSaveData.generated.h"

USTRUCT(BlueprintType)
struct FAutoSaveData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Save System")
	bool bInitPauseState;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Save System", meta = (ClampMin = "30.0"))
	float AutoSaveIntervalSeconds = 60.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Save System", meta = (ClampMin = "1"))
	int32 MaxAutoSaves = 3;

	FAutoSaveData()
	{
		bInitPauseState = false;
		AutoSaveIntervalSeconds = 60.0f;
		MaxAutoSaves = 3;
	}

	FAutoSaveData(const bool bInitPause, const float IntervalSeconds, const int32 MaxAutoSavesNumber)
	{
		bInitPauseState = bInitPause;
		AutoSaveIntervalSeconds = IntervalSeconds;
		MaxAutoSaves = MaxAutoSavesNumber;
	}
};
