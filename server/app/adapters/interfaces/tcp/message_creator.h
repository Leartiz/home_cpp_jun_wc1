#ifndef MESSAGE_CREATOR_H
#define MESSAGE_CREATOR_H

#include "dto/response.h"

namespace lez::adapters::interfaces::tcp
{

class Message_creator final
{
public:
    Message_creator();
    std::string create(dto::Response::Sp response) const;
};

}

#endif // MESSAGE_CREATOR_H
