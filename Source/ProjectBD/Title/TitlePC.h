// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TitlePC.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API ATitlePC : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UI")
	TSubclassOf<class UTitleWidgetBase> TitleWidgetClass;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "UI")
	class UTitleWidgetBase* TitleWidgetObject;

	UFUNCTION(BlueprintCallable)
	void StartServer();

	UFUNCTION(BlueprintCallable)
	void ConnectServer(FString ServerIP);
};
