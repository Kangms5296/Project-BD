// Fill out your copyright notice in the Description page of Project Settings.


#include "BDGameInstance.h"

FString UBDGameInstance::GetUserID()
{
	if (UserID.IsEmpty())
	{
		FString Temp = FString::Printf(TEXT("No_%d"), FMath::RandRange(1, 100));
		UserID = Temp;
	}

	return UserID;
}

void UBDGameInstance::SetUserID(FString NewUserID)
{
	UserID = NewUserID;
}
