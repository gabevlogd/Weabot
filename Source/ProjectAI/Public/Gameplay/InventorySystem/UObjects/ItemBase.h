// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/InventorySystem/Data/DataAssets/ItemData.h"
#include "UObject/Object.h"
#include "ItemBase.generated.h"


UCLASS(NotBlueprintable, BlueprintType)
class PROJECTAI_API UItemBase : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory System", meta = (AllowPrivateAccess = "true"))
	UItemData* ItemData;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory System", meta = (AllowPrivateAccess = "true"))
	int32 CurrentQuantity;
	
public:
	void Init(UItemData* Data);
	void SetQuantity(int32 Quantity);
	void Increment();
	const UItemData& GetItemData() const;
	int32 GetQuantity() const;
};
