// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Tank.h"
#include "BattleTank.h"


void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	auto PlayerTank = GetPlayerTank();
	if (!PlayerTank) {
		UE_LOG(LogTemp, Warning, TEXT("AIController can't find player tank"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("AIController found player: %s"), *(PlayerTank->GetName()));
	}
	//UE_LOG(LogTemp, Warning, TEXT("AIController Begin Play"));


}

ATank* ATankAIController::GetControlledTank() const
{

	return Cast<ATank>(GetPawn());
}

ATank* ATankAIController::GetPlayerTank() const
{
	auto PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!PlayerPawn) {
		return nullptr;
	}
	else {
		return Cast<ATank>(PlayerPawn);
	}
}

void ATankAIController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	
	auto PlayerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
	auto AITank = Cast<ATank>(GetPawn());
	if (PlayerTank) {
		
		MoveToActor(PlayerTank, AcceptanceRadius);
		AITank->IsAI();
		AITank->AimAt(PlayerTank->GetActorLocation());
		AITank->Fire();
	}
	
	/*
	auto PlayerTank = GetPlayerTank();
	FVector playerAt = PlayerTank->GetActorLocation();
	GetControlledTank()->AimAt(playerAt);
	GetControlledTank()->Fire();
	UE_LOG(LogTemp, Warning, TEXT("%s"), *(PlayerTank->GetActorLocation().ToString()) );
	*/
};