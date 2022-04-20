// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SIP_InteriorMappingHUD.generated.h"

UCLASS()
class ASIP_InteriorMappingHUD : public AHUD
{
	GENERATED_BODY()

public:
	ASIP_InteriorMappingHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

