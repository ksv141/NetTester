#ifndef _NETTEST_H
#define _NETTEST_H

#include "abstractprotocol.h"
#include "nettest.pb.h"

/* 
Nettest Protocol Frame Format -
    +-------------+--------------+
    |  timestamp  |   seqnumber  |
    |    (64)     |     (64)     |
    +-------------+--------------+
Figures in brackets represent field width in bits
*/

class NetTestProtocol : public AbstractProtocol
{
public:
    enum nettestfield
    {
        // Frame Fields
        nettest_timestamp = 0,
        nettest_seqnumber,

        // Meta Fields
        nettest_fieldCount
    };

    NetTestProtocol(StreamBase *stream, AbstractProtocol *parent = 0);
    virtual ~NetTestProtocol();

    static AbstractProtocol* createInstance(StreamBase *stream,
        AbstractProtocol *parent = 0);
    virtual quint32 protocolNumber() const;

    virtual void protoDataCopyInto(OstProto::Protocol &protocol) const;
    virtual void protoDataCopyFrom(const OstProto::Protocol &protocol);

    virtual QString name() const;
    virtual QString shortName() const;

    virtual int fieldCount() const;

    virtual AbstractProtocol::FieldFlags fieldFlags(int index) const;
    virtual QVariant fieldData(int index, FieldAttrib attrib,
               int streamIndex = 0) const;
    virtual bool setFieldData(int index, const QVariant &value, 
            FieldAttrib attrib = FieldValue);

    virtual int protocolFrameSize(int streamIndex = 0) const;

    virtual bool isProtocolFrameSizeVariable() const;
    virtual int protocolFrameVariableCount() const;

private:
    OstProto::NetTest    data;
};

#endif
