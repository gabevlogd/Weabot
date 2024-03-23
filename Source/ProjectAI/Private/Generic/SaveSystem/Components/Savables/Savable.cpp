// Copyright The Prototypers, Inc. All Rights Reserved.


#include "Generic/SaveSystem/Components/Savables/Savable.h"
#include "Generic/SaveSystem/Utility/SVUtility.h"
#include "Kismet/GameplayStatics.h"


USavable::USavable()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USavable::PushChanges_Implementation()
{
}

void USavable::OnSuccessfulSave_Implementation(const FString& SlotName, const int32 UserIndex, bool bSuccess, UGenericSaveGame* SaveGameData)
{
	OnGameSaved.Broadcast(SlotName, UserIndex, bSuccess, SaveGameData);
}

void USavable::OnSuccessfulLoad_Implementation(const FString& SlotName, const int32 UserIndex, UGenericSaveGame* LoadedData)
{
	OnGameLoaded.Broadcast(SlotName, UserIndex, LoadedData);
}

FName USavable::GetUniqueNameID() const
{
	const FString LevelName = UGameplayStatics::GetCurrentLevelName(this, true);
	const FString OwnerName = GetOwner()->GetName();
	return FName(*FString::Printf(TEXT("%s_%s"), *LevelName, *OwnerName));
}

void USavable::BeginPlay()
{
	Super::BeginPlay();
	if (!USVUtility::GetSaveManager()) return;

	USVUtility::GetSaveManager()->OnReadyToSave.AddDynamic(this, &USavable::PushChanges);
	USVUtility::GetSaveManager()->OnGameSaved.AddDynamic(this, &USavable::OnSuccessfulSave);
	USVUtility::GetSaveManager()->OnGameLoaded.AddDynamic(this, &USavable::OnSuccessfulLoad);
}

void USavable::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	USVUtility::GetSaveManager()->OnReadyToSave.RemoveDynamic(this, &USavable::PushChanges);
	USVUtility::GetSaveManager()->OnGameSaved.RemoveDynamic(this, &USavable::OnSuccessfulSave);
	USVUtility::GetSaveManager()->OnGameLoaded.RemoveDynamic(this, &USavable::OnSuccessfulLoad);
}