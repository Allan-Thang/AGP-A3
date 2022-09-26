// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "AdvGamesProgramming/DungeonGenerator.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeDungeonGenerator() {}
// Cross Module References
	ADVGAMESPROGRAMMING_API UClass* Z_Construct_UClass_ADungeonGenerator_NoRegister();
	ADVGAMESPROGRAMMING_API UClass* Z_Construct_UClass_ADungeonGenerator();
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	UPackage* Z_Construct_UPackage__Script_AdvGamesProgramming();
// End Cross Module References
	void ADungeonGenerator::StaticRegisterNativesADungeonGenerator()
	{
	}
	UClass* Z_Construct_UClass_ADungeonGenerator_NoRegister()
	{
		return ADungeonGenerator::StaticClass();
	}
	struct Z_Construct_UClass_ADungeonGenerator_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ADungeonGenerator_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AActor,
		(UObject* (*)())Z_Construct_UPackage__Script_AdvGamesProgramming,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ADungeonGenerator_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "DungeonGenerator.h" },
		{ "ModuleRelativePath", "DungeonGenerator.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_ADungeonGenerator_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ADungeonGenerator>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_ADungeonGenerator_Statics::ClassParams = {
		&ADungeonGenerator::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x009000A4u,
		METADATA_PARAMS(Z_Construct_UClass_ADungeonGenerator_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ADungeonGenerator_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ADungeonGenerator()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_ADungeonGenerator_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(ADungeonGenerator, 3555415890);
	template<> ADVGAMESPROGRAMMING_API UClass* StaticClass<ADungeonGenerator>()
	{
		return ADungeonGenerator::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_ADungeonGenerator(Z_Construct_UClass_ADungeonGenerator, &ADungeonGenerator::StaticClass, TEXT("/Script/AdvGamesProgramming"), TEXT("ADungeonGenerator"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(ADungeonGenerator);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
