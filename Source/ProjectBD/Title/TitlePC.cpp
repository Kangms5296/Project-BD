// Fill out your copyright notice in the Description page of Project Settings.


#include "TitlePC.h"
#include "Blueprint/UserWidget.h"
#include "TitleWidgetBase.h"
#include "Kismet/GameplayStatics.h"

void ATitlePC::BeginPlay()
{
	if (TitleWidgetClass)
	{
		TitleWidgetObject = CreateWidget<UTitleWidgetBase>(this, TitleWidgetClass);
		TitleWidgetObject->AddToViewport();

		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
	}
}

void ATitlePC::StartServer()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Lobby"), true, TEXT("listen"));
}

void ATitlePC::ConnectServer(FString ServerIPAddress)
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(*ServerIPAddress));
}

