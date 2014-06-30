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
void PrintResult(vector<LineHistory> records, ofstream *line_html) {
	string zTimeStamp;
	string mType;
	string zChat;
	string zContentType;
	string zName;
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
	vector<LineHistory>::iterator it = records.begin();

	// 1���R�[�h������
	while( it != records.end() ) {
		lh = *it;

		// ZCONTENTTYPE
		zContentType = lh.GetValue("ZCONTENTTYPE");

		// ZTIMESTAMP
		zTimeStamp = lh.GetValue("ZTIMESTAMP");

		// ZCHAT
		zChat = lh.GetValue("ZCHAT");

		// ZTEXT
		zText = lh.GetValue("ZTEXT");

		// ZNAME
		zName = lh.GetValue("ZNAME");

		if ((0 == zName.length()) || zName.empty()) {
			mType = "���M";
		} else {
			mType = "��M";
			if ("6" == zContentType) {
				// Phone call
				zText = "�������ʘb��";
			}
		}

		if ("7" == zContentType) {
				// Stamp
				zText = "���X�^���v��";
		}

		// ZTHUMBNAIL
		zThumbnail = lh.GetZthumbnail();
		size = lh.GetThumbnailSize();
		if (0 != size) {
			fileName = lh.GetThumbFileName();
			ofstream fout;
			fout.open(THUMBNAIL_DIR + string("/") + fileName, ios::out|ios::binary|ios::trunc);
			fout.write((char *)zThumbnail, size);
			fout.close();
			// Free allocated memory.
			free(zThumbnail);
		}

		if (it == records.begin()) {
			// First record.
			*line_html << "<table border=\"2\">" << endl;
			*line_html << "<th width=\"150\" nowrap>����(���{����)</th>" << endl;
			*line_html << "<th width=\"150\" nowrap>���O</th>" << endl;
			*line_html << "<th nowrap>����M���</th>" << endl;
			*line_html << "<th>���e</th>" << endl;
		} else if (last_zChat != zChat) {
			// Differ last calue of zChat.
			*line_html << "</table>" << endl;
			*line_html << "<hr size=\"5\"/>" << endl;
			*line_html << "<table border=\"2\">" << endl;
			// Output header
			*line_html << "<th width=\"150\" nowrap>����(���{����)</th>" << endl;
			*line_html << "<th width=\"150\" nowrap>���O</th>" << endl;
			*line_html << "<th nowrap>����M���</th>" << endl;
			*line_html << "<th>���e</th>" << endl;
		}

		// zChat���L�^
		last_zChat = zChat;

		// �o��
		*line_html << "<tr>" << endl;
		*line_html << "<td nowrap>" << zTimeStamp << "</td>" << endl;
		*line_html << "<td nowrap>" << zName << "</td>" << endl;
		*line_html << "<td nowrap align=\"center\">" << mType << "</td>" << endl;
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
		zName = EMPTY_STRING;
		mType = EMPTY_STRING;
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
*  Execute query and get result set to "results".
*/
int getLineHistoryRecords(sqlite3 *db, vector<LineHistory> *results) {
	int sqlite3_result = 0;
	int columnCount = 0;
	const char *sql = "SELECT ZMESSAGE.Z_OPT, ZMESSAGE.ZCONTENTTYPE"
		", DATETIME(ZMESSAGE.ZTIMESTAMP/1000, 'unixepoch', 'localtime') AS ZTIMESTAMP"
		", ZMESSAGE.ZCHAT, ZMESSAGE.ZID, ZMESSAGE.ZTEXT"
		", ZMESSAGE.ZTHUMBNAIL, ZUSER.ZADDRESSBOOKNAME"
		", ZUSER.ZNAME"
		" FROM ZMESSAGE LEFT JOIN ZUSER ON ZMESSAGE.ZSENDER = ZUSER.Z_PK"
		" ORDER BY ZMESSAGE.ZCHAT, ZMESSAGE.ZTIMESTAMP;";
	const char *param = (char *)0;
	const unsigned char *value = (unsigned char *)0;

	LineHistory lh;
	// Prepared statement.
	sqlite3_stmt *stmt = (sqlite3_stmt *)0;

	if ((sqlite3 *)0 == db) {
		// DB file is not open.
		results = (vector<LineHistory> *)0;
	} else {
		// Generate statement.
		sqlite3_prepare(db, sql, strlen(sql), &stmt, NULL);

		// Clear statement buffer.
		sqlite3_reset(stmt);

		// Set columns count.
		columnCount = sqlite3_column_count(stmt);

		// Execute query.
		while (SQLITE_ROW == (sqlite3_result = sqlite3_step(stmt))) {
			lh = LineHistory();
			for (int i = 0; i < columnCount ; i++) {
				param = sqlite3_column_name(stmt, i);
				value = sqlite3_column_text(stmt, i);
				if ((unsigned char *)0 == value) {
					lh.SetValue(param, EMPTY_STRING);
				} else {
					if (0 == strcmp(param, "ZTHUMBNAIL")) {
						// ZTHUMBNAIL
						// �T���l�C�����i�[
						char *zThumbnail = (char *)0;
						int size = sqlite3_column_bytes(stmt, i);
						void *buf = (char *)0;

						if (0 == size) {
							// Result set does not have thumbnail.
							lh.SetZthumbnail((void *)0);
						} else {
							// Allocate memory for thumbnail.
							buf = calloc(size, sizeof(char));
							// Copy data of thumbnail to buffer.
							zThumbnail = (char *)sqlite3_column_blob(stmt, i);
							memcpy(buf, zThumbnail, size);
							lh.SetZthumbnail(buf);
						}
						// Set thumbnail size.
						lh.SetThumbnailSize(size);
						// Set ZID for filename ofthumbnail.
						lh.SetThumbFileName(lh.GetValue("ZID") + string(".jpg"));
					}
					lh.SetValue(param, reinterpret_cast <char const *> (value));
				}
			}
			// Add to object array.
			results->push_back(lh);
		}
		// stmt�����
		sqlite3_finalize(stmt);
	}
	return sqlite3_result;
}

/**
*  Execute query and get result set to "results".
*/
int getRecords(sqlite3 *db, const char *sql, vector<Record> *results) {
	Record rec;

	int sqlite3_result = 0;
	int columnCount = 0;
	sqlite3_stmt *stmt = (sqlite3_stmt *)0;
	// Error message.
	char *errMsg = (char *)0;
	const char *param = (char *)0;
	const unsigned char *value = (unsigned char *)0;

	if ((sqlite3 *)0 == db) {
		// DB file is not open.
		results = (vector<Record> *)0;
	} else {
		// Generate statement.
		sqlite3_prepare(db, sql, strlen(sql), &stmt, NULL);

		// Clear statement buffer.
		sqlite3_reset(stmt);

		// Set columns count.
		columnCount = sqlite3_column_count(stmt);

		// Execute query.
		while (SQLITE_ROW == (sqlite3_result = sqlite3_step(stmt))) {
			rec = Record();
			for (int i = 0; i < columnCount ; i++) {
				param = sqlite3_column_name(stmt, i);
				value = sqlite3_column_text(stmt, i);
				if ((unsigned char *)0 == value) {
					rec.SetValue(param, EMPTY_STRING);
				} else {
					rec.SetValue(param, reinterpret_cast <char const *> (value));
				}
			}
			// Add to object array.
			results->push_back(rec);
		}
		// stmt�����
		sqlite3_finalize(stmt);
	}

	return sqlite3_result;
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

	// �N�G�����ʂ��i�[����z��
	vector<LineHistory> results;

	// �t�@�C�����݊m�F�̂��߂̕ϐ�
	struct stat statFile;

	// �o�̓t�@�C��
	ofstream line_html;

	// Initialize.
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

	// Open db file.
	result = sqlite3_open(argv[1], &db);

	if (SQLITE_OK != result) {
		fprintf(stderr, "sqlite3_open result[%d]\n", result);
		goto close;
	} else {
		result = getLineHistoryRecords(db, &results);
		if (SQLITE_DONE == result) {
			// Create output directory.
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
			PrintResult(results, &line_html);
			PrintHtmlTagEnd(&line_html);
			line_html.flush();
			line_html.close();
		} else { 
			fprintf(stderr, "sqlite3_step result[%d]\n", result);
		}
	}

close:
	// DB�����
	sqlite3_close(db);

	return result;
}
