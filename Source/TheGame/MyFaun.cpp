// Fill out your copyright notice in the Description page of Project Settings.

#include "TheGame.h"
#include "MyFaun.h"
#include "Enemy.h"
#include "AAttack.h"


AMyFaun::AMyFaun()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this pawn to be controlled by the lowest-numbered player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Create a dummy root component we can attach things to.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	// Create a camera and a visible object
	//OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));
	OurVisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent"));
	// Attach our camera and visible object to our root component. Offset and rotate the camera.
	//OurCamera->SetupAttachment(RootComponent);
	//OurCamera->SetRelativeLocation(FVector(-200.0f, 0.0f, 250.0f));
	//OurCamera->SetRelativeRotation(FRotator(CameraRotation, 0.0f, 0.0f));
	OurVisibleComponent->SetupAttachment(RootComponent);
	//TopDownCamera->SetupAttachment(RootComponent);

	//CollisionBox = this->FindComponentByClass<UBoxComponent>();
	//DummySceneComponent = NewObject<USceneComponent>();//USceneComponent();
}

// Called when the game starts or when spawned
void AMyFaun::BeginPlay()
{
	Super::BeginPlay();

	
	InputComponent->BindAction("Shoot", IE_Pressed, this, &AMyFaun::Shoot);
	InputComponent->BindAction("Shoot", IE_Repeat, this, &AMyFaun::ShootMore);
	InputComponent->BindAction("Restart", IE_Pressed, this, &AMyFaun::Restart).bExecuteWhenPaused = true;

	// Respond every frame to the values of our two movement axes, "MoveX" and "MoveY".
	InputComponent->BindAxis("MoveX", this, &AMyFaun::Move_XAxis);
	InputComponent->BindAxis("MoveY", this, &AMyFaun::Move_YAxis);
	InputComponent->BindAxis("RotateZ", this, &AMyFaun::RotateZ);

	//OurCamera->SetRelativeLocation(FVector(-250.0f, 0.0f, 250.0f));
	//OurCamera->SetRelativeRotation(FRotator(CameraRotation, 0.0f, 0.0f));

	CollisionBox = this->FindComponentByClass<UBoxComponent>();
	//CollisionBox->bGenerateOverlapEvents = true;

	if (CollisionBox)
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AMyFaun::OnOverlap);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CollisionBox not found!"));
	}

	//TopDownCamera->SetupAttachment(RootComponent);
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	//PlayerController->PlayerCameraManager->
	PlayerController->SetViewTarget(TopDownCamera);

	//Mouse setup
	PlayerController->bShowMouseCursor = true;
	PlayerController->bEnableClickEvents = true;
	PlayerController->bEnableMouseOverEvents = true;
	//PlayerController->SetAudioListenerOverride(DummySceneComponent, FVector(0.f), FRotator(0.f));

}

// Called every frame
void AMyFaun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

	// Handle movement based on our "MoveX" and "MoveY" axes
	Movement(DeltaTime);
}

// Called to bind functionality to input
void AMyFaun::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void AMyFaun::Movement(float DeltaTime)
{
	if (!CurrentVelocity.IsZero())
	{
		TimeAccellerating += DeltaTime;
		if (TimeAccellerating >= TimeBeforeAccelerate)
		{
			SpeedScale += 0.05;
			SpeedScale > 5.0f ? SpeedScale = 4.0f : SpeedScale;   //SpeedScale aldri over 5.0
																  //SpeedScale = FMath::Clamp(SpeedScale, 1.f, 5.f);      //Dette gjør ikke det det skal...
		}
		FVector NewLocation = GetActorLocation() + (CurrentVelocity * SpeedScale * DeltaTime);
		SetActorLocation(NewLocation);
	}
	else
	{
		SpeedScale = 2.5f;
		TimeAccellerating = 0.f;
	}
	FRotator TempRotation = GetActorRotation();
	TempRotation.Yaw += CurrentRotation;
	SetActorRotation(TempRotation);

}
void AMyFaun::Move_XAxis(float AxisValue)
{
	// Move at 100 units per second forward or backward
	CurrentVelocity.X = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 100.0f;
}

void AMyFaun::Move_YAxis(float AxisValue)
{
	// Move at 100 units per second right or left
	CurrentVelocity.Y = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 100.0f;
}

void AMyFaun::RotateZ(float AxisValue)
{
	CurrentRotation = FMath::Clamp(AxisValue, -1.0f, 1.0f);
}


void AMyFaun::Shoot()
{
	if (Ammo > 0)
	{
		Ammo--;
		UWorld* World = GetWorld();	//Henter peker til spillverdenen
		if (World)			//tester at verdenen finnes
		{
			FVector Location = GetActorLocation();   //Henter plasseringen til PlayerPawn
													 //Kan settes til annen plassering for det du vil spawne
			World->SpawnActor<ABullet>(BulletBlueprint, Location + FVector(0.4f, 0.f, 0.f), GetActorRotation());

			//Play Sound
			UGameplayStatics::PlaySound2D(World, ShootSound, 1.f, 1.f, 0.f);
			//        UGameplayStatics::PlaySoundAtLocation(World, ShootSound, GetActorLocation());
		}
	}
}

void AMyFaun::ShootMore()
{
	UE_LOG(LogTemp, Warning, TEXT("Button IE_Repeat"));
}

void AMyFaun::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComponent,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Player Overlap"))
		if (OtherActor->IsA(AEnemy::StaticClass()))
		{
			Died = true;
			this->SetActorHiddenInGame(true);
			UGameplayStatics::SetGamePaused(GetWorld(), true);
		}
}

void AMyFaun::Restart()
{
	if (Died)
	{
		UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
	}