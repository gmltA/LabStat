#ifndef SHEETSAPIREQUEST_H
#define SHEETSAPIREQUEST_H

#include "apirequest.h"

namespace Sheets
{
class ListFilesRequest : public GoogleAPIRequest
{
    public:
        ListFilesRequest();
};

class GetFileRequest : public GoogleAPIRequest
{
    public:
        GetFileRequest(SpreadSheet file);
};

class EditRowRequest : public GoogleAPIRequest
{
    public:
        EditRowRequest(QUrl url, QByteArray rowData);
};
}

#endif // SHEETSAPIREQUEST_H
