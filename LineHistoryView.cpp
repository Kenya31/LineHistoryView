/**
* LineHistoryView.exe
*
* Create Date 2014/03/30
*
*
* Update 2014/06/01
*		エラーハンドリングを追加
*		zTimeStampに値が入っていなかった場合にエラーとなる問題を修正
*		サムネイルをサムネイルフィールドに表示するように変更
*
* Update 2014/06/08
*		サムネイルを内容フィールドに表示するように変更
*
* Update 2014/06/10
*		SELECT文にDATETIME関数を追加
*		それに伴いいろいろ変更
*
* Update 2015/07/29
*		resultディレクトリをresult.20120729121314と時刻を追加
*/
#include <direct.h>
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "LineHistory.h"

#if defined(_MSC_VER)
#pragma execution_character_set("utf-8")
#endif

using namespace std;

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
#define THUMBNAIL_DIR "/thumbnail"
#define LINE_HTML "/Line.html"

#define SQL_FOR_IPHONE "SELECT ZMESSAGE.Z_OPT, ZMESSAGE.ZCONTENTTYPE"\
	", DATETIME(ZMESSAGE.ZTIMESTAMP/1000, 'unixepoch', 'localtime') AS ZTIMESTAMP"\
	", ZMESSAGE.ZCHAT, ZMESSAGE.ZID, ZMESSAGE.ZTEXT"\
	", ZMESSAGE.ZTHUMBNAIL, ZUSER.ZADDRESSBOOKNAME"\
	", ZUSER.ZNAME"\
	" FROM ZMESSAGE LEFT JOIN ZUSER ON ZMESSAGE.ZSENDER = ZUSER.Z_PK"\
	" ORDER BY ZMESSAGE.ZCHAT, ZMESSAGE.ZTIMESTAMP;"

#define SQL_FOR_ANDROID "SELECT chat_history.attachment_type, "\
	", DATETIME(chat_history.created_time/1000, 'unixepoch', 'localtime') AS created_time"\
	", chat_history.chat_id, contacts.name, chat_history.content"\
	"FROM chat_history LEFT JOIN contacts ON chat_history.from_mid = contacts.m_id"\
	"ORDER BY chat_history.chat_id, chat_history.chat_id;"

/**
* クエリ結果をHTMLタグで整形して出力
* 一覧形式
*
*/
void PrintResult(vector<LineHistory> records, ofstream *line_html, string thumbNailDir) {
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

	// 前回のZCHATの値
	string last_zChat;

	// 1レコード分のデータを格納するオブジェクト
	LineHistory lh;
	vector<LineHistory>::iterator it = records.begin();

	// 1レコードずつ処理
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
			mType = "送信";
		} else {
			mType = "受信";
			if ("6" == zContentType) {
				// Phone call
				zText = "＜音声通話＞";
			}
		}

		if ("7" == zContentType) {
			// Stamp
			zText = "＜スタンプ＞";
		}

		// ZTHUMBNAIL
		zThumbnail = lh.GetZthumbnail();
		size = lh.GetThumbnailSize();
		if (0 != size) {
			fileName = lh.GetThumbFileName();
			ofstream fout;
			fout.open(thumbNailDir + string("/") + fileName, ios::out|ios::binary|ios::trunc);
			fout.write((char *)zThumbnail, size);
			fout.close();
			// Free allocated memory.
			free(zThumbnail);
		}

		if (it == records.begin()) {
			// First record.
			*line_html << "<table border=\"2\">" << endl;
			*line_html << "<th width=\"150\" nowrap>日時(日本時間)</th>" << endl;
			*line_html << "<th width=\"150\" nowrap>名前</th>" << endl;
			*line_html << "<th nowrap>送受信種別</th>" << endl;
			*line_html << "<th>内容</th>" << endl;
		} else if (last_zChat != zChat) {
			// Differ last calue of zChat.
			*line_html << "</table>" << endl;
			*line_html << "<hr size=\"5\"/>" << endl;
			*line_html << "<table border=\"2\">" << endl;
			// Output header
			*line_html << "<th width=\"150\" nowrap>日時(日本時間)</th>" << endl;
			*line_html << "<th width=\"150\" nowrap>名前</th>" << endl;
			*line_html << "<th nowrap>送受信種別</th>" << endl;
			*line_html << "<th>内容</th>" << endl;
		}

		// zChatを記録
		last_zChat = zChat;

		// 出力
		*line_html << "<tr>" << endl;
		*line_html << "<td nowrap>" << zTimeStamp << "</td>" << endl;
		*line_html << "<td nowrap>" << zName << "</td>" << endl;
		*line_html << "<td nowrap align=\"center\">" << mType << "</td>" << endl;
		if (EMPTY_STRING == fileName) {
			// 画像無し
			*line_html << "<td>" << zText << "</td>" << endl;
		} else {
			// 画像有り
			*line_html << "<td>" << endl;
			*line_html << zText << endl;
			*line_html << "<br/>" << endl;
			*line_html << "<img border=\"0\" src=./thumbnail/" << fileName << " /><br/>" << endl;
			*line_html << fileName << endl;
			*line_html << "</td>" << endl;
		}
		*line_html << "</tr>" << endl;

		// イテレータを進める
		it++;

		// 変数をクリア
		zTimeStamp = EMPTY_STRING;
		zName = EMPTY_STRING;
		mType = EMPTY_STRING;
		zText = EMPTY_STRING;
		fileName = EMPTY_STRING;
	}
}

