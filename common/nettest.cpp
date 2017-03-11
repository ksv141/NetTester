#include "nettest.h"

NetTestProtocol::NetTestProtocol(StreamBase *stream, AbstractProtocol *parent)
    : AbstractProtocol(stream, parent)
{
}

NetTestProtocol::~NetTestProtocol()
{
}

AbstractProtocol* NetTestProtocol::createInstance(StreamBase *stream,
    AbstractProtocol *parent)
{
    return new NetTestProtocol(stream, parent);
}

quint32 NetTestProtocol::protocolNumber() const
{
    return OstProto::Protocol::kNetTestFieldNumber;
}

void NetTestProtocol::protoDataCopyInto(OstProto::Protocol &protocol) const
{
    protocol.MutableExtension(OstProto::nettest)->CopyFrom(data);
    protocol.mutable_protocol_id()->set_id(protocolNumber());
}

void NetTestProtocol::protoDataCopyFrom(const OstProto::Protocol &protocol)
{
    if (protocol.protocol_id().id() == protocolNumber() &&
            protocol.HasExtension(OstProto::nettest))
        data.MergeFrom(protocol.GetExtension(OstProto::nettest));
}

QString NetTestProtocol::name() const
{
    return QString("NetTest Protocol");
}

QString NetTestProtocol::shortName() const
{
    return QString("NETTEST");
}

int NetTestProtocol::fieldCount() const
{
    return nettest_fieldCount;
}

/*!
  TODO Edit this function to return the appropriate flags for each field \n

  See AbstractProtocol::FieldFlags for more info
*/
AbstractProtocol::FieldFlags NetTestProtocol::fieldFlags(int index) const
{
    AbstractProtocol::FieldFlags flags;

    flags = AbstractProtocol::fieldFlags(index);

    switch (index)
    {
        case nettest_timestamp:
        case nettest_seqnumber:
            break;

        default:
            qFatal("%s: unimplemented case %d in switch", __PRETTY_FUNCTION__,
                index);
            break;
    }

    return flags;
}

/*!
TODO: Edit this function to return the data for each field

See AbstractProtocol::fieldData() for more info
*/
QVariant NetTestProtocol::fieldData(int index, FieldAttrib attrib,
        int streamIndex) const
{
    switch (index)
    {
        // TODO nettest_timestamp
        case nettest_timestamp:
        {
            quint64 timestamp = data.timestamp();

            switch(attrib)
            {
                case FieldName:            
                    return QString("TIMESTAMP");
                case FieldValue:
                    return timestamp;
                case FieldTextValue:
                    return QString("%1").arg(timestamp);
                case FieldFrameValue:
                {
                    QByteArray fv;
                    fv.resize(8);
                    qToBigEndian((quint64) timestamp, (uchar*) fv.data());
                    return fv;
                }
                case FieldBitSize:
                    return 64;
                default:
                    break;
            }
            break;

        }
        // TODO nettest_seqnumber
        case nettest_seqnumber:
        {
            quint64 seqnumber = data.seqnumber();

            switch(attrib)
            {
                case FieldName:            
                    return QString("SEQNUMBER");
                case FieldValue:
                    return seqnumber;
                case FieldTextValue:
                    return QString("%1").arg(seqnumber);
                case FieldFrameValue:
                {
                    QByteArray fv;
                    fv.resize(8);
                    qToBigEndian((quint64) seqnumber, (uchar*) fv.data());
                    return fv;
                }
                case FieldBitSize:
                    return 64;
                default:
                    break;
            }
            break;
        }

        default:
            qFatal("%s: unimplemented case %d in switch", __PRETTY_FUNCTION__,
                index);
            break;
    }

    return AbstractProtocol::fieldData(index, attrib, streamIndex);
}

/*!
TODO: Edit this function to set the data for each field

See AbstractProtocol::setFieldData() for more info
*/
bool NetTestProtocol::setFieldData(int index, const QVariant &value,
        FieldAttrib attrib)
{
    bool isOk = false;

    if (attrib != FieldValue)
        goto _exit;

    switch (index)
    {
        // TODO nettest_timestamp
        case nettest_timestamp:
        {
            quint64 a = value.toULongLong(&isOk);
            if (!isOk)
                a = 0;
            data.set_timestamp(a);
            break;
        }
        // TODO nettest_seqnumber
        case nettest_seqnumber:
        {
            quint64 a = value.toULongLong(&isOk);
            if (!isOk)
                a = 0;
            data.set_seqnumber(a);
            break;
        }
        default:
            qFatal("%s: unimplemented case %d in switch", __PRETTY_FUNCTION__,
                index);
            break;
    }

_exit:
    return isOk;
}

/*!
  TODO: Return the protocol frame size in bytes\n

  If your protocol has a fixed size - you don't need to reimplement this; the
  base class implementation is good enough
*/
int NetTestProtocol::protocolFrameSize(int streamIndex) const
{
    return AbstractProtocol::protocolFrameSize(streamIndex);
}

/*!
  TODO: If your protocol frame size can vary across pkts of the same stream,
  return true \n

  Otherwise you don't need to reimplement this method - the base class always
  returns false
*/
bool NetTestProtocol::isProtocolFrameSizeVariable() const
{
    return false;
}

/*!
  TODO: If your protocol frame has any variable fields or has a variable
  size, return the minimum number of frames required to vary the fields \n

  See AbstractProtocol::protocolFrameVariableCount() for more info
*/
int NetTestProtocol::protocolFrameVariableCount() const
{
    return AbstractProtocol::protocolFrameVariableCount();
}
