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
}

#endif // SHEETSAPIREQUEST_H
