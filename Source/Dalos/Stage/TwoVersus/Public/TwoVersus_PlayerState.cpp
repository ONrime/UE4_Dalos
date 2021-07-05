// Fill out your copyright notice in the Description page of Project Settings.


#include "Dalos/Stage/TwoVersus/Public/TwoVersus_PlayerState.h"

ATwoVersus_PlayerState::ATwoVersus_PlayerState() 
{
	playerHP = 100;
}

void ATwoVersus_PlayerState::DamageToHP(float damage)
{
	playerHP = playerHP - damage;
}
