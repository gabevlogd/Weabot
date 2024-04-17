// Copyright The Prototypers, Inc. All Rights Reserved.


#include "..\..\..\Public\Generic\SaveSystem\SlotSelectorManager.h"
#include "Generic/SaveSystem/Utility/SlotsUtility.h"


USaveManager* USlotSelectorManager::CurrentSaveManager = nullptr;
FString USlotSelectorManager::CurrentSlotName = "";

void USlotSelectorManager::Init(USaveManager* SaveManager)
{
	CurrentSaveManager = SaveManager;
	TrySelectMostRecentSaveGame();
}

FString USlotSelectorManager::GetSelectedSlotName()
{
	return CurrentSlotName;
}

bool USlotSelectorManager::TrySelectSaveGameSlot(const FString& SlotName)
{
	if (!USlotsUtility::DoesSlotFileExist(SlotName)) return false;
	CurrentSlotName = SlotName;
	return true;
}

bool USlotSelectorManager::TrySelectMostRecentSaveGame()
{
	if (FSlotInfoData SlotData; USlotsUtility::TryGetMostRecentSlotInfoData(SlotData))
	{
		TrySelectSaveGameSlot(SlotData.SlotName);
		return true;
	}

	return false;
}

bool USlotSelectorManager::TrySelectMostAncientSaveGame()
{
	if (FSlotInfoData SlotData; USlotsUtility::TryGetMostAncientSlotInfoData(SlotData))
	{
		TrySelectSaveGameSlot(SlotData.SlotName);
		return true;
	}

	return false;
}

void USlotSelectorManager::SaveOnSelectedSlot()
{
	if (USlotsUtility::IsSlotNameValid(CurrentSlotName))
		CurrentSaveManager->Save(CurrentSlotName);
}

void USlotSelectorManager::LoadFromSelectedSlot()
{
	if (USlotsUtility::IsSlotNameValid(CurrentSlotName))
		CurrentSaveManager->Load(CurrentSlotName);
}