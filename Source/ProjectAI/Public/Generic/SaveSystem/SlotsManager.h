// Copyright Denis Faraci, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SaveManager.h"
#include "Data/Structs/SaveSlotData.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SlotsManager.generated.h"


UCLASS()
class PROJECTAI_API USlotsManager : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	static FString SelectedSaveSlotName;
	static USaveManager* CurrentSaveManager;

public:
	static void Init(USaveManager* SaveManager);

	/**
	 * Selects a slot by name.
	 * @param SlotName The name of the slot to select.
	 * @return True if the slot was selected, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	static bool SelectSaveSlot(const FString& SlotName);

	/**
	 * Gets the name of the selected slot.
	 * @return The name of the selected slot.
	 */
	UFUNCTION(BlueprintPure, Category = "Save System")
	static FString GetSelectedSaveSlotName();

	/**
	 * Deletes the slot file with the given name.
	 * @param SlotName The name of the slot to delete.
	 * @return True if the slot file was deleted, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	static bool DeleteSlotFile(const FString& SlotName);

	/**
	 * Deletes all the slot files.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	static void DeleteAllSlotFiles();

	/**
	 * Checks if a slot file with the given name exists.
	 * @param SlotName The name of the slot to check.
	 * @return True if the slot file exists, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool DoesSlotFileExist(const FString& SlotName);

	/**
	 * Checks if any slot file exists.
	 * @return True if any slot file exists, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool DoesAnySlotFileExist();
	
	UFUNCTION(BlueprintPure, Category = "Save System")
	static TArray<FSaveSlotData> GetAllSlotsData();

	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool GetSlotFilesNames(TArray<FString>& OutSaveFiles, bool bHasExtension = true);

	/**
	 * Gets the number of save slots.
	 * @return The number of save slots.
	 */
	UFUNCTION(BlueprintPure, Category = "Save System")
	static int32 GetNumberOfSlots(int32& OutAutoSavesSlotsNumber, int32& OutManualSaveSlotsNumber);

	UFUNCTION(BlueprintCallable, Category = "Save System")
	static void SelectMostRecentSaveSlot();

	UFUNCTION(BlueprintCallable, Category = "Save System")
	static void SelectMostAncientSaveSlot();
	
	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool GetMostRecentSlotData(FSaveSlotData& OutSlotData);

	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool GetMostAncientSlotData(FSaveSlotData& OutSlotData, bool bIsAutoSave = false);

	/**
 	* Creates a slot file with the given name.
 	* @param SlotName The name of the slot to create.
 	* @param bSelectCreatedSlot If true, the created slot will be selected.
 	* @return True if the slot file was created, false otherwise.
 	*/
	static bool CreateSlotFileOfName(const FString& SlotName, bool bSelectCreatedSlot = true);

	/**
	 * Creates the next slot file and selects it.
	 * @return True if the slot file was created, false otherwise.
	 */
	static bool CreateSlotFile();

	/**
 	* Checks if the selected slot is valid.
 	* @return True if the selected slot is valid, false otherwise.
 	*/
	static bool IsSelectedSlotValid();

	/**
	 * Checks if the given slot name is valid.
	 * @param SlotName The name of the slot to check.
	 * @return True if the slot name is valid, false otherwise.
	 */
	static bool IsSlotNameValid(const FString& SlotName);
	
	static bool CreateAutoSaveSlotFile(FString& SlotName);
	static void SaveSlotData(UDefaultSaveGame* SaveGame);
};
