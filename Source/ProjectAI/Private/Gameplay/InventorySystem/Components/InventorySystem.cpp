// Copyright The Prototypers, Inc. All Rights Reserved.

#include "Gameplay/InventorySystem/Components/InventorySystem.h"
#include "Gameplay/InventorySystem/Data/DataAssets/ItemData.h"
#include "Gameplay/InventorySystem/Utility/ISFactory.h"

UInventorySystem::UInventorySystem()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UInventorySystem::AddItem(UItemData* ItemData, UObject* ItemObject)
{
	if (!CanContainItem(ItemData))
	{
		UE_LOG(LogInventorySystem, Display, TEXT("Cannot add item, inventory is full."));
		return false;
	}

	UE_LOG(LogInventorySystem, Display, TEXT("Adding Item %s to inventory %s"), *ItemData->ItemName, *GetName())
	if (UItemBase* FoundItem = Find(ItemData))
	{
		FoundItem->IncreaseQuantity(); // Increase quantity of the item, if it already exists in the inventory
		OnItemAdded.Broadcast(FoundItem);
		OnInventoryModified.Broadcast();
		return true;
	}

	UItemBase* CreatedItem = UISFactory::CreateItem(ItemData, this, ItemObject);
	Items.Add(CreatedItem);
	OnItemAdded.Broadcast(CreatedItem);
	OnInventoryModified.Broadcast();
	return true;
}

bool UInventorySystem::RemoveItem(UItemData* ItemData)
{
	if (UItemBase* FoundItem = Find(ItemData))
	{
		Items.Remove(FoundItem);
		FoundItem->OnRemove();
		OnAnyItemRemoved.Broadcast(FoundItem);
		OnInventoryModified.Broadcast();
		return true;
	}

	return false;
}

void UInventorySystem::RemoveAllItems()
{
	Items.Empty();
	OnInventoryModified.Broadcast();
	OnInventoryCleared.Broadcast();
}

UItemBase* UInventorySystem::Find(const UItemData* ItemData) const
{
	for (UItemBase* Item : Items)
	{
		if (Item->GetItemData()->GetItemID() == ItemData->GetItemID())
			return Item;
	}

	return nullptr;
}

int32 UInventorySystem::GetTotalMinRequiredSlotsCount() const
{
	int32 CurrSlots = 0;
	for (const UItemBase* Item : Items)
		CurrSlots += Item->GetMinRequiredSlots();

	return CurrSlots;
}

bool UInventorySystem::IsEmpty() const
{
	return Items.IsEmpty();
}

bool UInventorySystem::IsFull() const
{
	if (IsSatisfyingAllRequiredSlots()) return false;

	for (const UItemBase* Item : Items)
	{
		for (const FItemSlotData& Slot : Item->GetRequiredSlots()) // For each slot the item occupies
		{
			if (!Slot.IsMaxStacked())
				return false;
		}
	}

	return true;
}

bool UInventorySystem::IsSatisfyingAllRequiredSlots() const
{
	return GetTotalMinRequiredSlotsCount() < InventorySlotsSize;
}

bool UInventorySystem::HasItem(const UItemData* ItemData) const
{
	for (const UItemBase* Item : Items)
	{
		if (Item->GetItemData()->GetItemID() == ItemData->GetItemID())
			return true;
	}

	return false;
}

bool UInventorySystem::CanContainItem(const UItemData* ItemData) const
{
	if (IsFull()) return false;

	if (const UItemBase* FoundItem = Find(ItemData))
	{
		for (const FItemSlotData& Slot : FoundItem->GetRequiredSlots())
		{
			if (!Slot.IsMaxStacked())
				return true;
		}
	}

	return IsSatisfyingAllRequiredSlots();
}
