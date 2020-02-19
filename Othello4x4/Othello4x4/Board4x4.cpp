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
	for (int mask = MSB, i = 0; mask != 0; mask>>=1) {
		if( (m_black & mask) != 0 )
			txt += "●";
		else if( (m_white & mask) != 0 )
			txt += "○";
		else
			txt += "・";
		if( ++i % 4 == 0 )
			txt += "\n";
	}
	return txt;
}

