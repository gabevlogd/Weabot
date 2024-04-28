// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/InventorySystem/Data/Structs/ItemSlotData.h"
#include "UObject/Object.h"
#include "ItemBase.generated.h"

class UInventorySystem;
class UItemData;

UCLASS(Blueprintable, BlueprintType)
class PROJECTAI_API UItemBase : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory System|Item")
	UItemData* ItemData;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory System|Item")
	UInventorySystem* RelatedInventory;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory System|Item")
	UObject* ItemObject;
	
private:
	int32 CurrentQuantity;

public:
	void Init(UItemData* Data, UInventorySystem* InventorySystem, UObject* Object = nullptr);

	UFUNCTION(BlueprintPure, Category = "Inventory System|Item")
	const UItemData* GetItemData() const;

	UFUNCTION(BlueprintPure, Category = "Inventory System|Item")
	TArray<FItemSlotData> GetNeededSlots() const;

	UFUNCTION(BlueprintPure, Category = "Inventory System|Item")
	int32 GetNeededSlotsCount() const;
	
	UFUNCTION(BlueprintPure, Category = "Inventory System|Item")
	int32 GetMaxStackSize() const;
	
	UFUNCTION(BlueprintPure, Category = "Inventory System|Item")
	int32 GetCurrentQuantity() const;

	UFUNCTION(BlueprintCallable, Category = "Inventory System|Item")
	void IncreaseQuantity();

	UFUNCTION(BlueprintCallable, Category = "Inventory System|Item")
	void DecreaseQuantity();
	
	UFUNCTION(BlueprintCallable, Category = "Inventory System|Item")
	void Use();
	
	UFUNCTION(BlueprintCallable, Category = "Inventory System|Item")
	void Remove() const;
	
	UFUNCTION(BlueprintCallable, Category = "Inventory System|Item")
	void Consume();
	
	UFUNCTION(BlueprintNativeEvent, Category = "Inventory System|Item")
	void OnRemove();
	
protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Inventory System|Item")
	void OnInit();
	
	UFUNCTION(BlueprintNativeEvent, Category = "Inventory System|Item")
	void OnUse();
	
	UFUNCTION(BlueprintNativeEvent, Category = "Inventory System|Item")
	void OnConsume();
	
	UFUNCTION(BlueprintPure, Category = "Inventory System|Item")
	AGameModeBase* GetGameMode() const;

	UFUNCTION(BlueprintPure, Category = "Inventory System|Item")
	APlayerController* GetPlayerController(const int PlayerIndex) const;

	UFUNCTION(BlueprintPure, Category = "Inventory System|Item")
	APlayerState* GetPlayerState(const int PlayerStateIndex) const;
};
