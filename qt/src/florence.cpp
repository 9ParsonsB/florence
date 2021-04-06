//    This file is part of Florence Virtual Keyboard for Qt.
//    Copyright (C) 2011  iKare Corporation
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <QtPlugin>
#include <QFile>
#include <QDomElement>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include "florence.h"

Florence::Florence( QWidget *parent )
    : QGraphicsView( parent )
{
    this->focusKey = nullptr;
    this->moving = false;

    this->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    this->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    this->setStyleSheet( "background: transparent; border-style: none;" );
    this->setContentsMargins(0, 0, 0, 0);
    this->graphicsScene = new QGraphicsScene( this );
    this->setScene( this->graphicsScene );

    this->setMouseTracking( true );
    this->autoRepeatTimer = new QTimer( this );
    connect(this->autoRepeatTimer, SIGNAL(timeout()), this, SLOT(repeat()));

    this->settings = new Settings();
    this->setLayout( ":/florence.xml" );
}

Florence::~Florence()
{
    delete this->settings;
    foreach ( QGraphicsItem *it, this->scene()->items() ) {
        this->scene()->removeItem( it );
        Key *k = static_cast<Key *>(it);
        delete k;
    }
    delete this->graphicsScene;
}

bool Florence::getKeyboardSize( QDomElement *keyboard, qreal *width, qreal *height )
{
    bool ok;

    if (keyboard->isNull()) return false;
    QDomElement w = keyboard->firstChildElement("width");
    QDomElement h = keyboard->firstChildElement("height");
    if ( w.isNull() || h.isNull() ) return false;
    *width = w.text().toDouble(&ok);
    if (!ok) return false;
    *height = h.text().toDouble(&ok);
    if (!ok) return false;

    return true;
}

bool Florence::loadKeyboard( QDomElement *keyboard, qreal xOffset, qreal yOffset )
{
    QDomElement key = keyboard->firstChildElement("key");
    while ( !key.isNull() ) {
        Key *k = new Key( key, this->settings, xOffset, yOffset );
        k->setStyle( this->settings->getStyle() );
        this->connect( k, SIGNAL(inputText(Symbol::symbol_role,QString)),
                       SLOT(inputText(Symbol::symbol_role, QString)) );
        this->connect( k, SIGNAL(actionTrigger(QString)),
                       SIGNAL(actionTrigger(QString)) );
        this->connect( k, SIGNAL(latchKey(Key*)), SLOT(latchKey(Key*)) );
        this->connect( k, SIGNAL(unlatchKey(Key*)), SLOT(unlatchKey(Key*)) );
        this->connect( k, SIGNAL(lockKey(Key*)), SLOT(lockKey(Key*)) );
        this->connect( k, SIGNAL(unlockKey(Key*)), SLOT(unlockKey(Key*)) );
        this->connect( k, SIGNAL(unlatchAll()), SLOT(unlatchAll()) );
        this->connect( k, SIGNAL(keyPressed(quint8)), SLOT(keyPressedSlot(quint8)) );
        this->connect( k, SIGNAL(keyReleased(quint8)), SLOT(keyReleasedSlot(quint8)) );
        this->scene()->addItem(k);
        key = key.nextSiblingElement("key");
    }

    return true;
}

