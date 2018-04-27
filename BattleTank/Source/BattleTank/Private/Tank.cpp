// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "TankAimingComponent.h"
#include "TankMovementComponent.h"
#include "TankBarrel.h"
#include "Projectile.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"



// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//no need to protect points as added at construction
	//create component inherited on bp
	TankAimingComponent = CreateDefaultSubobject<UTankAimingComponent>(FName("Aiming Component"));
	//TankMovementComponent = CreateDefaultSubobject<UTankMovementComponent>(FName("Moving Component"));

	//ConstructorHelpers::FObjectFinder<UTexture2D> HealthBarTextureObj(TEXT("Texture2D'/Game/UI/HealthBar.HealthBar'"));

	
	//UClass* blueprintClass = FindObject<AProjectile>(ANY_PACKAGE, TEXT("/Game/Tank/Projectile_BP.Projectile_BP_C"));
	/*
	auto ProjectileBP = FindObject<AProjectile>(ANY_PACKAGE, TEXT("/Game/Tank/Projectile_BP.Projectile_BP_C"));
	ProjectileBlueprint = ProjectileBP;
	
	AProjectile* DefaultPawn = DefaultPawnClass->GetDefaultObject<APawn>();
	*/

	
	static ConstructorHelpers::FClassFinder<AProjectile> ProjectileBP(TEXT("/Game/Tank/Projectile_BP"));
	ProjectileBlueprint = ProjectileBP.Class;
	

}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle TimerHandle;
	//GetWorldTimerManager().SetTimer(TimerHandle, &ATank::CheckSpawnActor, 5.f, true);
	//GetWorldTimerManager().SetTimer(this, *ATank::DestroyActor, 5.0f, false);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATank::DestroyActor, 5.0f, true);
}



// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATank::AimAt(FVector HitLocation)
{
	TankAimingComponent->AimAt(HitLocation, LaunchSpeed);
	//auto OurTankName = GetName();
	//UE_LOG(LogTemp, Warning, TEXT("%s aiming at %s"), *OurTankName ,*HitLocation.ToString());
}

void ATank::SetBarrelReference(UTankBarrel* BarrelToSet) {
	TankAimingComponent->SetBarrelReference(BarrelToSet);
	Barrel = BarrelToSet;
}
void ATank::SetTurretReference(UTankTurret* TurretToSet) {
	TankAimingComponent->SetTurretReference(TurretToSet);
}

/*
void ATank::IntendMoveForward(float Throw) {
	TankMovementComponent->IntendMoveForward(Throw);
}
*/



void ATank::Fire() {

	bool isReloaded = ( GetWorld()->GetTimeSeconds() - LastFireTime) > ReloadTimeInSecond;

	//if (!Barrel) { return; }
	if (Barrel && isReloaded) {

		//spawn a projectile at the socket location
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			Barrel->GetSocketLocation(FName("Projectile") ),
			Barrel->GetSocketRotation(FName("Projectile"))
		);
		ProjectileActors.Add(Projectile);
		/*
		FTimerHandle TimerHandle;
		//Calling MyUsefulFunction after 1 seconds with looping
		GetWorldTimerManager().SetTimer(TimerHandle, &ATank::TT, 1.f,true);
		*/

		Projectile->LaunchProjectile(LaunchSpeed);
		LastFireTime = GetWorld()->GetTimeSeconds();
	}

}
void ATank::CheckSpawnActor()
{

	//UE_LOG(LogTemp, Warning, TEXT("name:"));
}
void ATank::DestroyActor()
{
	int32 ProjectileOnScene = ProjectileActors.Num();
	if (ProjectileOnScene <= 0) return;
	ProjectileActors2 = {};
	for (int32 b = 0; b < ProjectileOnScene; b++)
	{
		AProjectile* t = ProjectileActors[b];
		if (!t) continue;
		if (!t->IsValidLowLevel()) continue;
		if (t->GetActorLocation().Z < -100.f ) {
			t->Destroy();
		} else {
			ProjectileActors2.Add(t);
		}
		//UE_LOG(LogTemp, Warning, TEXT("name: %s"), *ProjectileActors[b]->GetName());

		//UE_LOG(LogTemp, Warning, TEXT("name:") );
	};
	ProjectileActors = ProjectileActors2;
	//UE_LOG(LogTemp, Warning, TEXT("Array: %i"), ProjectileActors.Num());
}

void ATank::IsAI()
{
	hasAI = true;
}
bool ATank::GetAI()
{
	return hasAI;
}