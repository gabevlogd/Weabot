// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ItemSaveData.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FItemSaveData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory System|Item")
	int32 Quantity;
};
