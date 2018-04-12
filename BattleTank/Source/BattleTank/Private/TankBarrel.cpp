// Fill out your copyright notice in the Description page of Project Settings.

#include "TankBarrel.h"



void UTankBarrel::Elevate(float RelativeSpeed) {

	auto Time = GetWorld()->GetTimeSeconds(); // float
	//UE_LOG(LogTemp, Warning, TEXT("%f Barrel-Elevate: %f"), Time,RelativeSpeed);

	RelativeSpeed = FMath::Clamp<float>(RelativeSpeed,-1,1);

	//UE_LOG(LogTemp, Warning, TEXT("Barrel-Elevate: %f"), RelativeSpeed);
	//move the barrel the right amount this frame
	auto ElevationChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	auto RawNewElevation = RelativeRotation.Pitch + ElevationChange;
	//FMath::Clamp<float>(?,?,?); / FMath::Clamp(?,?,?);
	auto Elevation = FMath::Clamp<float>(RawNewElevation, MinElevationDegrees, MaxElevationDegrees);
	SetRelativeRotation(FRotator(Elevation,0,0));
	//UE_LOG(LogTemp, Warning, TEXT("%f Barrel-Elevate: %f"), Time, Elevation);

	//given a max elevation speed, and the frme time
}