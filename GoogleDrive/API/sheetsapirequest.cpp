#include "sheetsapirequest.h"

Sheets::ListFilesRequest::ListFilesRequest()
    : GoogleAPIRequest(QUrl("https://spreadsheets.google.com/feeds/spreadsheets/private/full"), "GET")
{

}

Sheets::GetFileRequest::GetFileRequest(SpreadSheet file)
    : GoogleAPIRequest(QUrl("https://spreadsheets.google.com/feeds/worksheets/" + file.getId() + "/private/full"), "GET")
{

}

Sheets::EditRowRequest::EditRowRequest(QUrl url, QByteArray rowData)
    : GoogleAPIRequest(url, "PUT", rowData)
{
    setRawHeader("Content-Type", QString("application/atom+xml").toLatin1());
}
