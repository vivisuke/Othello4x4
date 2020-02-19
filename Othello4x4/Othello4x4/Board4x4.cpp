#include "Board4x4.h"

using namespace std;

//	盤面初期化
//		・・・・
//		・○●・
//		・●○・
//		・・・・
void Board4x4::init()
{
	m_black = 0x0240;
	m_white = 0x0420;
}
std::string Board4x4::text() const
{
	string txt;
	return txt;
}

