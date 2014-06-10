/**
* LineHistoryView.exe
*
* Create Date 2014/03/30
*
*
* Update 2014/06/01
*		エラーハンドリングを追加
*		zTimeStampに値が入っていなかった場合にエラーとなる問題を修正
*		サムネイルをサムネイルフィールドに表示するように変更。
*
* Update 2014/06/08
*		サムネイルを内容フィールドに表示するように変更
*
* Update 2014/06/10
*		SELECT文にDATETIME関数を追加。
*		それに伴いいろいろ変更
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include <sqlite3.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <direct.h>

#include "Shlwapi.h"
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
#define RESULT_DIR "result\\"
#define THUMBNAIL_DIR "result\\thumbnail\\"
#define LINE_HTML "result\\Line.html"

/**
* クエリ結果をHTMLタグで整形して出力
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

	// 前回のZCHATの値
	string last_zChat;

	// 1レコード分のデータを格納するオブジェクト
	LineHistory lh;
	vector<LineHistory>::iterator it = array.begin();

	// 1レコードずつ処理
	while( it != array.end() ) {
		lh = *it;

		// 時刻
		zTimeStamp = lh.GetZtimeStamp();

		// アドレス帳上の名前
		zAddress = lh.GetZaddressBookName();

		// トークの区別
		zChat = lh.GetZchat();

		// Content Type
		zContentType = lh.GetZcontentType();
		if ("0" == zContentType) {
			// テキスト
			cType = "テキスト";
		} else if ("1" == zContentType) {
			// 画像
			cType = "画像";
		} else if ("6" == zContentType) {
			// 音声通話
			cType = "音声通話";
		} else if ("7" == zContentType) {
			// スタンプ
			cType = "スタンプ";
		} else if ("100" == zContentType) {
			// スタンプ
			cType = "位置情報";
		} else {
			// 不明な値
			cType = "不明な値[" + zContentType + "]";
		}

		// LINE上の名前
		zName = lh.GetZname();
		if (zAddress.empty() && zName.empty()) {
			mType = "送信";
		} else {
			mType = "受信";
		}

		// 送受信種別
		zOpt = lh.GetZopt();

		// メッセージ
		zText = lh.GetZtext();

		// 画像のサムネイル
		zThumbnail = lh.GetZthumbnail();
		size = lh.GetThumbnailSize();
		if (0 != size) {
			// ファイル名をZIDから作成
			fileName = lh.GetFileName();
			ofstream fout;
			fout.open(THUMBNAIL_DIR + fileName, ios::out|ios::binary|ios::trunc);
			fout.write((char *)zThumbnail, size);
			fout.close();
			// サムネイルのために確保した領域を解放する
			free(zThumbnail);
		}
		if (it == array.begin()) {
			// 最初のレコード
			*line_html << "<table border=\"2\">" << endl;
			*line_html << "<th width=\"150\" nowrap>日時(日本時間)</th>" << endl;
			*line_html << "<th width=\"150\" nowrap>アドレス帳上の名前</th>" << endl;
			*line_html << "<th width=\"150\" nowrap>LINE上の名前</th>" << endl;
			*line_html << "<th nowrap>送受信種別</th>" << endl;
			*line_html << "<th nowrap>内容種別</th>" << endl;
			*line_html << "<th>内容</th>" << endl;
		} else if (last_zChat != zChat) {
			// zChatの値が前回と違う
			*line_html << "</table>" << endl;
			*line_html << "<hr size=\"5\"/>" << endl;
			*line_html << "<table border=\"2\">" << endl;
			// ヘッダー出力
			*line_html << "<th width=\"150\" nowrap>日時(日本時間)</th>" << endl;
			*line_html << "<th width=\"150\" nowrap>アドレス帳上の名前</th>" << endl;
			*line_html << "<th width=\"150\" nowrap>LINE上の名前</th>" << endl;
			*line_html << "<th nowrap>送受信種別</th>" << endl;
			*line_html << "<th nowrap>内容種別</th>" << endl;
			*line_html << "<th>内容</th>" << endl;
		}

		// zChatを記録
		last_zChat = zChat;

		// 出力
		*line_html << "<tr>" << endl;
		*line_html << "<td nowrap>" << zTimeStamp << "</td>" << endl;
		*line_html << "<td nowrap>" << zAddress << "</td>" << endl;
		*line_html << "<td nowrap>" << zName << "</td>" << endl;
		*line_html << "<td nowrap>" << mType << "</td>" << endl;
		*line_html << "<td nowrap>" << cType << "</td>" << endl;
		if (EMPTY_STRING == fileName) {
			// 画像無し
			*line_html << "<td><div>" << zText << "</div></td>" << endl;
		} else {
			// 画像有り
			*line_html << "<td><div>" << zText << "</div>" << endl;
			*line_html << "<img border=\"0\" src=./thumbnail/" << fileName << " /></td>" << endl;
		}
		*line_html << "</tr>" << endl;

		// イテレータを進める
		it++;

		// 変数をクリア
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
	*line_html << HTML_TAG_END << endl;
	*line_html << BODY_TAG_END << endl;
}
/**
*
*
*
*/
int main(int argc, char * argv[])
{
	int result = -1;
	sqlite3 *db = (sqlite3 *)0;
	//	const char *sql = "SELECT ZMESSAGE.Z_OPT, ZMESSAGE.ZCONTENTTYPE, ZMESSAGE.ZTIMESTAMP, ZMESSAGE.ZCHAT, ZMESSAGE.ZID, ZMESSAGE.ZTEXT, ZMESSAGE.ZTHUMBNAIL, ZUSER.ZADDRESSBOOKNAME, ZUSER.ZNAME FROM ZMESSAGE LEFT JOIN ZUSER ON ZMESSAGE.ZSENDER = ZUSER.Z_PK ORDER BY ZMESSAGE.ZCHAT, ZMESSAGE.ZTIMESTAMP;";
	const char *sql = "SELECT ZMESSAGE.Z_OPT, ZMESSAGE.ZCONTENTTYPE"
		", DATETIME(ZMESSAGE.ZTIMESTAMP/1000, 'unixepoch', 'localtime')"
		", ZMESSAGE.ZCHAT, ZMESSAGE.ZID, ZMESSAGE.ZTEXT"
		", ZMESSAGE.ZTHUMBNAIL, ZUSER.ZADDRESSBOOKNAME"
		", ZUSER.ZNAME"
		" FROM ZMESSAGE LEFT JOIN ZUSER ON ZMESSAGE.ZSENDER = ZUSER.Z_PK"
		" ORDER BY ZMESSAGE.ZCHAT, ZMESSAGE.ZTIMESTAMP;";

	sqlite3_stmt *stmt = (sqlite3_stmt *)0;
	char *errMsg = (char *)0;
	// サムネイルを格納
	int size = 0;
	void *buf = (char *)0;

	// クエリ結果を格納する配列
	vector<LineHistory> array;

	// クエリ結果1レコード分を格納するオブジェクト
	LineHistory lh;

	string tmp;
	const unsigned char * zid = (unsigned char *)0;
	const unsigned char * value = (unsigned char *)0;
	const void * zThumbnail = (void *)0;

	// 出力ファイル
	ofstream line_html;

	// 引数をチェック
	if (1 == argc) {
		fprintf(stderr, "Usage: %s <talk.sqlite>", argv[0]);
		exit(-1);
	} else if (FALSE == PathFileExists(argv[1])) {
		// ファイルの存在をチェック
		fprintf(stderr, "File not exists [%s]", argv[1]);
		exit(-2);
	}

	// DBファイルをオープン
	result = sqlite3_open(argv[1], &db);

	if (SQLITE_OK != result) {
		fprintf(stderr, "sqlite3_open result[%d]\n", result);
		goto close;
	} else {
		// stmt生成
		sqlite3_prepare(db, sql, strlen(sql), &stmt, NULL);

		//stmt内のバッファをクリア
		sqlite3_reset(stmt);

		/* SELECT文の実行 */
		while (SQLITE_ROW == (result = sqlite3_step(stmt))) {
			lh = LineHistory();
			tmp = EMPTY_STRING;
			// const unsiged char *をstringにキャストし、
			// オブジェクトに詰め詰めする。
			// レコードの最初のフィールドの値を取得
			// Z_OPT
			value = sqlite3_column_text(stmt, 0);
			if ((unsigned char *)0 == value) {
				lh.SetZopt(EMPTY_STRING);
			} else {
				tmp = (reinterpret_cast <char const *> (value));
				lh.SetZopt(tmp);
			}

			// レコードの2番目のフィールドの値を取得
			// ZCONTENTTYPE
			value = sqlite3_column_text(stmt, 1);
			if ((unsigned char *)0 == value) {
				lh.SetZcontentType(EMPTY_STRING);
			} else {
				tmp = (reinterpret_cast <char const *> (value));
				lh.SetZcontentType(tmp);
			}

			// レコードの3番目のフィールドの値を取得
			// ZTIMESTAMP
			value = sqlite3_column_text(stmt, 2);
			if ((unsigned char *)0 == value) {
				lh.SetZtimeStamp(EMPTY_STRING);
			} else {
				tmp = (reinterpret_cast <char const *> (value));
				lh.SetZtimeStamp(tmp);
			}

			// レコードの4番目のフィールドの値を取得
			// ZCHAT
			value = sqlite3_column_text(stmt, 3);
			if ((unsigned char *)0 == value) {
				lh.SetZchat(EMPTY_STRING);
			} else {
				tmp = (reinterpret_cast <char const *> (value));
				lh.SetZchat(tmp);
			}

			// レコードの5番目のフィールドの値を取得
			// ZIDはファイル名に使用する
			zid = sqlite3_column_text(stmt, 4);
			if ((unsigned char *)0 == zid) {
				lh.SetZid(EMPTY_STRING);
			} else {
				tmp = (reinterpret_cast <char const *> (zid));
				lh.SetZid(tmp);
				// サムネイルのファイル名を格納
				lh.SetFileName(tmp + ".jpg");
			}

			// レコードの6番目のフィールドの値を取得
			// ZTEXT
			value = sqlite3_column_text(stmt, 5);
			if ((unsigned char *)0 == value) {
				lh.SetZtext(EMPTY_STRING);
			} else {
				tmp = (reinterpret_cast <char const *> (value));
				lh.SetZtext(tmp);
			}

			// レコードの7番目のフィールドの値を取得
			// ZTHUMBNAIL
			size = sqlite3_column_bytes(stmt, 6);
			if (0 == size) {
				lh.SetZthumbnail(EMPTY_STRING);
			} else {
				// サムネイルのデータ格納用バッファを確保
				buf = calloc(size, sizeof(char));
				// サムネイルのデータをコピー
				zThumbnail = (char *)sqlite3_column_blob(stmt, 6);
				memcpy(buf, zThumbnail, size);
				lh.SetZthumbnail(buf);
			}
			// サムネイルのサイズを格納
			lh.SetThumbnailSize(size);

			// レコードの8番目のフィールドの値を取得
			// ZADDRESSBOOKNAME
			value = sqlite3_column_text(stmt, 7);
			if ((unsigned char *)0 == value) {
				lh.SetZaddressBookName(EMPTY_STRING);
			} else {
				tmp = (reinterpret_cast <char const *> (value));
				lh.SetZaddressBookName(tmp);
			}

			// レコードの9番目のフィールドの値を取得
			// ZNAME
			value = sqlite3_column_text(stmt, 8);
			if ((unsigned char *)0 == value) {
				lh.SetZname(EMPTY_STRING);
			} else {
				tmp = (reinterpret_cast <char const *> (value));
				lh.SetZname(tmp);
			}

			// オブジェクト配列のケツに追加。
			array.push_back(lh);
			lh.~LineHistory();
		}

		if (SQLITE_DONE == result) {
			// 出力フォルダ作成
			if (!PathIsDirectory(RESULT_DIR)) {
				// resultフォルダが無い
				_mkdir(RESULT_DIR);
			}
			if (!PathIsDirectory(THUMBNAIL_DIR)) {
				// result\thumbnailフォルダが無い
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
	// エラーメッセージ用バッファを解放
	sqlite3_free(errMsg);

	// stmtを解放
	sqlite3_finalize(stmt);

close:
	// DBを閉じる
	sqlite3_close(db);

	return result;
}

