#include "LineHistory.h"

LineHistory::LineHistory(void)
{
	// �R���X�g���N�^
}


LineHistory::~LineHistory(void)
{
	// �f�X�g���N�^
}

void LineHistory::SetZopt(string value)
{
	// zOpt�̃Z�b�^�[
	this->zOpt = value;
}


string LineHistory::GetZopt(void)
{
	// zOpt�̃Q�b�^�[
	return this->zOpt;
}

void LineHistory::SetZcontentType(string value)
{
	// zContentType�̃Z�b�^�[
	this->zContentType = value;
}


string LineHistory::GetZcontentType(void)
{
	// zContentType�̃Q�b�^�[
	return this->zContentType;
}

void LineHistory::SetZtimeStamp(string value)
{
	// zTimeStamp�̃Z�b�^�[
	this->zTimeStamp = value;
}


string LineHistory::GetZtimeStamp(void)
{
	// zTimeStamp�̃Q�b�^�[
	return this->zTimeStamp;
}

void LineHistory::SetZchat(string value)
{
	// zChat�̃Z�b�^�[
	this->zChat = value;
}


string LineHistory::GetZchat(void)
{
	// zChat�̃Q�b�^�[
	return this->zChat;
}

void LineHistory::SetZid(string value)
{
	// zId�̃Z�b�^�[
	this->zId = value;
}


string LineHistory::GetZid(void)
{
	// zId�̃Q�b�^�[
	return this->zId;
}

void LineHistory::SetZtext(string value)
{
	// zText�̃Z�b�^�[
	this->zText = value;
}


string LineHistory::GetZtext(void)
{
	// zText�̃Q�b�^�[
	return this->zText;
}

void LineHistory::SetZthumbnail(void * value)
{
	// zThumbnail�̃Z�b�^�[
	this->zThumbnail = value;
}


void * LineHistory::GetZthumbnail(void)
{
	// zThumbnail�̃Q�b�^�[
	return this->zThumbnail;
}

void LineHistory::SetThumbnailSize(int value)
{
	// zThumbnail�̃Z�b�^�[
	this->thumbnailSize = value;
}


int LineHistory::GetThumbnailSize(void)
{
	// zThumbnail�̃Q�b�^�[
	return this->thumbnailSize;
}

void LineHistory::SetZaddressBookName(string value)
{
	// zAddressBookName�̃Z�b�^�[
	this->zAddressBookName = value;
}


string LineHistory::GetZaddressBookName(void)
{
	// zAddressBookName�̃Q�b�^�[
	return this->zAddressBookName;
}

void LineHistory::SetZname(string value)
{
	// zName�̃Z�b�^�[
	this->zName = value;
}


string LineHistory::GetZname(void)
{
	// zName�̃Q�b�^�[
	return this->zName;
}

void LineHistory::SetFileName(string value)
{
	// fileName�̃Z�b�^�[
	this->fileName = value;
}


string LineHistory::GetFileName(void)
{
	// fileName�̃Q�b�^�[
	return this->fileName;
}