/**
* HTMLタグの開始を出力
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
* HTMLタグの終端を出力
*/
void PrintHtmlTagEnd(ofstream *line_html) {
	*line_html << BODY_TAG_END << endl;
	*line_html << HTML_TAG_END << endl;
}

/**
*  Execute query and get result set to "results".
*/
int getLineHistoryRecords(sqlite3 *db, vector<LineHistory> *results) {
	int sqlite3_result = 0;
	int columnCount = 0;
	const char *sql = SQL_FOR_IPHONE;
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
						// サムネイルを格納
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
		// stmtを解放
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
		// stmtを解放
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

	// クエリ結果を格納する配列
	vector<LineHistory> results;

	// ファイル存在確認のための変数
	struct stat statFile;

	// 出力ファイル
	ofstream line_html;

	// Initialize.
	memset(&statFile, 0, sizeof(statFile));

	// 引数をチェック
	if (1 == argc) {
		// 引数が指定されていない
		fprintf(stderr, "Usage: %s <talk.sqlite>\n", argv[0]);
		exit(-1);
	} else if (0 != stat(argv[1], &statFile)) {
		// ファイルが無い
		fprintf(stderr, "File \"%s\" does not exist.\n", argv[1]);
		exit(-2);
	} else {
		// ファイルが有る
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

	// Make directory name.
	string dirName;
	string thumbNailDir;
	string lineHtml;
	string dateTime;
	struct tm date;
	char tmp[32];
	time_t timer;

	memset(&date, '\0', sizeof(date));
	timer = time(NULL);
	localtime_s(&date, &timer);
	strftime(tmp, sizeof(tmp), "%Y%m%d%H%M%S", &date);
	dateTime = tmp;
	// result.YYYYmmDDHHMMSS
	dirName = RESULT_DIR;
	dirName.append(".");
	dirName.append(dateTime);
	// result.YYYYmmDDHHMMSS\thumbnail
	thumbNailDir = dirName;
	thumbNailDir.append(THUMBNAIL_DIR);
	// result.YYYYmmDDHHMMSS\Line.html
	lineHtml = dirName;
	lineHtml.append(LINE_HTML);

	// Check output directory.
	if (0 == stat(dirName.c_str(), &statFile)) {
		// "result" exists.
		fprintf(stderr,"Directory \"%s\" exists.\n", dirName.c_str());
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
			if (0 != stat(dirName.c_str(), &statFile)) {
				// resultフォルダが無い
				_mkdir(dirName.c_str());
			}
			if (0 != stat(thumbNailDir.c_str(), &statFile)) {
				// result.YYYYmmDDHHMMSS\thumbnailフォルダが無い
				_mkdir(thumbNailDir.c_str());
			}
			line_html.open(lineHtml.c_str(), ios::out|ios::binary|ios::trunc);

			PrintHtmlTagStart(&line_html);
			PrintResult(results, &line_html, thumbNailDir);
			PrintHtmlTagEnd(&line_html);
			line_html.flush();
			line_html.close();
		} else { 
			fprintf(stderr, "sqlite3_step result[%d]\n", result);
		}
	}

close:
	// DBを閉じる
	sqlite3_close(db);

	return result;
}
