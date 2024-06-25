// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Gameplay/InventorySystem/Data/DataAssets/InventoryRegistry.h"
#include "Gameplay/InventorySystem/Data/Structs/InventorySaveData.h"
#include "Gameplay/InventorySystem/UObjects/ItemBase.h"
#include "InventorySystem.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogInventorySystem, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnAnyItemAdded,
	UItemBase*, Item
	);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnAnyItemConsumed,
	UItemBase*, Item
	);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnAnyItemUsed,
	UItemBase*, Item
	);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnAnyItemRemoved,
	UItemBase*, Item
	);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnInventoryCleared
	);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnInventoryModified
	);

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTAI_API UInventorySystem : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory System")
	UInventoryRegistry* InventoryRegistry;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory System", meta = (ClampMin = "1", UIMin = "1"))
	int32 InventorySlotsSize = 16;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory System")
	TSet<UItemBase*> Items;

	UPROPERTY(BlueprintAssignable, Category = "Inventory System")
	FOnInventoryCleared OnInventoryCleared;
	UPROPERTY(BlueprintAssignable, Category = "Inventory System")
	FOnInventoryModified OnInventoryModified;
	UPROPERTY(BlueprintAssignable, Category = "Inventory System")
	FOnAnyItemAdded OnAnyItemAdded;
	UPROPERTY(BlueprintAssignable, Category = "Inventory System")
	FOnAnyItemUsed OnAnyItemUsed;
	UPROPERTY(BlueprintAssignable, Category = "Inventory System")
	FOnAnyItemConsumed OnAnyItemConsumed;
	UPROPERTY(BlueprintAssignable, Category = "Inventory System")
	FOnAnyItemRemoved OnAnyItemRemoved;
	
public:
	UInventorySystem();

	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	virtual FInventorySaveData CreateSaveData() const;

	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	virtual void LoadSaveData(const FInventorySaveData InventorySaveData);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	UItemBase* AddItem(UItemData* ItemData);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	bool RemoveItem(UItemData* ItemData);

	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	void RemoveAllItems();

	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	UItemBase* Find(const UItemData* ItemData) const;

	UFUNCTION(BlueprintPure, Category = "Inventory System")
	int32 GetTotalMinRequiredSlotsCount() const;
	
	UFUNCTION(BlueprintPure, Category = "Inventory System")
	bool IsEmpty() const;

	UFUNCTION(BlueprintPure, Category = "Inventory System")
	bool IsFull() const;

	UFUNCTION(BlueprintPure, Category = "Inventory System")
	bool IsSatisfyingAllRequiredSlots() const;
	
	UFUNCTION(BlueprintPure, Category = "Inventory System")
	bool HasItem(const UItemData* ItemData) const;
	
	UFUNCTION(BlueprintPure, Category = "Inventory System")
	bool CanContainItem(const UItemData* ItemData) const;

	UFUNCTION(BlueprintPure, Category = "Inventory System")
	bool IsInRegistry(const UItemData* ItemData) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	UItemData* GetItemByID(const FName ItemID) const;

	bool Check() const;
	
protected:
	virtual void BeginPlay() override;
};
