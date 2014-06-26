/**
* LineHistoryView.exe
*
* Create Date 2014/03/30
*
*
* Update 2014/06/01
*		�G���[�n���h�����O��ǉ�
*		zTimeStamp�ɒl�������Ă��Ȃ������ꍇ�ɃG���[�ƂȂ�����C��
*		�T���l�C�����T���l�C���t�B�[���h�ɕ\������悤�ɕύX�B
*
* Update 2014/06/08
*		�T���l�C������e�t�B�[���h�ɕ\������悤�ɕύX
*
* Update 2014/06/10
*		SELECT����DATETIME�֐���ǉ��B
*		����ɔ������낢��ύX
*
*/
#include <direct.h>
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "LineHistory.h"

using namespace std;

#pragma execution_character_set("UTF-8")

#define DOCTYPE_HTML "<!DOCTYPE html>"
#define HTML_TAG_START "<html>"
#define HTML_TAG_END "</html>"
#define HEAD_TAG_START "<head>"
#define HEAD_TAG_END "</head>"
#define TITLE_TAG_START "<title>"
#define TITLE_TAG_END "</title>"
#define STYLE_TAG_START "<style>"
#define DIV_STYLE "div { white-space:pre; }"
#define STYLE_TAG_END "</style>"
#define BODY_TAG_START "<body>"
#define BODY_TAG_END "</body>"
#define META_TAG "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />"
#define BREAK_TAG "<br/>"
#define EMPTY_STRING ""
#define RESULT_DIR "result"
#define THUMBNAIL_DIR "result/thumbnail"
#define LINE_HTML "result/Line.html"

