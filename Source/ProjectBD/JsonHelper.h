// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "JsonHelper.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API UJsonHelper : public UObject
{
	GENERATED_BODY()
	
public:
	void SaveToFile(FString Json, FString Path); // File�� ���� JSON ����.
	void SaveToFile_Append(FString Json, FString Path); // File �ۼ� ���� �ڷ� Json�� �߰�. 
	FString LoadFromFile(FString Path); // File���� JSON ĳ��

	void StartMake(TSharedPtr<class FJsonObject>& JsonStr); // ���ο� JSON ����.
	void AddStringField(TSharedPtr<class FJsonObject>& JsonStr, FString Key, FString Value); // String Field �߰�.
	void AddArrayField(TSharedPtr<class FJsonObject>& JsonStr, FString Key, TArray<class TSharedPtr<class FJsonValue>> Value); // Array Field �߰�.
	FString EndMake(TSharedPtr<class FJsonObject>& JsonStr); // ������ JSON ��ȯ.

	FString GetStringField(FString Json, FString Key); // �ش� Json����, Key�� �ش��ϴ� String �� ��ȯ
	TArray<class TSharedPtr<class FJsonValue>> GetArrayField(FString Json, FString Key); // �ش� Json����, Key�� �ش��ϴ� Array �� ��ȯ
};
