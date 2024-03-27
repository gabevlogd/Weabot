// Copyright The Prototypers, Inc. All Rights Reserved.


#include "Generic/SaveSystem/Components/Savables/TransformSaver.h"
#include "Generic/SaveSystem/Utility/SVUtility.h"


UTransformSaver::UTransformSaver()
{
}

void UTransformSaver::OnPrepareSave_Implementation(UGenericSaveGame* SaveGameData)
{
	if (!USVUtility::GetSaveManager()) return;
	
	USVUtility::GetSaveManager()->GetSaveGame()->ActorTransforms.Add(GetUniqueNameID(), GetOwnerTransform());
}

void UTransformSaver::OnLoadCompletedEvent_Implementation(const FString& SlotName, const int32 UserIndex, UGenericSaveGame* LoadedData)
{
	Super::OnLoadCompletedEvent_Implementation(SlotName, UserIndex, LoadedData);
	const FName OwnerUniqueID = GetUniqueNameID();
	
	if (LoadedData->ActorTransforms.Contains(OwnerUniqueID))
	{
		GetOwner()->SetActorTransform(LoadedData->ActorTransforms[OwnerUniqueID]);
	}
	else
	{
		UE_LOG( LogTemp, Warning, TEXT("Owner Unique ID not found in the loaded data") );
		for (auto& ActorTransform : LoadedData->ActorTransforms)
		{
			UE_LOG(LogTemp, Warning, TEXT("Actor Unique ID: %s"), *ActorTransform.Key.ToString());
		}
	}
}

FTransform UTransformSaver::GetOwnerTransform() const
{
	return GetOwner()->GetActorTransform();
}

