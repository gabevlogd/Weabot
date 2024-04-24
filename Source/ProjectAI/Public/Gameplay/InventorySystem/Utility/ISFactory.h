// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/InventorySystem/UObjects/ItemBase.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ISFactory.generated.h"


UCLASS()
class PROJECTAI_API UISFactory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static UItemBase* CreateItem(UItemData* Data);
};
