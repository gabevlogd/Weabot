// Copyright Denis Faraci, Inc. All Rights Reserved.


#include "Generic/SaveSystem/Components/Savables/TransformSaver.h"
#include "Generic/SaveSystem/Utility/SVUtility.h"


UTransformSaver::UTransformSaver()
{
}

void UTransformSaver::OnPrepareSave_Implementation(UDefaultSaveGame* SaveGameData)
{
	if (!USVUtility::GetSaveManager()) return;

	USVUtility::GetSaveManager()->GetSaveGame()->ActorTransforms.Add(GetUniqueSaveID(), GetOwnerTransform());
}

void UTransformSaver::OnLoadCompletedEvent_Implementation(const FString& SlotName, const int32 UserIndex, UDefaultSaveGame* LoadedData)
{
	Super::OnLoadCompletedEvent_Implementation(SlotName, UserIndex, LoadedData);
	const FName OwnerUniqueID = GetUniqueSaveID();
	
	if (LoadedData->ActorTransforms.Contains(OwnerUniqueID))
		GetOwner()->SetActorTransform(LoadedData->ActorTransforms[OwnerUniqueID]);
	else
		UE_LOG( LogTemp, Warning, TEXT("Owner Unique ID not found in the loaded data") );
}

FTransform UTransformSaver::GetOwnerTransform() const
{
	return GetOwner()->GetActorTransform();
}

