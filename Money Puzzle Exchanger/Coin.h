#pragma once
#include "PCH.h"
#include "CoinType.h"

struct Coin
{
	CoinType Type;

	bool CanCombine = false;
	bool Visited = false;
};