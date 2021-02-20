#ifndef SNMPCOMANDS_H
#define SNMPCOMANDS_H

#include <QString>

class SNMPcomands
{
public:
    SNMPcomands();
    SNMPcomands(QString name_,
                QString type_,
                QString access_,
                QString comments_);

    QString GetName() {return name_;}
    QString GetType() {return type_;}
    QString GetAcess() {return access_;}
    QString GetComments() {return comments_;}

    void SetName(QString name) {name_ = name;}
    void SetType(QString type) {type_ = type;}
    void SetAccess(QString access) {access_ =access;}
    void SetComments(QString comments) {comments_ = comments;}
private:
    QString name_;
    QString type_;
    QString access_;
    QString comments_;
};

#endif // SNMPCOMANDS_H
