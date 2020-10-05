// Fill out your copyright notice in the Description page of Project Settings.


#include "NormalZombie.h"
#include "ZombieAIController.h"
#include "Components/CapsuleComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ANormalZombie::ANormalZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.f, 0));

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
}

// Called when the game starts or when spawned
void ANormalZombie::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHP = MaxHP;

	SetSpeed(WalkSpeed);
	SetCurrentState(EZombieState::Normal);

	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &ANormalZombie::ProcessSeenPawn);
		PawnSensing->OnHearNoise.AddDynamic(this, &ANormalZombie::ProcessHeardPawn);
	}
}

// Called every frame
void ANormalZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANormalZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ANormalZombie::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (CurrentHP <= 0)
	{
		return 0.0f;
	}

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID)) //PointDamage 贸府
	{
		//促屈己
		FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)(&DamageEvent);

		UE_LOG(LogClass, Warning, TEXT("Point Damage %s"), *(PointDamageEvent->HitInfo.BoneName.ToString()));

		if (PointDamageEvent->HitInfo.BoneName.Compare(TEXT("head")) == 0)
		{
			CurrentHP = 0;
		}
		else
		{
			CurrentHP -= DamageAmount;
		}
	}
	else if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID)) //RadialDamage 贸府
	{
		FRadialDamageEvent* RadialDamageEvent = (FRadialDamageEvent*)(&DamageEvent);

		UE_LOG(LogClass, Warning, TEXT("Radial Damage %f"), DamageAmount);
	}
	else //葛电 单固瘤 贸府
	{
		CurrentHP -= DamageAmount;
	}

	if (CurrentHP <= 0)
	{
		SetCurrentState(EZombieState::Dead);
	}


	UE_LOG(LogClass, Warning, TEXT("%f"), DamageAmount);

	return 0.0f;
}

void ANormalZombie::ProcessSeenPawn(APawn* Pawn)
{
	if (CurrentState == EZombieState::Normal)
	{
		UE_LOG(LogClass, Warning, TEXT("See %s"), *Pawn->GetName());
		SetCurrentState(EZombieState::Chase);
		//Set Blackboard Value
		AZombieAIController* AIC = GetController<AZombieAIController>();
		if (AIC)
		{
			AIC->SetPlayer(Pawn);
		}
	}
}

void ANormalZombie::ProcessHeardPawn(APawn* Pawn, const FVector& Location, float Volume)
{
	UE_LOG(LogClass, Warning, TEXT("Heard %s"), *Pawn->GetName());

}

void ANormalZombie::SetCurrentState(EZombieState NewState)
{
	CurrentState = NewState;

	//Set Blackboard Value
	AZombieAIController* AIC = GetController<AZombieAIController>();
	if (AIC)
	{
		AIC->SetCurrentState(NewState);
	}
}

void ANormalZombie::SetSpeed(float NewSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}

