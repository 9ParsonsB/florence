#include "manager.h"

Manager::Manager(QObject *parent) : QObject(parent)
{

}

void Manager::action( QString action )
{
    qDebug() << action;
}
