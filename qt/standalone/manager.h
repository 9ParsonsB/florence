#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include <QDebug>

class Manager : public QObject
{
    Q_OBJECT
public:
    explicit Manager(QObject *parent = 0);

signals:

public slots:
    void action( QString action );
};

#endif // MANAGER_H
