// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API ULobbyWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UEditableTextBox* ETB_ChatInput;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UScrollBox* SB_ChatBox;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UTextBlock* T_ConnectCount;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UTextBlock* T_LeftTime;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UButton* B_StartGameButton;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnClickStartButton();

	UFUNCTION()
	void OnTextCommited(const FText& Text, ETextCommit::Type CommitMethod);

	void AddMessage(FText Message);

	void SetConnectCount(int NewConnectCount);

	void SetPublicMessage(int LeftTime);

	void ShowStartGameButton();
};
