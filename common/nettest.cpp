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

quint32 NetTestProtocol::protocolId(AbstractProtocol::ProtocolIdType type) const
{
    switch(type)
    {
        case ProtocolIdLlc: return 0xFFFFFE;
        case ProtocolIdEth: return 0xFFFE;
        case ProtocolIdIp: return 0xFE;
        default:break;
    }

    return AbstractProtocol::protocolId(type);
}

QString NetTestProtocol::name() const
{
    return QString("Протокол NetTest");
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

        case nettest_timestampMode: // Мета-поля протокола, для них сбрасываем флаг фреймового поля и устанавливаем флаг мета-поля
        case nettest_seqnumberMode:
            flags &= ~FrameField;
            flags |= MetaField;
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
        case nettest_seqnumber:
        {
            switch(attrib)
            {
                case FieldName:            
                    return QString("SEQNUMBER");
                case FieldValue:
                {
                    quint64 s = (quint64)streamIndex;
                    return s;
                }
                case FieldTextValue:
                    return QString(fieldData(index, FieldFrameValue,
                            streamIndex).toByteArray().toHex());
                case FieldFrameValue:
                {
                    QByteArray fv;
                    fv.resize(8);
                    qToBigEndian(fieldData(index, FieldValue, streamIndex).toULongLong(),
                                 (uchar*) fv.data());
                    return fv;
                }
                case FieldBitSize:
                    return 64;
                default:
                    break;
            }
            break;
        }

        case nettest_timestampMode:
            switch(attrib)
            {
                case FieldValue: return data.timestamp_mode();
                default: break;
            }
            break;

        case nettest_seqnumberMode:
            switch(attrib)
            {
                case FieldValue: return data.seqnumber_mode();
                default: break;
            }
            break;

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
        case nettest_timestamp:
        {
            quint64 a = value.toULongLong(&isOk);
            if (!isOk)
                a = 0;
            data.set_timestamp(a);
            break;
        }
        case nettest_seqnumber:
        {
            quint64 a = value.toULongLong(&isOk);
            if (!isOk)
                a = 0;
            data.set_seqnumber(a);
            break;
        }

        // Meta-Fields
        case nettest_timestampMode:
        {
            uint mode = value.toUInt(&isOk);
            if (isOk && data.TimeStampMode_IsValid(mode))
                data.set_timestamp_mode((OstProto::NetTest::TimeStampMode) mode);
            else
                isOk = false;
            break;
        }
        case nettest_seqnumberMode:
        {
            uint mode = value.toUInt(&isOk);
            if (isOk && data.SeqNumberMode_IsValid(mode))
                data.set_seqnumber_mode((OstProto::NetTest::SeqNumberMode) mode);
            else
                isOk = false;
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
