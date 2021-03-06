// Copyright (c) 2017 Bytecode Bits

#pragma once

#include "GameFramework/Pawn.h"
#include "ZynapsPlayerState.h"
#include "ZynapsGameState.h"
#include "PlayerProjectile.h"
#include "Fly2DMovementComponent.h"
#include "FuelCapsule.h"
#include "PlayerPawn.generated.h"

// Log category
DECLARE_LOG_CATEGORY_EXTERN(LogPlayerPawn, Log, All);

// Constants to identify the three cannons of the ship
const uint8 RightCannon = 0;
const uint8 LeftCannon = 1;
const uint8 TopCannon = 2;

// Constant which defines the speed of the highlight glow while in power-up activation mode
const float HighlightGlowSpeed = 1.0f;

/**
 * The default Pawn class used by StageGameMode while playing the game.
 */
UCLASS()
class ZYNAPSRELOADED_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

	// Called after component initialization
	virtual void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to move the player up
	UFUNCTION(BlueprintCallable, Category = ZynapsActions)
	void MoveUp(float Val);

	// Called to move the player down
	UFUNCTION(BlueprintCallable, Category = ZynapsActions)
	void MoveDown(float Val);

	// Called to move the player to the left
	UFUNCTION(BlueprintCallable, Category = ZynapsActions)
	void MoveLeft(float Val);

	// Called to move the player to the right
	UFUNCTION(BlueprintCallable, Category = ZynapsActions)
	void MoveRight(float Val);

	// Called to fire
	UFUNCTION(BlueprintCallable, Category = ZynapsActions)
	void Fire();

	// Called when a player hits other actor or an obstacle
	UFUNCTION(BlueprintNativeEvent, Category = ZynapsEvents)
	void Hit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult &HitResult);

	// Called when the player begins overlapping with another actor
	UFUNCTION(BlueprintNativeEvent, Category = ZynapsEvents)
	void BeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor, 
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, 
		const FHitResult &SweepResult);

	// Called when the player begins overlapping with another actor
	UFUNCTION(BlueprintNativeEvent, Category = ZynapsEvents)
	void EndOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Collision capsule
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Components)
	UCapsuleComponent* CapsuleComponent;

	// Mesh
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Components)
	UStaticMeshComponent* MeshComponent;

	// Engine particle system
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Components)
	UParticleSystemComponent* EnginePartSystemComponent;

	// Movement component
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Components)
	UFly2DMovementComponent* MovementComponent;

	// The type of projectile spawned when firing
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Actor)
	TSubclassOf<class APlayerProjectile> ProjectileClass;

	// The explosion particle system spawned when the ship is hit
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Actor)
	UParticleSystem* ExplosionPartSystem;

	// The type of camera shake used when the ship is destroyed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Actor)
	TSubclassOf<UCameraShake> CameraShakeClass;

	// The name of the socket for the left cannon
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Actor)
	FName LeftCannonSocketName;

	// The name of the socket for the right cannon
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Actor)
	FName RightCannonSocketName;

	// The name of the socket for the top cannon
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Actor)
	FName TopCannonSocketName;

	// The name of the socket where the engine thrust particle system will be attached
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Actor)
	FName EngineThrustSocketName;

	// The sound to be played when the player fires a cannon
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Actor)
	USoundBase* FireSound;

	// The sound to be played when the player is destroyed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Actor)
	USoundBase* ExplosionSound;

	// The sound to be played when the selected power-up is shifted
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Actor)
	USoundBase* ShiftPowerUpSound;

	// The sound to be played when the selected power-up is activated
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Actor)
	USoundBase* ActivatePowerUpSound;

protected:

	// Returns the transform of a socket
	UFUNCTION(BlueprintPure, meta = (BlueprintProtected), Category = ZynapsState)
	FTransform GetSocketTransform(FName SocketName) const;

	// Called when a fuel capsule is collected
	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected), Category = ZynapsState)
	void FuelCapsuleCollected(AZynapsPlayerState* ZynapsPlayerState, AFuelCapsule* FuelCapsule);

	// Called when the player pawn is destroyed
	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected), Category = ZynapsState)
	void PlayerPawnDestroyed(AZynapsPlayerState* ZynapsPlayerState);

private: 

	// Creates the capsule component used for collision detection
	UCapsuleComponent* CreateCapsuleComponent(USceneComponent* Parent);

	// Creates the mesh component which models the ship
	UStaticMeshComponent* CreateMeshComponent(USceneComponent* Parent);

	// Creates the particle system for the engine thrust
	UParticleSystemComponent* CreateEngineThrustParticleSystem(USceneComponent* Parent, FName SocketName);

	// Creates the component which manages the movement of the ship
	UFly2DMovementComponent* CreateMovementComponent();

	// Creates the explosion particle system
	UParticleSystem* CreateExplosionParticleSystem();

	// Returns a reference to the instance of AZynapsPlayerState or NULL if it doesn't exist
	AZynapsPlayerState* GetZynapsPlayerState() const;

	// Returns the game state
	AZynapsGameState* GetZynapsGameState() const;

	// The next cannon to be shot
	uint8 NextCannon;

	// The dynamic material instance used to change the color of the ship during the power-up
	// activation mode
	UMaterialInstanceDynamic* DynMaterial;

	// Highlight direction
	float HighlightDirection = 1.0f;
};
