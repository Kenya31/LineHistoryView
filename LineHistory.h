#include <map>
#include <string>

#include "Record.h"

using namespace std;

/**
 * Inherit Record class.
 */
class LineHistory : public Record
{
public:
	LineHistory(void);
	virtual ~LineHistory(void);

	void LineHistory::SetZthumbnail(void *value);
	void * LineHistory::GetZthumbnail(void);

	void LineHistory::SetThumbnailSize(int value);
	int LineHistory::GetThumbnailSize(void);

	void LineHistory::SetThumbFileName(string value);
	string LineHistory::GetThumbFileName(void);

private:
	void *zThumbnail;
	int thumbSize;
	string thumbFileName;
};
