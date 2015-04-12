#include "sheetsapirequest.h"

Sheets::ListFilesRequest::ListFilesRequest()
    : GoogleAPIRequest(QUrl("https://spreadsheets.google.com/feeds/spreadsheets/private/full"), "GET")
{

}
