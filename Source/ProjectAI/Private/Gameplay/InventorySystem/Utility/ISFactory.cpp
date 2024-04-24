// Copyright The Prototypers, Inc. All Rights Reserved.

#include "Gameplay/InventorySystem/Utility/ISFactory.h"


UItemBase* UISFactory::CreateItem(UItemData* Data)
{
	UItemBase* Item = NewObject<UItemBase>();
	Item->Init(Data);
	return Item;
}
