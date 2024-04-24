// Copyright The Prototypers, Inc. All Rights Reserved.


#include "Gameplay/InventorySystem/UObjects/ItemBase.h"


void UItemBase::Init(UItemData* Data)
{
	ItemData = Data;
	CurrentQuantity = 1;
}

void UItemBase::SetQuantity(int32 Quantity)
{
	if (Quantity < 1) Quantity = 1;
	CurrentQuantity = Quantity;
}

void UItemBase::Increment()
{
	CurrentQuantity++;
}

const UItemData& UItemBase::GetItemData() const
{
	return *ItemData;
}

int32 UItemBase::GetQuantity() const
{
	return CurrentQuantity;
}
