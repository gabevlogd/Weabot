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
	if (IsFull()) // This works because the items are added one by one
	{
		UE_LOG(LogInventorySystem, Display, TEXT("Cannot add item, inventory is full."));
		return false;
	}

	UE_LOG(LogInventorySystem, Display, TEXT("Adding Item %s to inventory %s"), *ItemData->ItemName, *GetName())
	if (UItemBase* FoundItem = FindByItemID(ItemData))
	{
		FoundItem->IncreaseQuantity(); // Increase quantity of the item, if it already exists in the inventory
		OnItemAdded.Broadcast(FoundItem);
		return true;
	}

	UItemBase* CreatedItem = UISFactory::CreateItem(ItemData, this, ItemObject);
	Items.Add(CreatedItem);
	OnItemAdded.Broadcast(CreatedItem);
	return true;
}

bool UInventorySystem::TryRemoveItem(UItemData* ItemData)
{
	if (UItemBase* FoundItem = FindByItemID(ItemData))
	{
		OnItemRemoved.Broadcast(FoundItem);
		Items.Remove(FoundItem);
		FoundItem->OnRemove();
		return true;
	}

	return false;
}

void UInventorySystem::RemoveAllItems()
{
	Items.Empty();
	OnInventoryCleared.Broadcast();
}

bool UInventorySystem::HasItem(const UItemData* ItemData)
{
	for (const UItemBase* Item : Items)
	{
		if (Item->GetItemData()->GetItemID() == ItemData->GetItemID())
			return true;
	}

	return false;
}

UItemBase* UInventorySystem::FindByItemID(const UItemData* ItemData)
{
	for (UItemBase* Item : Items)
	{
		if (Item->GetItemData()->GetItemID() == ItemData->GetItemID())
			return Item;
	}

	return nullptr;
}

int32 UInventorySystem::GetOccupiedSlots() const
{
	int32 CurrSlots = 0;
	for (const UItemBase* Item : Items)
		CurrSlots += Item->GetNeededSlots();

	return CurrSlots;
}

bool UInventorySystem::IsEmpty() const
{
	return Items.IsEmpty();
}

bool UInventorySystem::IsFull() const
{
	return GetOccupiedSlots() >= InventorySlotsSize;
}
