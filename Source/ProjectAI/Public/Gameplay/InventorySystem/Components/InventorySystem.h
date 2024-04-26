// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Gameplay/InventorySystem/UObjects/ItemBase.h"
#include "InventorySystem.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogInventorySystem, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnItemAdded,
	UItemBase*, Item
	);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnItemConsumed,
	UItemBase*, Item
	);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnItemRemoved,
	UItemBase*, Item
	);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnInventoryCleared
	);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTAI_API UInventorySystem : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory System", meta = (ClampMin = "1", UIMin = "1"))
	int32 InventorySlotsSize = 10;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory System")
	TSet<UItemBase*> Items;

	UPROPERTY(BlueprintAssignable, Category = "Inventory System")
	FOnItemAdded OnItemAdded;
	UPROPERTY(BlueprintAssignable, Category = "Inventory System")
	FOnItemConsumed OnItemConsumed;
	UPROPERTY(BlueprintAssignable, Category = "Inventory System")
	FOnItemRemoved OnItemRemoved;
	UPROPERTY(BlueprintAssignable, Category = "Inventory System")
	FOnInventoryCleared OnInventoryCleared;
	
public:
	UInventorySystem();

	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	bool AddItem(UItemData* ItemData, UObject* ItemObject = nullptr);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	bool TryRemoveItem(UItemData* ItemData);

	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	void RemoveAllItems();

	UFUNCTION(BlueprintPure, Category = "Inventory System")
	bool HasItem(const UItemData* ItemData);

	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	UItemBase* FindByItemID(const UItemData* ItemData);

	UFUNCTION(BlueprintPure, Category = "Inventory System")
	int32 GetOccupiedSlots() const;

	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	bool IsEmpty() const;

	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	bool IsFull() const;
};
