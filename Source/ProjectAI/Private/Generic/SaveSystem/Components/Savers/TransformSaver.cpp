// Copyright The Prototypers, Inc. All Rights Reserved.


#include "Generic/SaveSystem/Components/Savers/TransformSaver.h"
#include "Generic/SaveSystem/Utility/SSUtility.h"


UTransformSaver::UTransformSaver()
{
}

void UTransformSaver::OnPrepareSave_Implementation(UDefaultSaveGame* SaveGameData, USlotInfoItem* SlotInfoItem)
{
	if (!USSUtility::GetSaveManager()) return;

	USSUtility::GetSaveManager()->GetSaveGameInstance()->ActorTransforms.Add(GetUniqueSaveID(), GetOwnerTransform());
}

void UTransformSaver::OnLoadCompletedEvent_Implementation(const FString& SlotName, const int32 UserIndex, UDefaultSaveGame* LoadedData)
{
	Super::OnLoadCompletedEvent_Implementation(SlotName, UserIndex, LoadedData);

	// Check if LoadedData and ActorTransforms are valid
	if (const FName OwnerUniqueID = GetUniqueSaveID(); LoadedData && LoadedData->ActorTransforms.Contains(OwnerUniqueID))
	{
		// Check if Owner is valid
		if (AActor* Owner = GetOwner())
			Owner->SetActorTransform(LoadedData->ActorTransforms[OwnerUniqueID]);
		else
			UE_LOG(LogTemp, Warning, TEXT("Owner not found in the loaded data, Saver Unique ID: %s"), *OwnerUniqueID.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner Unique ID not found in the loaded data"));
	}
}

FTransform UTransformSaver::GetOwnerTransform() const
{
	return GetOwner()->GetActorTransform();
}

