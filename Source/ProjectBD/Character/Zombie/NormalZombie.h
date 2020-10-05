// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NormalZombie.generated.h"

UENUM(BlueprintType)
enum class EZombieState : uint8
{
	Normal = 0	UMETA(Display = "Normal"),
	Battle = 1	UMETA(Display = "Battle"),
	Chase = 2	UMETA(Display = "Chase"),
	Dead = 3	UMETA(Display = "Dead"),
};

UCLASS()
class PROJECTBD_API ANormalZombie : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANormalZombie();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	EZombieState CurrentState;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	float CurrentHP;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	float MaxHP = 100.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	float WalkSpeed = 150.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	float RunSpeed = 300.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	float Attack = 30.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
	float AttackRange = 100.0f;
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UPawnSensingComponent* PawnSensing;

	UFUNCTION()
	void ProcessSeenPawn(APawn* Pawn);

	UFUNCTION()
	void ProcessHeardPawn(APawn* Pawn, const FVector& Location, float Volume);

	UFUNCTION(BlueprintCallable)
	void SetCurrentState(EZombieState NewState);

	UFUNCTION(BlueprintCallable)
	void SetSpeed(float Speed);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Data")
	class UBehaviorTree* ZombieBT;
};
