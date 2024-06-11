// Copyright The Prototypers, Inc. All Rights Reserved.

#include "Gameplay/InventorySystem/UObjects/ItemBase.h"
#include "Gameplay/InventorySystem/Components/InventorySystem.h"
#include "Gameplay/InventorySystem/Data/DataAssets/ItemData.h"
#include "Kismet/GameplayStatics.h"

void UItemBase::Init(UItemData* Data, UInventorySystem* InventorySystem)
{
	RelatedInventory = InventorySystem;
	ItemData = Data;
	CurrentQuantity = 1;
	UE_LOG(LogInventorySystem, Display, TEXT("ItemBase::Init(), Item %s has been initialized in the inventory %s"), *ItemData->ItemName, *RelatedInventory->GetName());
	OnInit();
	OnItemAdded.Broadcast(CurrentQuantity); // Event when the item is added to the inventory for the first time
}

FItemSaveData UItemBase::CreateSaveData() const
{
	FItemSaveData ItemSaveData;
	ItemSaveData.Quantity = CurrentQuantity;
	return ItemSaveData;
}

void UItemBase::LoadSaveData(const FItemSaveData ItemSaveData)
{
	CurrentQuantity = ItemSaveData.Quantity;
}

UItemData* UItemBase::GetItemData() const
{
	return ItemData;
}

TArray<FItemSlotData> UItemBase::GetRequiredSlots() const
{
	const int32 ItemMaxStack = GetMaxStackSize();
	const int32 NeededSlots = GetMinRequiredSlots();

	TArray<FItemSlotData> Slots;
	for (int32 i = 0; i < NeededSlots; i++)
	{
		FItemSlotData Slot;
		Slot.StackSize = ItemMaxStack;
		Slot.SlotQuantity = FMath::Min(CurrentQuantity - (i * ItemMaxStack), ItemMaxStack);
		Slots.Add(Slot);
	}

	return Slots;
}

int32 UItemBase::GetMinRequiredSlots() const
{
	const int32 ItemMaxStack = GetMaxStackSize();
	int32 NeededSlotsCount = CurrentQuantity / ItemMaxStack;
	if (CurrentQuantity % ItemMaxStack > 0) NeededSlotsCount++;

	return NeededSlotsCount;
}

int32 UItemBase::GetMaxStackSize() const
{
	return ItemData->MaxStackSize;
}

int32 UItemBase::GetCurrentQuantity() const
{
	return CurrentQuantity;
}

void UItemBase::IncreaseQuantity()
{
	OnItemAdded.Broadcast(CurrentQuantity);
	CurrentQuantity++;
}

void UItemBase::DecreaseQuantity()
{
	if (CurrentQuantity - 1 > 0)
		CurrentQuantity--;
	else
		CurrentQuantity = 0;
}

void UItemBase::Use()
{
	UE_LOG(LogInventorySystem, Display, TEXT("ItemBase::Use(), Item %s has been used from the inventory %s"), *ItemData->ItemName, *RelatedInventory->GetName());
	if (ItemData->bIsConsumable)
		Consume();

	OnItemUsed.Broadcast();
	RelatedInventory->OnAnyItemUsed.Broadcast(this);
	OnUse();
}

void UItemBase::Remove()
{
	if (RelatedInventory->RemoveItem(ItemData))
	{
		// OnRemove is called in the InventorySystem->RemoveItem
		// otherwise ONLY when calling Remove() from the ItemBase, it will trigger the OnRemove event
		UE_LOG(LogInventorySystem, Display, TEXT("ItemBase::Remove(), Item %s has been removed from the inventory %s"), *ItemData->ItemName, *RelatedInventory->GetName());
	}
}

void UItemBase::Consume()
{
	UE_LOG(LogInventorySystem, Display, TEXT("ItemBase::Consume(), Item %s has been consumed from the inventory %s"), *ItemData->ItemName, *RelatedInventory->GetName());

	DecreaseQuantity();
	OnConsume();
	OnItemConsumed.Broadcast(CurrentQuantity);
	RelatedInventory->OnAnyItemConsumed.Broadcast(this);
	RelatedInventory->OnInventoryModified.Broadcast();
	
	if (CurrentQuantity <= 0)
		Remove();
}

void UItemBase::OnInit_Implementation()
{
}

void UItemBase::OnRemove_Implementation()
{
}

void UItemBase::OnUse_Implementation()
{
}

void UItemBase::OnConsume_Implementation()
{
}

AGameModeBase* UItemBase::GetGameMode() const
{
	return UGameplayStatics::GetGameMode(RelatedInventory);
}

APlayerController* UItemBase::GetPlayerController(const int PlayerIndex) const
{
	return UGameplayStatics::GetPlayerController(RelatedInventory, PlayerIndex);
}

APlayerState* UItemBase::GetPlayerState(const int PlayerStateIndex) const
{
	return UGameplayStatics::GetPlayerState(RelatedInventory, PlayerStateIndex);
}
