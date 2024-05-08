// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "Engine/DataAsset.h"
#include "InventoryRegistry.generated.h"

UCLASS(NotBlueprintable, BlueprintType)
class PROJECTAI_API UInventoryRegistry : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory System|Registry")
	TSet<UItemData*> RegisteredItems;
};