/**
* �N�G�����ʂ�HTML�^�O�Ő��`���ďo��
* �ꗗ�`��
*
*/
void PrintResult(vector<LineHistory> array, ofstream *line_html) {
	string cType;
	string zTimeStamp;
	string mType;
	string zAddress;
	string zChat;
	string zContentType;
	string zName;
	string zOpt;
	string zId;
	string zText;
	void * zThumbnail;
	int size;
	string tmp;
	string fileName;

	// �O���ZCHAT�̒l
	string last_zChat;

	// 1���R�[�h���̃f�[�^���i�[����I�u�W�F�N�g
	LineHistory lh;
	vector<LineHistory>::iterator it = array.begin();

	// 1���R�[�h������
	while( it != array.end() ) {
		lh = *it;

		// ����
		zTimeStamp = lh.GetZtimeStamp();

		// �A�h���X����̖��O
		zAddress = lh.GetZaddressBookName();

		// �g�[�N�̋��
		zChat = lh.GetZchat();

		// Content Type
		zContentType = lh.GetZcontentType();
		if ("0" == zContentType) {
			// �e�L�X�g
			cType = "�e�L�X�g";
		} else if ("1" == zContentType) {
			// �摜
			cType = "�摜";
		} else if ("6" == zContentType) {
			// �����ʘb
			cType = "�����ʘb";
		} else if ("7" == zContentType) {
			// �X�^���v
			cType = "�X�^���v";
			lh.SetZtext(cType);
		} else if ("100" == zContentType) {
			// �X�^���v
			cType = "�ʒu���";
		} else {
			// �s���Ȓl
			cType = "�s���Ȓl[" + zContentType + "]";
		}

		// LINE��̖��O
		zName = lh.GetZname();
		if (zAddress.empty() && zName.empty()) {
			mType = "���M";
		} else {
			mType = "��M";
		}

		// ����M���
		zOpt = lh.GetZopt();

		// ���b�Z�[�W
		zText = lh.GetZtext();

		// �摜�̃T���l�C��
		zThumbnail = lh.GetZthumbnail();
		size = lh.GetThumbnailSize();
		if (0 != size) {
			// �t�@�C������ZID����쐬
			fileName = lh.GetFileName();
			ofstream fout;
			fout.open(THUMBNAIL_DIR + string("/") + fileName, ios::out|ios::binary|ios::trunc);
			fout.write((char *)zThumbnail, size);
			fout.close();
			// �T���l�C���̂��߂Ɋm�ۂ����̈���������
			free(zThumbnail);
		}
		if (it == array.begin()) {
			// �ŏ��̃��R�[�h
			*line_html << "<table border=\"2\">" << endl;
			*line_html << "<th width=\"150\" nowrap>����(���{����)</th>" << endl;
			//*line_html << "<th width=\"150\" nowrap>�A�h���X����̖��O</th>" << endl;
			*line_html << "<th width=\"150\" nowrap>���O</th>" << endl;
			*line_html << "<th nowrap>����M���</th>" << endl;
			//*line_html << "<th nowrap>���e���</th>" << endl;
			*line_html << "<th>���e</th>" << endl;
		} else if (last_zChat != zChat) {
			// zChat�̒l���O��ƈႤ
			*line_html << "</table>" << endl;
			*line_html << "<hr size=\"5\"/>" << endl;
			*line_html << "<table border=\"2\">" << endl;
			// �w�b�_�[�o��
			*line_html << "<th width=\"150\" nowrap>����(���{����)</th>" << endl;
			//*line_html << "<th width=\"150\" nowrap>�A�h���X����̖��O</th>" << endl;
			*line_html << "<th width=\"150\" nowrap>���O</th>" << endl;
			*line_html << "<th nowrap>����M���</th>" << endl;
			//*line_html << "<th nowrap>���e���</th>" << endl;
			*line_html << "<th>���e</th>" << endl;
		}

		// zChat���L�^
		last_zChat = zChat;

		// �o��
		*line_html << "<tr>" << endl;
		*line_html << "<td nowrap>" << zTimeStamp << "</td>" << endl;
		//*line_html << "<td nowrap>" << zAddress << "</td>" << endl;
		*line_html << "<td nowrap>" << zName << "</td>" << endl;
		*line_html << "<td nowrap align=\"center\">" << mType << "</td>" << endl;
		//*line_html << "<td nowrap>" << cType << "</td>" << endl;
		if (EMPTY_STRING == fileName) {
			// �摜����
			*line_html << "<td>" << zText << "</td>" << endl;
		} else {
			// �摜�L��
			*line_html << "<td>" << zText << endl;
			*line_html << "<br/><img border=\"0\" src=./thumbnail/" << fileName << " /></td>" << endl;
		}
		*line_html << "</tr>" << endl;

		// �C�e���[�^��i�߂�
		it++;

		// �ϐ����N���A
		zTimeStamp = EMPTY_STRING;
		zAddress = EMPTY_STRING;
		zName = EMPTY_STRING;
		mType = EMPTY_STRING;
		cType = EMPTY_STRING;
		zText = EMPTY_STRING;
		fileName = EMPTY_STRING;
	}
}

/**
* HTML�^�O�̊J�n���o��
*/
void PrintHtmlTagStart(ofstream *line_html) {
	*line_html << DOCTYPE_HTML << endl;
	*line_html << HTML_TAG_START << endl;
	*line_html << HEAD_TAG_START << endl;
	*line_html << META_TAG << endl;
	*line_html << TITLE_TAG_START << endl;
	*line_html << TITLE_TAG_END << endl;
	*line_html << STYLE_TAG_START << endl;
	*line_html << DIV_STYLE << endl;
	*line_html << STYLE_TAG_END << endl;
	*line_html << HEAD_TAG_END << endl;
	*line_html << BODY_TAG_START << endl;
	*line_html << BREAK_TAG << endl;
}

/**
* HTML�^�O�̏I�[���o��
*/
void PrintHtmlTagEnd(ofstream *line_html) {
	*line_html << HTML_TAG_END << endl;
	*line_html << BODY_TAG_END << endl;
}

