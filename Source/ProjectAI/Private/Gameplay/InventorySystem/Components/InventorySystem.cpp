// Copyright The Prototypers, Inc. All Rights Reserved.

#include "Gameplay/InventorySystem/Components/InventorySystem.h"
#include "Gameplay/InventorySystem/Data/DataAssets/ItemData.h"
#include "Gameplay/InventorySystem/Utility/ISFactory.h"

UInventorySystem::UInventorySystem()
{
	InventoryRegistry = nullptr;
	PrimaryComponentTick.bCanEverTick = false;
}

FInventorySaveData UInventorySystem::CreateSaveData() const
{
	FInventorySaveData InventorySaveData;
	
	for (const UItemBase* Item : Items)
	{
		FItemSaveData ItemSaveData = Item->CreateSaveData();
		FName ItemID = Item->GetItemData()->GetItemID();
		InventorySaveData.SavedItems.Add(ItemID, ItemSaveData);
	}
	
	return InventorySaveData;
}

void UInventorySystem::LoadSaveData(const FInventorySaveData InventorySaveData)
{	
	for (const TPair<FName, FItemSaveData>& LoadedItem : InventorySaveData.SavedItems)
	{
		const FName ItemID = LoadedItem.Key;
		const FItemSaveData ItemSaveData = LoadedItem.Value;
		
		if(UItemData* ItemData = GetItemByID(ItemID))
		{
			if(UItemBase* AddedItem = AddItem(ItemData))
				AddedItem->LoadSaveData(ItemSaveData);
		}
		else
		{
			UE_LOG(LogInventorySystem, Warning, TEXT("Item with ID %s not found in the inventory registry %s"), *ItemID.ToString(), *GetName());
		}
	}
}

UItemBase* UInventorySystem::AddItem(UItemData* ItemData)
{
	if (!CanContainItem(ItemData))
	{
		UE_LOG(LogInventorySystem, Display, TEXT("Cannot add item, inventory is full."));
		return nullptr;
	}

	UE_LOG(LogInventorySystem, Display, TEXT("Adding Item %s to inventory %s"), *ItemData->ItemName, *GetName())
	if (UItemBase* FoundItem = Find(ItemData))
	{
		FoundItem->IncreaseQuantity(); // Increase quantity of the item, if it already exists in the inventory
		OnItemAdded.Broadcast(FoundItem);
		OnInventoryModified.Broadcast();
		return FoundItem;
	}

	UItemBase* CreatedItem = UISFactory::CreateItem(ItemData, this);
	Items.Add(CreatedItem);
	OnItemAdded.Broadcast(CreatedItem);
	OnInventoryModified.Broadcast();
	return CreatedItem;
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
	if (!IsInRegistry(ItemData))
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("Item %s is not registered in the inventory registry %s"), *ItemData->ItemName, *GetName());
		return false;
	}
	
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

bool UInventorySystem::IsInRegistry(const UItemData* ItemData) const
{
	return InventoryRegistry->RegisteredItems.Contains(ItemData);
}

UItemData* UInventorySystem::GetItemByID(const FName ItemID) const
{
	for (UItemData* ItemData : InventoryRegistry->RegisteredItems)
	{
		if (ItemData->GetItemID() == ItemID)
			return ItemData;
	}
	
	return nullptr;
}

bool UInventorySystem::Check() const
{
	if (InventoryRegistry == nullptr)
	{
		UE_LOG(LogInventorySystem, Error, TEXT("Inventory Registry is not set for inventory %s"), *GetName());
		return false;
	}

	return true;
}

void UInventorySystem::BeginPlay()
{
	Super::BeginPlay();
	Check();
}
