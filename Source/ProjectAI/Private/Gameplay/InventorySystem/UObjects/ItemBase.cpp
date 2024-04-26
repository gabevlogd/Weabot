// Copyright The Prototypers, Inc. All Rights Reserved.

#include "Gameplay/InventorySystem/UObjects/ItemBase.h"
#include "Gameplay/InventorySystem/Components/InventorySystem.h"
#include "Gameplay/InventorySystem/Data/DataAssets/ItemData.h"
#include "Kismet/GameplayStatics.h"

void UItemBase::Init(UItemData* Data, UInventorySystem* InventorySystem, UObject* Object)
{
	RelatedInventory = InventorySystem;
	ItemData = Data;
	CurrentQuantity = 1;
	ItemObject = Object;
	UE_LOG(LogInventorySystem, Display, TEXT("ItemBase::Init(), Item %s has been initialized in the inventory %s"), *ItemData->ItemName, *RelatedInventory->GetName());
	OnInit();
}

const UItemData* UItemBase::GetItemData() const
{
	return ItemData;
}

int32 UItemBase::GetNeededSlots() const
{
	int32 NeededSlots = 0;
	const int32 ItemMaxStack = ItemData->MaxStackSize;
	
	NeededSlots += CurrentQuantity / ItemMaxStack;
	if (CurrentQuantity % ItemMaxStack > 0) NeededSlots++;
	return NeededSlots;
}

int32 UItemBase::GetCurrentQuantity() const
{
	return CurrentQuantity;
}

void UItemBase::IncreaseQuantity()
{
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

	OnUse();
}

void UItemBase::Remove() const
{
	if (RelatedInventory->TryRemoveItem(ItemData)) // OnRemove() is called in TryRemoveItem()
		UE_LOG(LogInventorySystem, Display, TEXT("ItemBase::Remove(), Item %s has been removed from the inventory %s"), *ItemData->ItemName, *RelatedInventory->GetName());
}

void UItemBase::Consume()
{
	UE_LOG(LogInventorySystem, Display, TEXT("ItemBase::Consume(), Item %s has been consumed from the inventory %s"), *ItemData->ItemName, *RelatedInventory->GetName());

	DecreaseQuantity();
	OnConsume();

	if (CurrentQuantity <= 0)
		Remove();
}

void UItemBase::OnInit_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Item %s has been initialized in the inventory %s"), *ItemData->ItemName, *RelatedInventory->GetName()));
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
