#include "sheetsapirequest.h"

Sheets::ListFilesRequest::ListFilesRequest()
    : GoogleAPIRequest(QUrl("https://spreadsheets.google.com/feeds/spreadsheets/private/full"), "GET")
{

}

Sheets::GetFileRequest::GetFileRequest(SpreadSheet file)
    : GoogleAPIRequest(QUrl("https://spreadsheets.google.com/feeds/worksheets/" + file.getId() + "/private/full"), "GET")
{

}
