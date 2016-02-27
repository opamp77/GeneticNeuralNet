// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "NeuralNet.h"
#include "nnCreature.generated.h"


UCLASS()
class AnnCreature : public AActor
{
	GENERATED_BODY()

	float Fitness;
	UNeuralNet *Brain;
	FVector startloc;

public:
	
	
	// Sets default values for this actor's properties
	AnnCreature();
	~AnnCreature();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(BlueprintReadWrite)
		TArray<float> InputArray;
	UPROPERTY(BlueprintReadOnly)
		TArray<float> OutputArray;


	UFUNCTION(BlueprintCallable, Category = "NeuralNetCreature")
		void SetFitness(float f);
	UFUNCTION(BlueprintCallable, Category = "NeuralNetCreature")
		float GetFitness();
	UFUNCTION(BlueprintCallable, Category = "NeuralNetCreature")
		void Think();
	UFUNCTION(BlueprintNativeEvent, Category = "NeuralNetCreature")
		void Reset();
	virtual void Reset_Implementation();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "NeuralNetCreature")
		void Simulate();
	virtual void Simulate_Implementation();


	void SetWeights(TArray<float> &Weights);
	int32 GetNumberOfWeights()const;

	


	

	
	
};
