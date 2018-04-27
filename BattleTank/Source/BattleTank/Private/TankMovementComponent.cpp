// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent.h"
#include "TankTrack.h"

UTankMovementComponent::UTankMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UTankMovementComponent::Initialise(UTankTrack* LeftTrackToSet, UTankTrack* RightTrackToSet) {
	
	LeftTrack = LeftTrackToSet;
	RightTrack = RightTrackToSet;
}

void UTankMovementComponent::IntendMoveForward(float Throw) {
	if (!LeftTrack || !RightTrack) {
		return;
	}

	//auto Name = GetName();
	//UE_LOG(LogTemp, Warning, TEXT("Throw: %f"), Throw);

	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(Throw);
}

void UTankMovementComponent::IntendTurnRight(float Throw) {			
	if (!LeftTrack || !RightTrack) {								
		return;														
	}																
	LeftTrack->SetThrottle(Throw);									
	RightTrack->SetThrottle(-Throw);								
}

void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) {
	//no need to call super as we're replacing  the funcitionality
	auto TankName = GetOwner()->GetName();
	auto TankForward = GetOwner()->GetActorForwardVector().GetSafeNormal();
	auto AIForwardIntention = MoveVelocity.GetSafeNormal();

	float Throw = FVector::DotProduct(TankForward, AIForwardIntention);
	FVector Turn = FVector::CrossProduct(TankForward, AIForwardIntention);
	//IntendMoveForward(Throw);
	IntendTurnRight(Turn.Z);
	//UE_LOG(LogTemp, Warning, TEXT("name: %s, move: %s"), *TankName, *AIForwardIntention.ToString());
};