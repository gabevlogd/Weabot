// Copyright The Prototypers, Inc. All Rights Reserved.

#include "Gameplay/InventorySystem/Utility/ISFactory.h"
#include "Gameplay/InventorySystem/Data/DataAssets/ItemData.h"

UItemBase* UISFactory::CreateItem(UItemData* Data, UInventorySystem* InventorySystem)
{
	UItemBase* Item = NewObject<UItemBase>(GetTransientPackage(), Data->ItemClass);
	Item->Init(Data, InventorySystem);
	return Item;
}
