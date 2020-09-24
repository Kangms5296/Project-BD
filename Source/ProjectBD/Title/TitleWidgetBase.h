// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API UTitleWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Control")
	class UEditableTextBox* T_UserID;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Control")
	class UEditableTextBox* T_Password;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Control")
	class UEditableTextBox* T_ServerIP;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Control")
	class UButton* B_Connect;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Control")
	class UButton* B_Start;

	UFUNCTION()
	void OnConnectButton();

	UFUNCTION()
	void OnStartButton();

	void SaveUserID();
};