bool Florence::setLayout( QString file )
{
    QDomDocument layout;
    QFile f(file);

    if ( !f.open( QIODevice::ReadOnly ) ) return false;
    if ( !layout.setContent(&f) ) {
        f.close();
        return false;
    }
    f.close();

    foreach ( QGraphicsItem *it, this->scene()->items() ) {
        this->scene()->removeItem( it );
        Key *k = static_cast<Key *>(it);
        delete k;
    }

    QDomElement root = layout.documentElement();
    QDomElement keyboard = root.firstChildElement("keyboard");

    if (!this->getKeyboardSize( &keyboard, &this->sceneWidth, &this->sceneHeight)) {
        return false;
    }

    qreal width;
    qreal height;
    qreal xOffset = 0.0;
    qreal yOffset = 0.0;

    QList<QDomElement *> left;
    QList<QDomElement *> right;
    QList<QDomElement *> top;
    QList<QDomElement *> bottom;

    QDomElement extension = root.firstChildElement("extension");
    while ( !extension.isNull() ) {
        QDomElement placement = extension.firstChildElement("placement");
        QString pos = placement.text();
        if (pos == QStringLiteral("left")) {
            QDomElement *extKeyboard = new QDomElement(extension.firstChildElement("keyboard"));
            if (this->getKeyboardSize( extKeyboard, &width, &height)) {
                left << extKeyboard;
                this->sceneWidth += width;
                xOffset += width;
            }
        } else if (pos == QStringLiteral("right")) {
            QDomElement *extKeyboard = new QDomElement(extension.firstChildElement("keyboard"));
            if (this->getKeyboardSize( extKeyboard, &width, &height)) {
                right << extKeyboard;
                this->sceneWidth += width;
            }
        } else if (pos == QStringLiteral("top")) {
            QDomElement *extKeyboard = new QDomElement(extension.firstChildElement("keyboard"));
            if (this->getKeyboardSize( extKeyboard, &width, &height)) {
                top << extKeyboard;
                this->sceneHeight += height;
                yOffset += height;
            }
        } else if (pos == QStringLiteral("bottom")) {
            QDomElement *extKeyboard = new QDomElement(extension.firstChildElement("keyboard"));
            if (this->getKeyboardSize( extKeyboard, &width, &height)) {
                bottom << extKeyboard;
                this->sceneHeight += height;
            }
        }
        extension = extension.nextSiblingElement("extension");
    }

    qreal x = 0.0;
    qreal y = 0.0;
    for (int i = 0; i < left.size(); i++) {
        QDomElement *extKeyboard = left.at(i);
        if (this->getKeyboardSize( extKeyboard, &width, &height)) {
            if (!this->loadKeyboard( extKeyboard, x, yOffset )) {
                return false;
            }
            x += width;
        }
        delete extKeyboard;
    }
    for (int i = 0; i < top.size(); i++) {
        QDomElement *extKeyboard = top.at(i);
        if (this->getKeyboardSize( extKeyboard, &width, &height)) {
            if (!this->loadKeyboard( extKeyboard, xOffset, y )) {
                return false;
            }
            y += height;
        }
        delete extKeyboard;
    }
    if (this->getKeyboardSize( &keyboard, &width, &height)) {
        if (!this->loadKeyboard( &keyboard, xOffset, yOffset )) {
            return false;
        }
        x += width;
        y += height;
    }
    for (int i = 0; i < right.size(); i++) {
        QDomElement *extKeyboard = right.at(i);
        if (this->getKeyboardSize( extKeyboard, &width, &height)) {
            if (!this->loadKeyboard( extKeyboard, x, yOffset )) {
                return false;
            }
            x += width;
        }
        delete extKeyboard;
    }
    for (int i = 0; i < bottom.size(); i++) {
        QDomElement *extKeyboard = bottom.at(i);
        if (this->getKeyboardSize( extKeyboard, &width, &height)) {
            if (!this->loadKeyboard( extKeyboard, xOffset, y )) {
                return false;
            }
            y += height;
        }
        delete extKeyboard;
    }

    return true;
}

bool Florence::setStyle( QString file )
{
    return this->settings->setStyle( file );
}

void Florence::setColor( enum StyleItem::style_colours color, QString value )
{
    this->settings->setColor( color, value );
}

void Florence::setOpacity( qreal opacity )
{
    this->settings->setOpacity( opacity );
}

void Florence::setFont( QString font )
{
    this->settings->setFont( font );
}

void Florence::setFontSize( quint8 size )
{
    this->settings->setFontSize( size );
}

void Florence::setKeymap( Keymap *map )
{
    this->settings->setKeymap( map );
}

bool Florence::setKeymap( QString file )
{
    return this->settings->setKeymap( file );
}

void Florence::resizeEvent( QResizeEvent *event )
{
    QTransform matrix;
    matrix.scale(this->width()/this->scene()->width(), this->height()/this->scene()->height());
    this->setTransform( matrix, false );
    QGraphicsView::resizeEvent( event );
}

void Florence::mousePressEvent( QMouseEvent *event )
{
    this->mouseMoveEvent( event );
    if (this->focusKey->getAction() == QStringLiteral("move")) {
        QPoint p = this->mapToGlobal(QPoint(0,0));
        this->startX = event->globalX() - p.x();
        this->startY = event->globalY() - p.y();
        this->moving = true;
    }
}

void Florence::mouseDoubleClickEvent( QMouseEvent *event )
{
    this->mouseMoveEvent( event );
}

