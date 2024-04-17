// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "QuestFilterData.generated.h"


UCLASS(BlueprintType)
class PROJECTAI_API UQuestFilterData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Filter Data")
	FString QuestFilterName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Filter Data")
	FSlateBrush QuestFilterIcon;
};
