// Copyright Denis Faraci, Inc. All Rights Reserved.


#include "Generic/SaveSystem/Components/Savables/Saver.h"
#include "Generic/SaveSystem/SlotsManager.h"
#include "Generic/SaveSystem/Utility/SVUtility.h"
#include "Kismet/GameplayStatics.h"


USaver::USaver()
{
	PrimaryComponentTick.bCanEverTick = false;
}

FName USaver::GetUniqueSaveID() const
{
	const FString SlotName = USlotsManager::GetSelectedSaveSlotName();
	const FString OwnerName = GetOwner()->GetName();
	const FString LevelName = UGameplayStatics::GetCurrentLevelName(this, true);
	const FString UniqueID = SlotName + "_" + OwnerName + "::" + LevelName;
	const int32 Hash = GetTypeHash(UniqueID);
	const FString Hex = FString::Printf(TEXT("%08X"), Hash);
	
	return FName(*Hex);
}

void USaver::PrepareSave(UDefaultSaveGame* SaveGameData)
{
	UE_LOG(LogTemp, Warning, TEXT("USaver::PrepareSave"));
	OnPrepSave.Broadcast(SaveGameData);
	OnPrepareSave(SaveGameData);
}

void USaver::PrepareLoad(UDefaultSaveGame* SaveGameData)
{
	OnPrepLoad.Broadcast(SaveGameData);
	OnPrepareLoad(SaveGameData);
}

void USaver::OnPrepareSave_Implementation(UDefaultSaveGame* SaveGameData)
{
}

void USaver::OnPrepareLoad_Implementation(UDefaultSaveGame* SaveGameData)
{
}

void USaver::OnSaveCompletedEvent_Implementation(const FString& SlotName, const int32 UserIndex, bool bSuccess, UDefaultSaveGame* SaveGameData)
{
	OnSaveGameCompleted.Broadcast(SlotName, UserIndex, bSuccess, SaveGameData);
}

void USaver::OnLoadCompletedEvent_Implementation(const FString& SlotName, const int32 UserIndex, UDefaultSaveGame* LoadedData)
{
	OnLoadGameCompleted.Broadcast(SlotName, UserIndex, LoadedData);
}

void USaver::BeginPlay()
{
	Super::BeginPlay();
	if (!USVUtility::GetSaveManager()) return;
	USVUtility::GetSaveManager()->OnPrepareSave.AddDynamic(this, &USaver::PrepareSave);
	USVUtility::GetSaveManager()->OnPrepareLoad.AddDynamic(this, &USaver::PrepareLoad);
	USVUtility::GetSaveManager()->OnSaveGame.AddDynamic(this, &USaver::OnSaveCompletedEvent);
	USVUtility::GetSaveManager()->OnLoadGame.AddDynamic(this, &USaver::OnLoadCompletedEvent);
}

void USaver::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (!USVUtility::GetSaveManager()) return;
	USVUtility::GetSaveManager()->OnPrepareSave.RemoveDynamic(this, &USaver::PrepareSave);
	USVUtility::GetSaveManager()->OnPrepareLoad.RemoveDynamic(this, &USaver::PrepareLoad);
	USVUtility::GetSaveManager()->OnSaveGame.RemoveDynamic(this, &USaver::OnSaveCompletedEvent);
	USVUtility::GetSaveManager()->OnLoadGame.RemoveDynamic(this, &USaver::OnLoadCompletedEvent);
}
