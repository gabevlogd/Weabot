// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemData.generated.h"

UCLASS(Blueprintable, BlueprintType)
class PROJECTAI_API UItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory System")
	FString ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory System", meta=(ClampMin="1", UIMin="1"))
	int32 MaxStackSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory System")
	UObject* ObjectItem;

	FORCEINLINE FName GetItemID() const
	{
		const int32 Hash = GetTypeHash(GetFName()) + GetTypeHash(ItemName) + GetTypeHash(ObjectItem);
		const FString Hex = FString::Printf(TEXT("%08X"), Hash);
		return FName(*Hex);
	}
};
