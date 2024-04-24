// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Gameplay/InventorySystem/UObjects/ItemBase.h"
#include "InventorySystem.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogInventorySystem, Log, All);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTAI_API UInventorySystem : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory System", meta = (ClampMin = "1", UIMin = "1"))
	int32 MaxSize = 10;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory System")
	TSet<UItemBase*> Items;

private:
	int32 CurrentSize;
	
public:
	UInventorySystem();

	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	bool AddItem(UItemData* ItemData);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	void RemoveItem(UItemData* ItemData);

	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	void RemoveAllItems();

	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	bool HasItem(UItemData* ItemData);

	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	bool Find(UItemData* ItemData, UItemBase*& OutItem);

	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	bool IsEmpty() const;

	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	bool IsFull() const;
};