/**
* main routine.
*
*
*/
int main(int argc, char * argv[])
{
	int result = -1;
	sqlite3 *db = (sqlite3 *)0;
	const char *sql = "SELECT ZMESSAGE.Z_OPT, ZMESSAGE.ZCONTENTTYPE"
		", DATETIME(ZMESSAGE.ZTIMESTAMP/1000, 'unixepoch', 'localtime')"
		", ZMESSAGE.ZCHAT, ZMESSAGE.ZID, ZMESSAGE.ZTEXT"
		", ZMESSAGE.ZTHUMBNAIL, ZUSER.ZADDRESSBOOKNAME"
		", ZUSER.ZNAME"
		" FROM ZMESSAGE LEFT JOIN ZUSER ON ZMESSAGE.ZSENDER = ZUSER.Z_PK"
		" ORDER BY ZMESSAGE.ZCHAT, ZMESSAGE.ZTIMESTAMP;";

	sqlite3_stmt *stmt = (sqlite3_stmt *)0;
	char *errMsg = (char *)0;

	// �T���l�C�����i�[
	int size = 0;
	void *buf = (char *)0;

	// �N�G�����ʂ��i�[����z��
	vector<LineHistory> array;

	// �N�G������1���R�[�h�����i�[����I�u�W�F�N�g
	LineHistory lh;

	// �t�@�C�����݊m�F�̂��߂̕ϐ�
	struct stat statFile;

	string tmp;
	const unsigned char * zid = (unsigned char *)0;
	const unsigned char * value = (unsigned char *)0;
	const void * zThumbnail = (void *)0;

	// �o�̓t�@�C��
	ofstream line_html;

	// Initialize.
	memset(&lh, 0, sizeof(lh));
	memset(&statFile, 0, sizeof(statFile));

	// �������`�F�b�N
	if (1 == argc) {
		// �������w�肳��Ă��Ȃ�
		fprintf(stderr, "Usage: %s <talk.sqlite>\n", argv[0]);
		exit(-1);
	} else if (0 != stat(argv[1], &statFile)) {
		// �t�@�C��������
		fprintf(stderr, "File \"%s\" does not exist.\n", argv[1]);
		exit(-2);
	} else {
		// �t�@�C�����L��
		switch (statFile.st_mode & S_IFMT) {
		case S_IFCHR:
			fprintf(stderr,"File \"%s\" is character device.\n", argv[1]);
			exit(-3);
			break;
		case S_IFDIR:
			fprintf(stderr,"File \"%s\" is directory.\n", argv[1]);
			exit(-4);
			break;
		case S_IFREG:
			//fprintf(stdout,"File \"%s\" is regular file.\n", argv[1]);
			break;
		default:
			fprintf(stderr,"File \"%s\" is unknown.\n", argv[1]);
			exit(-5);
			break;
		}
	}

	// Check output directory.
	if (0 == stat(RESULT_DIR, &statFile)) {
		// "result" exists.
		fprintf(stderr,"Directory \"%s\" exists.\n", RESULT_DIR);
		exit(-6);
	}

	// DB�t�@�C�����I�[�v��
	result = sqlite3_open(argv[1], &db);

	if (SQLITE_OK != result) {
		fprintf(stderr, "sqlite3_open result[%d]\n", result);
		goto close;
	} else {
		// stmt����
		sqlite3_prepare(db, sql, strlen(sql), &stmt, NULL);

		//stmt���̃o�b�t�@���N���A
		sqlite3_reset(stmt);

		/* SELECT���̎��s */
		while (SQLITE_ROW == (result = sqlite3_step(stmt))) {
			lh = LineHistory();
			tmp = EMPTY_STRING;
			// const unsiged char *��string�ɃL���X�g���A
			// �I�u�W�F�N�g�ɋl�ߋl�߂���B
			// ���R�[�h�̍ŏ��̃t�B�[���h�̒l���擾
			// Z_OPT
			value = sqlite3_column_text(stmt, 0);
			if ((unsigned char *)0 == value) {
				lh.SetZopt(EMPTY_STRING);
			} else {
				tmp = (reinterpret_cast <char const *> (value));
				lh.SetZopt(tmp);
			}

			// ���R�[�h��2�Ԗڂ̃t�B�[���h�̒l���擾
			// ZCONTENTTYPE
			value = sqlite3_column_text(stmt, 1);
			if ((unsigned char *)0 == value) {
				lh.SetZcontentType(EMPTY_STRING);
			} else {
				tmp = (reinterpret_cast <char const *> (value));
				lh.SetZcontentType(tmp);
			}

			// ���R�[�h��3�Ԗڂ̃t�B�[���h�̒l���擾
			// ZTIMESTAMP
			value = sqlite3_column_text(stmt, 2);
			if ((unsigned char *)0 == value) {
				lh.SetZtimeStamp(EMPTY_STRING);
			} else {
				tmp = (reinterpret_cast <char const *> (value));
				lh.SetZtimeStamp(tmp);
			}

			// ���R�[�h��4�Ԗڂ̃t�B�[���h�̒l���擾
			// ZCHAT
			value = sqlite3_column_text(stmt, 3);
			if ((unsigned char *)0 == value) {
				lh.SetZchat(EMPTY_STRING);
			} else {
				tmp = (reinterpret_cast <char const *> (value));
				lh.SetZchat(tmp);
			}

			// ���R�[�h��5�Ԗڂ̃t�B�[���h�̒l���擾
			// ZID�̓t�@�C�����Ɏg�p����
			zid = sqlite3_column_text(stmt, 4);
			if ((unsigned char *)0 == zid) {
				lh.SetZid(EMPTY_STRING);
			} else {
				tmp = (reinterpret_cast <char const *> (zid));
				lh.SetZid(tmp);
				// �T���l�C���̃t�@�C�������i�[
				lh.SetFileName(tmp + ".jpg");
			}

			// ���R�[�h��6�Ԗڂ̃t�B�[���h�̒l���擾
			// ZTEXT
			value = sqlite3_column_text(stmt, 5);
			if ((unsigned char *)0 == value) {
				lh.SetZtext(EMPTY_STRING);
			} else {
				tmp = (reinterpret_cast <char const *> (value));
				lh.SetZtext(tmp);
			}

			// ���R�[�h��7�Ԗڂ̃t�B�[���h�̒l���擾
			// ZTHUMBNAIL
			size = sqlite3_column_bytes(stmt, 6);
			if (0 == size) {
				lh.SetZthumbnail(EMPTY_STRING);
			} else {
				// �T���l�C���̃f�[�^�i�[�p�o�b�t�@���m��
				buf = calloc(size, sizeof(char));
				// �T���l�C���̃f�[�^���R�s�[
				zThumbnail = (char *)sqlite3_column_blob(stmt, 6);
				memcpy(buf, zThumbnail, size);
				lh.SetZthumbnail(buf);
			}
			// �T���l�C���̃T�C�Y���i�[
			lh.SetThumbnailSize(size);

			// ���R�[�h��8�Ԗڂ̃t�B�[���h�̒l���擾
			// ZADDRESSBOOKNAME
			value = sqlite3_column_text(stmt, 7);
			if ((unsigned char *)0 == value) {
				lh.SetZaddressBookName(EMPTY_STRING);
			} else {
				tmp = (reinterpret_cast <char const *> (value));
				lh.SetZaddressBookName(tmp);
			}

			// ���R�[�h��9�Ԗڂ̃t�B�[���h�̒l���擾
			// ZNAME
			value = sqlite3_column_text(stmt, 8);
			if ((unsigned char *)0 == value) {
				lh.SetZname(EMPTY_STRING);
			} else {
				tmp = (reinterpret_cast <char const *> (value));
				lh.SetZname(tmp);
			}

			// �I�u�W�F�N�g�z��̃P�c�ɒǉ��B
			array.push_back(lh);
			lh.~LineHistory();
		}

		if (SQLITE_DONE == result) {
			// �o�̓t�H���_�쐬
			if (0 != stat(RESULT_DIR, &statFile)) {
				// result�t�H���_������
				_mkdir(RESULT_DIR);
			}
			if (0 != stat(THUMBNAIL_DIR, &statFile)) {
				// result\thumbnail�t�H���_������
				_mkdir(THUMBNAIL_DIR);
			}
			line_html.open(LINE_HTML, ios::out|ios::binary|ios::trunc);

			PrintHtmlTagStart(&line_html);
			PrintResult(array, &line_html);
			PrintHtmlTagEnd(&line_html);
			//line_html.flush();
			line_html.close();
		} else {
			fprintf(stderr, "[ERROR] %s\n", errMsg );
		}
	}
	// �G���[���b�Z�[�W�p�o�b�t�@�����
	sqlite3_free(errMsg);

	// stmt�����
	sqlite3_finalize(stmt);

close:
	// DB�����
	sqlite3_close(db);

	return result;
}
