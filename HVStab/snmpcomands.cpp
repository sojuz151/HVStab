#include "snmpcomands.h"

SNMPcomands::SNMPcomands()
{

}

SNMPcomands::SNMPcomands(QString name_, QString type_, QString access_, QString comments_)
{
    SetName(name_);
    SetType(type_);
    SetAccess(access_);
    SetComments(comments_);
}
