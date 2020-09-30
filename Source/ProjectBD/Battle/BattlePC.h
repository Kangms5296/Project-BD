// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BattlePC.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API ABattlePC : public APlayerController
{
	GENERATED_BODY()
	
public:
	ABattlePC();

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UI")
	TSubclassOf<class UBattleWidgetBase> BattleWidgetClass;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "UI")
	class UBattleWidgetBase* BattleWidgetObject;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UI")
	TSubclassOf<class UItemTooltipBase> ItemTooltipClass;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "UI")
	class UItemTooltipBase* ItemTooltipObject;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UI")
	TSubclassOf<class UInventoryWidgetBase> InventoryWidgetClass;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "UI")
	class UInventoryWidgetBase* InventoryWidgetObject;

	bool IsShowInventory;
	void ToggleInventory();

	void ShowItemTooltip(FString ItemName);
	void HideItemTooltip();
};
