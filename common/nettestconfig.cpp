/*
Copyright (C) 2010, 2014 Srivats P.

This file is part of "Ostinato"

This is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>
*/

#include "nettestconfig.h"
#include "nettest.h"

NetTestConfigForm::NetTestConfigForm(QWidget *parent)
    : AbstractProtocolConfigForm(parent)
{
    setupUi(this);
}

NetTestConfigForm::~NetTestConfigForm()
{
}

NetTestConfigForm* NetTestConfigForm::createInstance()
{
    return new NetTestConfigForm;
}

/*!
TODO: Edit this function to load each field's data into the config Widget

See AbstractProtocolConfigForm::loadWidget() for more info
*/
void NetTestConfigForm::loadWidget(AbstractProtocol *proto)
{
    // TODO nettestTimeStamp
    nettestTimeStamp->setText(
        proto->fieldData(
            NetTestProtocol::nettest_timestamp,
            AbstractProtocol::FieldValue
        ).toString());
    // TODO nettestSeqNumber
    nettestSeqNumber->setText(
        proto->fieldData(
            NetTestProtocol::nettest_seqnumber,
            AbstractProtocol::FieldValue
        ).toString());
}

/*!
TODO: Edit this function to store each field's data from the config Widget

See AbstractProtocolConfigForm::storeWidget() for more info
*/
void NetTestConfigForm::storeWidget(AbstractProtocol *proto)
{
    // TODO nettestTimeStamp
    proto->setFieldData(
        NetTestProtocol::nettest_timestamp,
        nettestTimeStamp->text());
    // TODO nettestSeqNumber
    proto->setFieldData(
        NetTestProtocol::nettest_seqnumber,
        nettestSeqNumber->text());
}

