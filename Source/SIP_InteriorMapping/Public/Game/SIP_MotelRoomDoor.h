// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SIP_MotelRoomDoor.generated.h"

class USceneCaptureComponentCube;

UCLASS()
class SIP_INTERIORMAPPING_API ASIP_MotelRoomDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASIP_MotelRoomDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room info")
	TSubclassOf<USceneCaptureComponentCube> CapCube;

};
