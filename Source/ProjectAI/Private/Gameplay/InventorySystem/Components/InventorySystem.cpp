// Copyright The Prototypers, Inc. All Rights Reserved.


#include "Gameplay/InventorySystem/Components/InventorySystem.h"
#include "Gameplay/InventorySystem/Utility/ISFactory.h"


UInventorySystem::UInventorySystem()
{
	PrimaryComponentTick.bCanEverTick = false;
	CurrentSize = 0;
}

bool UInventorySystem::AddItem(UItemData* ItemData)
{
	if (IsFull())
		UE_LOG(LogInventorySystem, Display, TEXT("Cannot add item, inventory is full."));

	UItemBase* FoundItem = nullptr;
	if (Find(ItemData, FoundItem) && FoundItem->GetQuantity() < ItemData->MaxStackSize)
	{
		FoundItem->Increment();
		return true;
	}

	UItemBase* Item = UISFactory::CreateItem(ItemData);
	if (const auto Result = Items.Add(Item); Result.IsValidId())
	{
		UE_LOG(LogInventorySystem, Error, TEXT("Failed to add item, the item is already in the TSet Items."));
		return false;
	}
		
	CurrentSize++;
	return true;
}

void UInventorySystem::RemoveItem(UItemData* ItemData)
{
	UItemBase* FoundItem = nullptr;
	if (Find(ItemData, FoundItem))
	{
		CurrentSize--;
		Items.Remove(FoundItem);
	}
}

void UInventorySystem::RemoveAllItems()
{
	CurrentSize = 0;
	Items.Empty();
}

bool UInventorySystem::HasItem(UItemData* ItemData)
{
	for (const UItemBase* Item : Items)
	{
		if (Item->GetItemData().GetItemID() == ItemData->GetItemID())
			return true;
	}

	return false;
}

bool UInventorySystem::Find(UItemData* ItemData, UItemBase*& OutItem)
{
	for (UItemBase* Item : Items)
	{
		if (Item->GetItemData().GetItemID() == ItemData->GetItemID())
		{
			OutItem = Item;
			return true;
		}
	}

	return false;
}

bool UInventorySystem::IsEmpty() const
{
	return Items.IsEmpty();
}

bool UInventorySystem::IsFull() const
{
	return CurrentSize >= MaxSize;
}
