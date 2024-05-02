// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ItemSaveData.h"
#include "Gameplay/InventorySystem/Data/DataAssets/ItemData.h"
#include "InventorySaveData.generated.h"

USTRUCT(BlueprintType)
struct FInventorySaveData
{
	GENERATED_BODY()
	
	TMap<FName, FItemSaveData> SavedItems;
};
