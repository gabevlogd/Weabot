// Copyright The Prototypers, Inc. All Rights Reserved.


#include "Generic/SaveSystem/Components/Savables/OwnerTransformSavable.h"
#include "Generic/SaveSystem/Utility/SVUtility.h"


UOwnerTransformSavable::UOwnerTransformSavable()
{
}

void UOwnerTransformSavable::PushChanges_Implementation()
{
	Super::PushChanges_Implementation();
	if (!USVUtility::GetSaveManager()) return;

	USVUtility::GetSaveManager()->GetSaveGame()->ActorTransforms.Add(GetUniqueNameID(), GetOwnerTransform());
}

FTransform UOwnerTransformSavable::GetOwnerTransform() const
{
	return GetOwner()->GetActorTransform();
}

void UOwnerTransformSavable::OnSuccessfulLoad_Implementation(const FString& SlotName, const int32 UserIndex, UGenericSaveGame* LoadedData)
{
	Super::OnSuccessfulLoad_Implementation(SlotName, UserIndex, LoadedData);
	UE_LOG(LogTemp, Warning, TEXT("OwnerTransformSavable::OnSuccessfulLoad_Implementation"));
	
	const FName OwnerUniqueID = GetUniqueNameID();
	
	UE_LOG(LogTemp, Warning, TEXT("Owner Unique ID: %s"), *OwnerUniqueID.ToString());
	
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
