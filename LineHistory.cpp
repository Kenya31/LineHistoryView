#include "LineHistory.h"

LineHistory::LineHistory(void)
{
	// �R���X�g���N�^
	//cout << "LineHistory()���Ă΂�܂����B" << endl;
	SetZthumbnail((void *)0);
	SetThumbnailSize(0);
	SetThumbFileName("");
}


LineHistory::~LineHistory(void)
{
	// �f�X�g���N�^
	//cout << "~LineHistory()���Ă΂�܂����B" << endl;
}

void LineHistory::SetZthumbnail(void *value)
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
	this->thumbSize = value;
}


int LineHistory::GetThumbnailSize(void)
{
	// zThumbnail�̃Q�b�^�[
	return this->thumbSize;
}

void LineHistory::SetThumbFileName(string value)
{
	// fileName�̃Z�b�^�[
	this->thumbFileName = value;
}


string LineHistory::GetThumbFileName(void)
{
	// fileName�̃Q�b�^�[
	return this->thumbFileName;
}
