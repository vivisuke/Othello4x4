#include "utils.h"

std::random_device	g_rd;
#if	1
std::mt19937	g_mt(g_rd());
#else
std::mt19937	g_mt(0);		//	��������Œ�ɂ������ꍇ
#endif
std::uniform_real_distribution<> g_rand11(-1, +1);        // [-1, +1] �͈͂̈�l����
