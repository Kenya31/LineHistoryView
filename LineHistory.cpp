#include "LineHistory.h"

LineHistory::LineHistory(void)
{
	// コンストラクタ
}


LineHistory::~LineHistory(void)
{
	// デストラクタ
}

void LineHistory::SetZopt(string value)
{
	// zOptのセッター
	this->zOpt = value;
}


string LineHistory::GetZopt(void)
{
	// zOptのゲッター
	return this->zOpt;
}

void LineHistory::SetZcontentType(string value)
{
	// zContentTypeのセッター
	this->zContentType = value;
}


string LineHistory::GetZcontentType(void)
{
	// zContentTypeのゲッター
	return this->zContentType;
}

void LineHistory::SetZtimeStamp(string value)
{
	// zTimeStampのセッター
	this->zTimeStamp = value;
}


string LineHistory::GetZtimeStamp(void)
{
	// zTimeStampのゲッター
	return this->zTimeStamp;
}

void LineHistory::SetZchat(string value)
{
	// zChatのセッター
	this->zChat = value;
}


string LineHistory::GetZchat(void)
{
	// zChatのゲッター
	return this->zChat;
}

void LineHistory::SetZid(string value)
{
	// zIdのセッター
	this->zId = value;
}


string LineHistory::GetZid(void)
{
	// zIdのゲッター
	return this->zId;
}

void LineHistory::SetZtext(string value)
{
	// zTextのセッター
	this->zText = value;
}


string LineHistory::GetZtext(void)
{
	// zTextのゲッター
	return this->zText;
}

void LineHistory::SetZthumbnail(void * value)
{
	// zThumbnailのセッター
	this->zThumbnail = value;
}


void * LineHistory::GetZthumbnail(void)
{
	// zThumbnailのゲッター
	return this->zThumbnail;
}

void LineHistory::SetThumbnailSize(int value)
{
	// zThumbnailのセッター
	this->thumbnailSize = value;
}


int LineHistory::GetThumbnailSize(void)
{
	// zThumbnailのゲッター
	return this->thumbnailSize;
}

void LineHistory::SetZaddressBookName(string value)
{
	// zAddressBookNameのセッター
	this->zAddressBookName = value;
}


string LineHistory::GetZaddressBookName(void)
{
	// zAddressBookNameのゲッター
	return this->zAddressBookName;
}

void LineHistory::SetZname(string value)
{
	// zNameのセッター
	this->zName = value;
}


string LineHistory::GetZname(void)
{
	// zNameのゲッター
	return this->zName;
}

void LineHistory::SetFileName(string value)
{
	// fileNameのセッター
	this->fileName = value;
}


string LineHistory::GetFileName(void)
{
	// fileNameのゲッター
	return this->fileName;
}
