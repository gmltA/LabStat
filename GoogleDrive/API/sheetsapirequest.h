#ifndef SHEETSAPIREQUEST_H
#define SHEETSAPIREQUEST_H

#include "apirequest.h"

/*!
 * \brief Sheets namespace holds requests related only to Sheets API on Drive.
 *
 * These requests has different URLs and result format. That's why extraction to separate namespace took place.
 */
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
