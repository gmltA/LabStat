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
}

#endif // SHEETSAPIREQUEST_H
