//    This file is part of Florence Virtual Keyboard, QT version.
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

#include "symbol.h"
#include "styleshape.h"
#include "style.h"
#include "settings.h"
#include <QDebug>

Symbol::Symbol( QString name, Settings *settings )
{
    this->name = name;
    this->role = SYMBOL_TEXT;
    if ( this->name == "BackSpace" ) {
        this->role = SYMBOL_BACKSPACE;
    } else if ( this->name == "Return" ) {
        this->role = SYMBOL_RETURN;
    } else if ( this->name == "Tab" ) {
        this->role = SYMBOL_TAB;
    } else if ( this->name == "ISO_Left_Tab" ) {
        this->role = SYMBOL_LEFTTAB;
    }

    this->renderer = nullptr;
    this->settings = settings;
    if (this->settings && this->settings->getStyle())
        this->renderer = this->settings->getStyle()->getSymbol( this->name );;
    this->connect( this->settings, SIGNAL(styleChanged(Style*)), SLOT(setStyle(Style*)) );
}

Symbol::Symbol( QDomElement el, Settings *settings )
{
    QDomElement name = el.firstChildElement("name");
    if ( name.isNull() ) {
        this->name = el.text();
        this->role = SYMBOL_TEXT;
    } else {
        this->name = name.text();
        if ( this->name == "BackSpace" ) {
            this->role = SYMBOL_BACKSPACE;
        } else if ( this->name == "Return" ) {
            this->role = SYMBOL_RETURN;
        } else if ( this->name == "Tab" ) {
            this->role = SYMBOL_TAB;
        } else if ( this->name == "ISO_Left_Tab" ) {
            this->role = SYMBOL_LEFTTAB;
        } else {
            this->role = SYMBOL_DEAD;
        }
    }
    this->renderer = nullptr;
    this->settings = settings;
    this->connect( this->settings, SIGNAL(styleChanged(Style*)), SLOT(setStyle(Style*)) );
}

void Symbol::setStyle( Style *style )
{
    this->renderer = style->getSymbol( this->name );
}

QString Symbol::getName()
{
    return this->name;
}

enum Symbol::symbol_role Symbol::getRole()
{
    return this->role;
}

void Symbol::paint( QPainter *painter, QRectF &bounds, bool hovered )
{
    if ( this->renderer && this->renderer->getRenderer() ) {
        this->renderer->paint( painter, bounds );
    } else {
        QString text;
        if ( this->renderer ) text = this->renderer->getLabel();
        else if ( this->role == SYMBOL_TEXT ) text = this->getName();
        else return;

        qreal z = 1.0;
        if ( hovered ) z = 1.2;

        QPainterPath textPath;
        QFont font( this->settings->getFont(), this->settings->getFontSize() );
        textPath.addText(0, 0, font, text);

        QFontMetrics fm(font);
        qreal height = fm.descent() * 0.1 * z;
        qreal yoffset = ( bounds.height() + height ) * 0.5;
        qreal width = fm.width(text) * 0.05 * z;
        qreal xoffset = ( bounds.width() - width ) * 0.5;
        painter->translate( bounds.x() + xoffset,
                            bounds.y() + yoffset );
        painter->scale( z*0.05, z*0.05 );

        painter->setRenderHint( QPainter::Antialiasing );
        QPen p( QColor( this->settings->getColor( StyleItem::STYLE_TEXT_OUTLINE_COLOR ) ) );
        p.setWidth( 2 );
        painter->strokePath( textPath, p );
        painter->fillPath( textPath, QBrush( QColor( this->settings->getColor( StyleItem::STYLE_TEXT_COLOR ) ) ) );
    }
}

ModifiedSymbol::ModifiedSymbol( QDomElement el, Settings *settings )
    : Symbol( el, settings )
{
    this->modifier = static_cast<quint8>(el.attribute("mod").toInt());
}

ModifiedSymbol::ModifiedSymbol( QString name, quint8 modifier, Settings *settings )
    : Symbol( name, settings )
{
    this->modifier = modifier;
}

quint8 ModifiedSymbol::getModifier()
{
    return this->modifier;
}
