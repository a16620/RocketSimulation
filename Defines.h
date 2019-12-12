#pragma once
#define WND_SIZE_X 720
#define WND_SIZE_Y 720

#define CONST_GRAVITY 100
#define CONST_ELECTROMAGNETIC 10000

template<typename T>
void TSwap(T& a, T& b) {
	const T tmp = a;
	a = b;
	b = tmp;
}