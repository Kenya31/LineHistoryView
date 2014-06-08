#include <string>

using namespace std;

class LineHistory
{
public:
	LineHistory(void);
	~LineHistory(void);

	void LineHistory::SetZopt(string value);
	string LineHistory::GetZopt(void);

	void LineHistory::SetZcontentType(string value);
	string LineHistory::GetZcontentType(void);

	void LineHistory::SetZtimeStamp(string value);
	string LineHistory::GetZtimeStamp(void);

	void LineHistory::SetZchat(string value);
	string LineHistory::GetZchat(void);

	void LineHistory::SetZid(string value);
	string LineHistory::GetZid(void);

	void LineHistory::SetZtext(string value);
	string LineHistory::GetZtext(void);

	void LineHistory::SetZthumbnail(void * value);
	void * LineHistory::GetZthumbnail(void);

	void LineHistory::SetThumbnailSize(int value);
	int LineHistory::GetThumbnailSize(void);

	void LineHistory::SetZaddressBookName(string value);
	string LineHistory::GetZaddressBookName(void);

	void LineHistory::SetZname(string value);
	string LineHistory::GetZname(void);

	void LineHistory::SetFileName(string value);
	string LineHistory::GetFileName(void);

private:
	string zOpt;
	string zContentType;
	string zTimeStamp;
	string zChat;
	string zId;
	string zText;
	void * zThumbnail;
	int thumbnailSize;
	string zAddressBookName;
	string zName;
	string fileName;
};
