// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ItemSlotData.generated.h"

USTRUCT(BlueprintType)
struct PROJECTAI_API FItemSlotData
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory System|Item")
	int32 SlotQuantity;
};