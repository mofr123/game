#include "publicDef.h"


static ToUTF8 * s_ToUTF8 = nullptr;

ToUTF8::ToUTF8(){};

ToUTF8::~ToUTF8(){};

void ToUTF8::StrToUTF8(std::string& dest, const std::wstring& src)
{
	dest.clear();  
	for (size_t i = 0; i < src.size(); i++){  
		wchar_t w = src[i];  
		if (w <= 0x7f)  
			dest.push_back((char)w);  
		else if (w <= 0x7ff)  
		{  
			dest.push_back(0xc0 | ((w >> 6)& 0x1f));  
			dest.push_back(0x80| (w & 0x3f));  
		}  
		else if (w <= 0xffff)  
		{  
			dest.push_back(0xe0 | ((w >> 12)& 0x0f));  
			dest.push_back(0x80| ((w >> 6) & 0x3f));  
			dest.push_back(0x80| (w & 0x3f));  
		}  
		else if (sizeof(wchar_t) > 2 && w <= 0x10ffff)  
		{  
			dest.push_back(0xf0 | ((w >> 18)& 0x07)); // wchar_t 4-bytes situation  
			dest.push_back(0x80| ((w >> 12) & 0x3f));  
			dest.push_back(0x80| ((w >> 6) & 0x3f));  
			dest.push_back(0x80| (w & 0x3f));  
		}  
		else  
			dest.push_back('?');  
	}  
}

std::string ToUTF8::WStrToUTF8(const std::wstring& str)  
{  
	std::string result;  
	StrToUTF8(result, str);  
	return result;
}


ToUTF8* ToUTF8::getInstance()
{
	if(!s_ToUTF8){
		s_ToUTF8 = new ToUTF8();
	}
	return s_ToUTF8;
}

void ToUTF8::destroyInstance()
{
	delete s_ToUTF8;
}

//================================================================================
//静态数据库
std::string getStaticDBName()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	return _TR_STATIC_DATABASE_FILE_NAME_;
#else
	std::string strResult = FileUtils::getInstance()->getWritablePath();
	strResult+= _TR_STATIC_DATABASE_FILE_NAME_;
	return strResult;
#endif
}

//存储数据库
std::string getSaveDBName()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	return _NF_SAVE_DATABASE_FILE_NAME_;
#else
	std::string strResult = FileUtils::getInstance()->getWritablePath();
	strResult+= _NF_SAVE_DATABASE_FILE_NAME_;
	return strResult;
#endif
}