void Florence::mouseMoveEvent( QMouseEvent *event )
{
    if (this->moving) {
        QPoint p = this->mapToGlobal(QPoint(0,0));
        int x = event->globalX() - p.x();
        int y = event->globalY() - p.y();
        emit actionMove(x - this->startX, y - this->startY);
        this->focusKey->setPressed();
    } else if ( event->buttons() > Qt::NoButton )  {
        Key *k = static_cast<Key *>( this->scene()->itemAt( this->mapToScene( event->pos() ), QGraphicsView::transform() ) );
        if ( k != this->focusKey ) {
            this->autoRepeatTimer->stop();
            if ( this->focusKey ) {
                this->focusKey->hoverLeaveEvent();
            }
            this->focusKey = k;
            if ( k ) {
                k->hoverEnterEvent();
                if (k->getCode() > 0) {
                    this->autoRepeatTimer->start( 1000 );
                }
            }
        }
    }
    QGraphicsView::mouseMoveEvent( event );
}

void Florence::repeat()
{
    if ( this->focusKey ) this->focusKey->press();
    this->autoRepeatTimer->stop();
    this->autoRepeatTimer->start(100);
}

void Florence::leaveEvent( QEvent *event )
{
    if (!this->moving) {
        this->autoRepeatTimer->stop();
        if ( this->focusKey ) this->focusKey->hoverLeaveEvent();
        this->focusKey = nullptr;
    }
    QGraphicsView::leaveEvent( event );
}

void Florence::mouseReleaseEvent( QMouseEvent *event )
{
    //Key *k = static_cast<Key *>( this->scene()->itemAt( this->mapToScene( event->pos() ) ) );
    if ( this->focusKey ) this->focusKey->mouseReleaseEvent();
    if ( this->autoRepeatTimer->isActive() ) this->autoRepeatTimer->stop();
    this->focusKey = nullptr;
    if (this->moving) {
        this->releaseMouse();
        QPoint p = this->mapToGlobal(QPoint(0,0));
        int x = event->globalX() - p.x();
        int y = event->globalY() - p.y();
        emit actionMove(x - this->startX, y - this->startY);
    }
    this->moving = false;
    QGraphicsView::mouseReleaseEvent( event );
}

void Florence::latchKey( Key *key )
{
    this->latchedKeys.append( key );
}

void Florence::unlatchKey( Key *key )
{
    this->latchedKeys.remove( this->latchedKeys.indexOf( key ) );
}

void Florence::lockKey( Key *key )
{
    this->lockedKeys.append( key );
    if ( key->isLocker() ) {
        emit keyPressed( key->getCode() );
        emit keyReleased( key->getCode() );
    }
}

void Florence::unlockKey( Key *key )
{
    this->lockedKeys.remove( this->lockedKeys.indexOf( key ) );
    if ( key->isLocker() ) {
        emit keyPressed( key->getCode() );
        emit keyReleased( key->getCode() );
    }
}

void Florence::unlatchAll()
{
    bool unlatched = false;
    foreach ( Key *k, this->latchedKeys ) {
        unlatched = k->unlatch() || unlatched;
        emit keyReleased( k->getCode() );
    }

    if ( unlatched ) foreach ( QGraphicsItem *it, this->scene()->items() ) {
        Key *k = static_cast<Key *>(it);
        k->update();
    }

    this->latchedKeys.clear();
}

void Florence::inputText( enum Symbol::symbol_role role, QString text )
{
    switch ( role ) {
        case Symbol::SYMBOL_TEXT:
            emit inputText( text );
            break;
        case Symbol::SYMBOL_BACKSPACE:
            emit backspace();
            break;
        case Symbol::SYMBOL_RETURN:
            emit enter();
            break;
        case Symbol::SYMBOL_TAB:
            emit tab();
            break;
        case Symbol::SYMBOL_LEFTTAB:
            emit leftTab();
            break;
        default:
            break;
    }
}

void Florence::keyPressedSlot( quint8 code )
{
    foreach ( Key *k, this->latchedKeys ) {
        emit keyPressed( k->getCode() );
    }
    foreach ( Key *k, this->lockedKeys ) {
        if ( !k->isLocker() ) emit keyPressed( k->getCode() );
    }
    emit keyPressed( code );
}

void Florence::keyReleasedSlot( quint8 code )
{
    emit keyReleased(code);
    foreach ( Key *k, this->latchedKeys ) {
        emit keyReleased( k->getCode() );
    }
    foreach ( Key *k, this->lockedKeys ) {
        if ( !k->isLocker() ) emit keyReleased( k->getCode() );
    }
}

Settings *Florence::getSettings()
{
    return this->settings;
}
