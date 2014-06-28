#include "LineHistory.h"

LineHistory::LineHistory(void)
{
	// コンストラクタ
	//cout << "LineHistory()が呼ばれました。" << endl;
	SetZthumbnail((void *)0);
	SetThumbnailSize(0);
	SetThumbFileName("");
}


LineHistory::~LineHistory(void)
{
	// デストラクタ
	//cout << "~LineHistory()が呼ばれました。" << endl;
}

void LineHistory::SetZthumbnail(void *value)
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
	this->thumbSize = value;
}


int LineHistory::GetThumbnailSize(void)
{
	// zThumbnailのゲッター
	return this->thumbSize;
}

void LineHistory::SetThumbFileName(string value)
{
	// fileNameのセッター
	this->thumbFileName = value;
}


string LineHistory::GetThumbFileName(void)
{
	// fileNameのゲッター
	return this->thumbFileName;
}
