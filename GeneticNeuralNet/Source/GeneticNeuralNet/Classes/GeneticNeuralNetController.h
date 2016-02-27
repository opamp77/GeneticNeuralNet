// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GeneticAlg.h"
#include "nnCreature.h"
#include "GeneticNeuralNetController.generated.h"


UCLASS()
class AGeneticNeuralNetController : public AActor
{
	GENERATED_BODY()
	
	GeneticAlg * Ga;
	TArray<AnnCreature*> CreatureArray;
	int32 NumWeightsPerNN;
	TArray<FGenome> ThePopulation;
	
	
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NeuralNet_Settings")
		bool bSimulate;
	//Stats
	UPROPERTY(BlueprintReadWrite)
	int32 TickCounter;
	UPROPERTY(BlueprintReadWrite)
	int32 GenerationCount;
	UPROPERTY(BlueprintReadOnly)
		float MaxAvgFitness;
	UPROPERTY(BlueprintReadOnly)
		float MaxBestFitness;
	UPROPERTY(BlueprintReadOnly)
		FDebugFloatHistory AvgFitnessHistory;
	UPROPERTY(BlueprintReadOnly)
		FDebugFloatHistory BestFitnessHistory;

	//Neural Network Settings
	UPROPERTY(EditAnywhere, Category = "NeuralNet_Settings", meta=(UIMin=0, UIMax=50000))
		int32 NumInputs;
	UPROPERTY(EditAnywhere, Category = "NeuralNet_Settings", meta=(UIMin=0, UIMax=1024))
		int32 NumHiddenLayers;
	UPROPERTY(EditAnywhere, Category = NeuralNet_Settings, meta=(UIMin=1, UIMax=50000))
		int32 NeuronsPerHiddenLayer;
	UPROPERTY(EditAnywhere, Category = NeuralNet_Settings, meta=(UIMin=1, UIMax=50000))
		int32 NumOutputs;
	UPROPERTY(EditAnywhere, Category = NeuralNet_Settings)
		ENeuralNetActivationType ActivationFunction;
	UPROPERTY(EditAnywhere, Category = NeuralNet_Settings, meta=(UIMin=-1.0, UIMax=1.0))
		float ActivationResponse; //Step response 0 to sigmoid response 1
	UPROPERTY(EditAnywhere, Category = NeuralNet_Settings, meta=(UIMin=-1.0, UIMax=1.0))
		float Bias; //bias(Fixed input for each layer )

	//GeneticAlg Parameters
	UPROPERTY(EditAnyWhere,BlueprintReadOnly, Category = Genetics)
		TSubclassOf<AnnCreature> CreatureClass;
	UPROPERTY(EditAnywhere, Category = Genetics, meta = (UIMin = 2, UIMax = 99999999))
		int32 PopulationSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Genetics, meta = (UIMin = 1, UIMax = 99999999))
		int32 TicksPerGen;
	UPROPERTY(EditAnywhere, Category = Genetics)
		EGeneticCrossoverMode CrossoverMode;
	UPROPERTY(EditAnywhere, Category = Genetics, meta = (UIMin=0.0, UIMax=1.0))
		float CrossoverRate;
	UPROPERTY(EditAnywhere, Category = Genetics)
		EGeneticSelectionType SelectionType;
	UPROPERTY(EditAnywhere, Category = Genetics, meta = (UIMin=0.0, UIMax=1.0))
		float MutationRate;
	UPROPERTY(EditAnywhere, Category = Genetics, meta = (UIMin=0.0, UIMax=1.0))
		float MaxPerturbation; //the max amount a weight may be mutated by
	UPROPERTY(EditAnywhere, Category = Genetics, meta = (UIMin=0, UIMax=1024))
		int32 NumCopiesElite; 
	UPROPERTY(EditAnywhere, Category = Genetics, meta = (UIMin=0, UIMax=1024))
		int32 NumElite;

	UFUNCTION(BlueprintCallable, Category="NeuralNet")
		void InitCreatures();

//	UFUNCTION(BlueprintCallable, Category = "NeuralNet") //todo
//		void SaveState();
//	UFUNCTION(BlueprintCallable, Category = "NeuralNet") //todo
//		void LoadState();

	// Sets default values for this actor's properties
	AGeneticNeuralNetController();
	~AGeneticNeuralNetController();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
