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

    nettestStreamID->setValidator(new QIntValidator(0, 0xffff, this));
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
    nettestTimeStamp->setText(
        proto->fieldData(
            NetTestProtocol::nettest_timestamp,
            AbstractProtocol::FieldValue
        ).toString());
    nettestSeqNumber->setText(
        proto->fieldData(
            NetTestProtocol::nettest_seqnumber,
            AbstractProtocol::FieldValue
        ).toString());

    cmbTimeStampMode->setCurrentIndex(
            proto->fieldData(
                NetTestProtocol::nettest_timestampMode,
                AbstractProtocol::FieldValue
                ).toUInt());
    cmbSeqNumberMode->setCurrentIndex(
            proto->fieldData(
                NetTestProtocol::nettest_seqnumberMode,
                AbstractProtocol::FieldValue
                ).toUInt());
}

/*!
TODO: Edit this function to store each field's data from the config Widget

See AbstractProtocolConfigForm::storeWidget() for more info
*/
void NetTestConfigForm::storeWidget(AbstractProtocol *proto)
{
    proto->setFieldData(
        NetTestProtocol::nettest_timestamp,
        nettestTimeStamp->text());
    proto->setFieldData(
        NetTestProtocol::nettest_seqnumber,
        nettestSeqNumber->text());

    proto->setFieldData(
            NetTestProtocol::nettest_timestampMode,
            cmbTimeStampMode->currentIndex());
    proto->setFieldData(
            NetTestProtocol::nettest_seqnumberMode,
            cmbSeqNumberMode->currentIndex());

}


void NetTestConfigForm::on_cmbSeqNumberMode_currentIndexChanged(int index)
{
    switch(index)
    {
        case OstProto::NetTest::e_sn_fixed:
            nettestSeqNumber->setEnabled(true);
            break;
        case OstProto::NetTest::e_sn_inc:
            nettestSeqNumber->setDisabled(true);
            break;
        default:
            qWarning("Unhandled/Unknown PatternMode = %d",index);
    }
}

void NetTestConfigForm::on_cmbTimeStampMode_currentIndexChanged(int index)
{
    switch(index)
    {
        case OstProto::NetTest::e_ts_fixed:
            nettestTimeStamp->setEnabled(true);
            break;
        case OstProto::NetTest::e_ts_systimer:
            nettestTimeStamp->setDisabled(true);
            break;
        default:
            qWarning("Unhandled/Unknown PatternMode = %d",index);
    }
}
