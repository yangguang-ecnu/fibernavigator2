/*
 * Propertystring.cpp
 *
 * Created on: Jan 17, 2013
 * @author Ralph Schurade
 */
#include "propertystring.h"

#include "../../gui/widgets/controls/editwithlabel.h"

PropertyString::PropertyString( QString name, QString value ) :
    Property( name ),
    m_value( value )
{
    m_widget = new EditWithLabel( m_name );
    m_widget->setText( value );
    connect( m_widget, SIGNAL( valueChanged( QString, int ) ), this, SLOT( widgetChanged( QString, int ) ) );
}

PropertyString::~PropertyString()
{
}

QWidget* PropertyString::getWidget()
{
    return m_widget;
}

QVariant PropertyString::getValue()
{
    return m_value;
}

void PropertyString::setValue( QVariant value )
{
    m_value = value.toString();
}

void PropertyString::widgetChanged( QString value, int id )
{
    m_value = value;
    emit( valueChanged() );
}
