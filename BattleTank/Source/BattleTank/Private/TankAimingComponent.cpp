// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "TankBarrel.h"
#include "TankTurret.h"


// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTankAimingComponent::AimAt(FVector HitLocation, float LaunchSpeed)
{
	if (!Barrel) {
		return;
	};

	auto OurTankName = GetOwner()->GetName();
	auto BarrelLocation = Barrel->GetComponentLocation().ToString();
	//UE_LOG(LogTemp, Warning, TEXT("%s aiming at %s from %s"), *OurTankName, *HitLocation.ToString(), *BarrelLocation );

	FVector OutLaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));

	/*
	TArray<AActor*> ActorIgnoreList;
	ActorIgnoreList.Add(GetOwner());
	*/

	//calculate the OutLaunchVelocity
	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity(
		this,
		OutLaunchVelocity,
		StartLocation,
		HitLocation,
		LaunchSpeed,
		false,
		0,
		0
		,ESuggestProjVelocityTraceOption::DoNotTrace
		//,FCollisionResponseParams::DefaultResponseParam,
		//,ActorIgnoreList
	);

	auto TankName = GetOwner()->GetName();
	bool isAI = TankName.Contains(TEXT("AI"), ESearchCase::CaseSensitive, ESearchDir::FromEnd);
	
	if (bHaveAimSolution) {
		auto AimDirection = OutLaunchVelocity.GetSafeNormal();


		if (!isAI) {
			//UE_LOG(LogTemp, Warning, TEXT("aiming at %s"),  *HitLocation.ToString());
			
			//draw line on the aiming point
			FVector linePos = FVector(HitLocation.X, HitLocation.Y, HitLocation.Z + 1000.f);
			UKismetSystemLibrary::DrawDebugLine(
				GetWorld(),
				HitLocation,
				linePos,
				FColor(255, 0, 0),
				0.f, 50.f
			);
			

			//UE_LOG(LogTemp, Warning, TEXT("%s aiming at %s"), *TankName, *AimDirection.ToString());
			
		}
		else {
			//UE_LOG(LogTemp, Warning, TEXT("B:%s || S: %s"), *BarrelLocation, *StartLocation.ToString());
		}
		
		MoveBarrelTowards(AimDirection);
		//.Contains(TEXT("AI"), ESearchCase::CaseSensitive, ESearchDir::FromEnd);


		

	} else {
		//FVector pos = GetOwner()->GetActorLocation();
		//UE_LOG(LogTemp, Warning, TEXT("%f"), pos.Z );
		auto Time = GetWorld()->GetTimeSeconds(); // float
		//UE_LOG(LogTemp, Warning, TEXT("%f target out of range."), Time);
	}

}

void UTankAimingComponent::SetBarrelReference(UTankBarrel* BarrelToSet) {
	Barrel = BarrelToSet;
}
void UTankAimingComponent::SetTurretReference(UTankTurret* TurretToSet) {
	Turret = TurretToSet;
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection) {
	
	//work-out difference current barrel rotation, and AimDirection
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;
	//UE_LOG(LogTemp, Warning, TEXT("AimAsRotator: %s"), *DeltaRotator.ToString() );

	//1 roll barrel up ,  -1 roll barrel down
	Barrel->Elevate(DeltaRotator.Pitch);
	Turret->Rotate(DeltaRotator.Yaw);
}