//
// Generated file, do not edit! Created by opp_msgc 4.5 from mpcp.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "mpcp_m.h"

USING_NAMESPACE


// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}



namespace epon {

// Template rule for outputting std::vector<T> types
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

EXECUTE_ON_STARTUP(
    cEnum *e = cEnum::find("epon::FrameTag");
    if (!e) enums.getInstance()->add(e = new cEnum("epon::FrameTag"));
    e->insert(TagNormal, "TagNormal");
    e->insert(TagDebug, "TagDebug");
    e->insert(TagFront, "TagFront");
    e->insert(TagBack, "TagBack");
    e->insert(TagFrontAndBack, "TagFrontAndBack");
);

EXECUTE_ON_STARTUP(
    cEnum *e = cEnum::find("epon::Response");
    if (!e) enums.getInstance()->add(e = new cEnum("epon::Response"));
    e->insert(NOSIGNAL, "NOSIGNAL");
    e->insert(NONE, "NONE");
    e->insert(ACK, "ACK");
    e->insert(NACK, "NACK");
    e->insert(CS, "CS");
    e->insert(DS, "DS");
);

Register_Class(Frame);

Frame::Frame(const char *name, int kind) : ::cPacket(name,kind)
{
    this->DST_var = 0;
    this->SRC_var = 0;
    this->rxTime_var = 0;
    this->txTime_var = 0;
    this->tag_var = TagNormal;
}

Frame::Frame(const Frame& other) : ::cPacket(other)
{
    copy(other);
}

Frame::~Frame()
{
}

Frame& Frame::operator=(const Frame& other)
{
    if (this==&other) return *this;
    ::cPacket::operator=(other);
    copy(other);
    return *this;
}

void Frame::copy(const Frame& other)
{
    this->DST_var = other.DST_var;
    this->SRC_var = other.SRC_var;
    this->rxTime_var = other.rxTime_var;
    this->txTime_var = other.txTime_var;
    this->tag_var = other.tag_var;
}

void Frame::parsimPack(cCommBuffer *b)
{
    ::cPacket::parsimPack(b);
    doPacking(b,this->DST_var);
    doPacking(b,this->SRC_var);
    doPacking(b,this->rxTime_var);
    doPacking(b,this->txTime_var);
    doPacking(b,this->tag_var);
}

void Frame::parsimUnpack(cCommBuffer *b)
{
    ::cPacket::parsimUnpack(b);
    doUnpacking(b,this->DST_var);
    doUnpacking(b,this->SRC_var);
    doUnpacking(b,this->rxTime_var);
    doUnpacking(b,this->txTime_var);
    doUnpacking(b,this->tag_var);
}

int64_t Frame::getDST() const
{
    return DST_var;
}

void Frame::setDST(int64_t DST)
{
    this->DST_var = DST;
}

int64_t Frame::getSRC() const
{
    return SRC_var;
}

void Frame::setSRC(int64_t SRC)
{
    this->SRC_var = SRC;
}

simtime_t Frame::getRxTime() const
{
    return rxTime_var;
}

void Frame::setRxTime(simtime_t rxTime)
{
    this->rxTime_var = rxTime;
}

simtime_t Frame::getTxTime() const
{
    return txTime_var;
}

void Frame::setTxTime(simtime_t txTime)
{
    this->txTime_var = txTime;
}

int Frame::getTag() const
{
    return tag_var;
}

void Frame::setTag(int tag)
{
    this->tag_var = tag;
}

class FrameDescriptor : public cClassDescriptor
{
  public:
    FrameDescriptor();
    virtual ~FrameDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(FrameDescriptor);

FrameDescriptor::FrameDescriptor() : cClassDescriptor("epon::Frame", "cPacket")
{
}

FrameDescriptor::~FrameDescriptor()
{
}

bool FrameDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<Frame *>(obj)!=NULL;
}

const char *FrameDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int FrameDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 5+basedesc->getFieldCount(object) : 5;
}

unsigned int FrameDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<5) ? fieldTypeFlags[field] : 0;
}

const char *FrameDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "DST",
        "SRC",
        "rxTime",
        "txTime",
        "tag",
    };
    return (field>=0 && field<5) ? fieldNames[field] : NULL;
}

int FrameDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='D' && strcmp(fieldName, "DST")==0) return base+0;
    if (fieldName[0]=='S' && strcmp(fieldName, "SRC")==0) return base+1;
    if (fieldName[0]=='r' && strcmp(fieldName, "rxTime")==0) return base+2;
    if (fieldName[0]=='t' && strcmp(fieldName, "txTime")==0) return base+3;
    if (fieldName[0]=='t' && strcmp(fieldName, "tag")==0) return base+4;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *FrameDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int64_t",
        "int64_t",
        "simtime_t",
        "simtime_t",
        "int",
    };
    return (field>=0 && field<5) ? fieldTypeStrings[field] : NULL;
}

const char *FrameDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 4:
            if (!strcmp(propertyname,"enum")) return "epon::FrameTag";
            return NULL;
        default: return NULL;
    }
}

int FrameDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    Frame *pp = (Frame *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string FrameDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    Frame *pp = (Frame *)object; (void)pp;
    switch (field) {
        case 0: return int642string(pp->getDST());
        case 1: return int642string(pp->getSRC());
        case 2: return double2string(pp->getRxTime());
        case 3: return double2string(pp->getTxTime());
        case 4: return long2string(pp->getTag());
        default: return "";
    }
}

bool FrameDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    Frame *pp = (Frame *)object; (void)pp;
    switch (field) {
        case 0: pp->setDST(string2int64(value)); return true;
        case 1: pp->setSRC(string2int64(value)); return true;
        case 2: pp->setRxTime(string2double(value)); return true;
        case 3: pp->setTxTime(string2double(value)); return true;
        case 4: pp->setTag(string2long(value)); return true;
        default: return false;
    }
}

const char *FrameDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    };
}

void *FrameDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    Frame *pp = (Frame *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(DataFrame);

DataFrame::DataFrame(const char *name, int kind) : ::epon::Frame(name,kind)
{
    this->vacation_var = false;
    this->residualTime_var = 0;
    this->queueTime_var = 0;
    this->forwarded_var = false;
    this->s1Delay_var = 0;
    this->burstTime_var = 0;
}

DataFrame::DataFrame(const DataFrame& other) : ::epon::Frame(other)
{
    copy(other);
}

DataFrame::~DataFrame()
{
}

DataFrame& DataFrame::operator=(const DataFrame& other)
{
    if (this==&other) return *this;
    ::epon::Frame::operator=(other);
    copy(other);
    return *this;
}

void DataFrame::copy(const DataFrame& other)
{
    this->vacation_var = other.vacation_var;
    this->residualTime_var = other.residualTime_var;
    this->queueTime_var = other.queueTime_var;
    this->forwarded_var = other.forwarded_var;
    this->s1Delay_var = other.s1Delay_var;
    this->burstTime_var = other.burstTime_var;
}

void DataFrame::parsimPack(cCommBuffer *b)
{
    ::epon::Frame::parsimPack(b);
    doPacking(b,this->vacation_var);
    doPacking(b,this->residualTime_var);
    doPacking(b,this->queueTime_var);
    doPacking(b,this->forwarded_var);
    doPacking(b,this->s1Delay_var);
    doPacking(b,this->burstTime_var);
}

void DataFrame::parsimUnpack(cCommBuffer *b)
{
    ::epon::Frame::parsimUnpack(b);
    doUnpacking(b,this->vacation_var);
    doUnpacking(b,this->residualTime_var);
    doUnpacking(b,this->queueTime_var);
    doUnpacking(b,this->forwarded_var);
    doUnpacking(b,this->s1Delay_var);
    doUnpacking(b,this->burstTime_var);
}

bool DataFrame::getVacation() const
{
    return vacation_var;
}

void DataFrame::setVacation(bool vacation)
{
    this->vacation_var = vacation;
}

simtime_t DataFrame::getResidualTime() const
{
    return residualTime_var;
}

void DataFrame::setResidualTime(simtime_t residualTime)
{
    this->residualTime_var = residualTime;
}

simtime_t DataFrame::getQueueTime() const
{
    return queueTime_var;
}

void DataFrame::setQueueTime(simtime_t queueTime)
{
    this->queueTime_var = queueTime;
}

bool DataFrame::getForwarded() const
{
    return forwarded_var;
}

void DataFrame::setForwarded(bool forwarded)
{
    this->forwarded_var = forwarded;
}

simtime_t DataFrame::getS1Delay() const
{
    return s1Delay_var;
}

void DataFrame::setS1Delay(simtime_t s1Delay)
{
    this->s1Delay_var = s1Delay;
}

simtime_t DataFrame::getBurstTime() const
{
    return burstTime_var;
}

void DataFrame::setBurstTime(simtime_t burstTime)
{
    this->burstTime_var = burstTime;
}

class DataFrameDescriptor : public cClassDescriptor
{
  public:
    DataFrameDescriptor();
    virtual ~DataFrameDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(DataFrameDescriptor);

DataFrameDescriptor::DataFrameDescriptor() : cClassDescriptor("epon::DataFrame", "epon::Frame")
{
}

DataFrameDescriptor::~DataFrameDescriptor()
{
}

bool DataFrameDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<DataFrame *>(obj)!=NULL;
}

const char *DataFrameDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int DataFrameDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 6+basedesc->getFieldCount(object) : 6;
}

unsigned int DataFrameDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<6) ? fieldTypeFlags[field] : 0;
}

const char *DataFrameDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "vacation",
        "residualTime",
        "queueTime",
        "forwarded",
        "s1Delay",
        "burstTime",
    };
    return (field>=0 && field<6) ? fieldNames[field] : NULL;
}

int DataFrameDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='v' && strcmp(fieldName, "vacation")==0) return base+0;
    if (fieldName[0]=='r' && strcmp(fieldName, "residualTime")==0) return base+1;
    if (fieldName[0]=='q' && strcmp(fieldName, "queueTime")==0) return base+2;
    if (fieldName[0]=='f' && strcmp(fieldName, "forwarded")==0) return base+3;
    if (fieldName[0]=='s' && strcmp(fieldName, "s1Delay")==0) return base+4;
    if (fieldName[0]=='b' && strcmp(fieldName, "burstTime")==0) return base+5;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *DataFrameDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "bool",
        "simtime_t",
        "simtime_t",
        "bool",
        "simtime_t",
        "simtime_t",
    };
    return (field>=0 && field<6) ? fieldTypeStrings[field] : NULL;
}

const char *DataFrameDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int DataFrameDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    DataFrame *pp = (DataFrame *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string DataFrameDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    DataFrame *pp = (DataFrame *)object; (void)pp;
    switch (field) {
        case 0: return bool2string(pp->getVacation());
        case 1: return double2string(pp->getResidualTime());
        case 2: return double2string(pp->getQueueTime());
        case 3: return bool2string(pp->getForwarded());
        case 4: return double2string(pp->getS1Delay());
        case 5: return double2string(pp->getBurstTime());
        default: return "";
    }
}

bool DataFrameDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    DataFrame *pp = (DataFrame *)object; (void)pp;
    switch (field) {
        case 0: pp->setVacation(string2bool(value)); return true;
        case 1: pp->setResidualTime(string2double(value)); return true;
        case 2: pp->setQueueTime(string2double(value)); return true;
        case 3: pp->setForwarded(string2bool(value)); return true;
        case 4: pp->setS1Delay(string2double(value)); return true;
        case 5: pp->setBurstTime(string2double(value)); return true;
        default: return false;
    }
}

const char *DataFrameDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    };
}

void *DataFrameDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    DataFrame *pp = (DataFrame *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(MPCPGate);

MPCPGate::MPCPGate(const char *name, int kind) : ::epon::Frame(name,kind)
{
    this->startTime_var = 0;
    this->length_var = 0;
}

MPCPGate::MPCPGate(const MPCPGate& other) : ::epon::Frame(other)
{
    copy(other);
}

MPCPGate::~MPCPGate()
{
}

MPCPGate& MPCPGate::operator=(const MPCPGate& other)
{
    if (this==&other) return *this;
    ::epon::Frame::operator=(other);
    copy(other);
    return *this;
}

void MPCPGate::copy(const MPCPGate& other)
{
    this->startTime_var = other.startTime_var;
    this->length_var = other.length_var;
}

void MPCPGate::parsimPack(cCommBuffer *b)
{
    ::epon::Frame::parsimPack(b);
    doPacking(b,this->startTime_var);
    doPacking(b,this->length_var);
}

void MPCPGate::parsimUnpack(cCommBuffer *b)
{
    ::epon::Frame::parsimUnpack(b);
    doUnpacking(b,this->startTime_var);
    doUnpacking(b,this->length_var);
}

simtime_t MPCPGate::getStartTime() const
{
    return startTime_var;
}

void MPCPGate::setStartTime(simtime_t startTime)
{
    this->startTime_var = startTime;
}

simtime_t MPCPGate::getLength() const
{
    return length_var;
}

void MPCPGate::setLength(simtime_t length)
{
    this->length_var = length;
}

class MPCPGateDescriptor : public cClassDescriptor
{
  public:
    MPCPGateDescriptor();
    virtual ~MPCPGateDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(MPCPGateDescriptor);

MPCPGateDescriptor::MPCPGateDescriptor() : cClassDescriptor("epon::MPCPGate", "epon::Frame")
{
}

MPCPGateDescriptor::~MPCPGateDescriptor()
{
}

bool MPCPGateDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<MPCPGate *>(obj)!=NULL;
}

const char *MPCPGateDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int MPCPGateDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int MPCPGateDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *MPCPGateDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "startTime",
        "length",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int MPCPGateDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "startTime")==0) return base+0;
    if (fieldName[0]=='l' && strcmp(fieldName, "length")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *MPCPGateDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "simtime_t",
        "simtime_t",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *MPCPGateDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int MPCPGateDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    MPCPGate *pp = (MPCPGate *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string MPCPGateDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    MPCPGate *pp = (MPCPGate *)object; (void)pp;
    switch (field) {
        case 0: return double2string(pp->getStartTime());
        case 1: return double2string(pp->getLength());
        default: return "";
    }
}

bool MPCPGateDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    MPCPGate *pp = (MPCPGate *)object; (void)pp;
    switch (field) {
        case 0: pp->setStartTime(string2double(value)); return true;
        case 1: pp->setLength(string2double(value)); return true;
        default: return false;
    }
}

const char *MPCPGateDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    };
}

void *MPCPGateDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    MPCPGate *pp = (MPCPGate *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(MPCPReport);

MPCPReport::MPCPReport(const char *name, int kind) : ::epon::Frame(name,kind)
{
    this->requiredBandwidth_var = 0;
}

MPCPReport::MPCPReport(const MPCPReport& other) : ::epon::Frame(other)
{
    copy(other);
}

MPCPReport::~MPCPReport()
{
}

MPCPReport& MPCPReport::operator=(const MPCPReport& other)
{
    if (this==&other) return *this;
    ::epon::Frame::operator=(other);
    copy(other);
    return *this;
}

void MPCPReport::copy(const MPCPReport& other)
{
    this->requiredBandwidth_var = other.requiredBandwidth_var;
}

void MPCPReport::parsimPack(cCommBuffer *b)
{
    ::epon::Frame::parsimPack(b);
    doPacking(b,this->requiredBandwidth_var);
}

void MPCPReport::parsimUnpack(cCommBuffer *b)
{
    ::epon::Frame::parsimUnpack(b);
    doUnpacking(b,this->requiredBandwidth_var);
}

int MPCPReport::getRequiredBandwidth() const
{
    return requiredBandwidth_var;
}

void MPCPReport::setRequiredBandwidth(int requiredBandwidth)
{
    this->requiredBandwidth_var = requiredBandwidth;
}

class MPCPReportDescriptor : public cClassDescriptor
{
  public:
    MPCPReportDescriptor();
    virtual ~MPCPReportDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(MPCPReportDescriptor);

MPCPReportDescriptor::MPCPReportDescriptor() : cClassDescriptor("epon::MPCPReport", "epon::Frame")
{
}

MPCPReportDescriptor::~MPCPReportDescriptor()
{
}

bool MPCPReportDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<MPCPReport *>(obj)!=NULL;
}

const char *MPCPReportDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int MPCPReportDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int MPCPReportDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *MPCPReportDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "requiredBandwidth",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int MPCPReportDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='r' && strcmp(fieldName, "requiredBandwidth")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *MPCPReportDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *MPCPReportDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int MPCPReportDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    MPCPReport *pp = (MPCPReport *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string MPCPReportDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    MPCPReport *pp = (MPCPReport *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getRequiredBandwidth());
        default: return "";
    }
}

bool MPCPReportDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    MPCPReport *pp = (MPCPReport *)object; (void)pp;
    switch (field) {
        case 0: pp->setRequiredBandwidth(string2long(value)); return true;
        default: return false;
    }
}

const char *MPCPReportDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    };
}

void *MPCPReportDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    MPCPReport *pp = (MPCPReport *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(SLAGate);

SLAGate::SLAGate(const char *name, int kind) : ::epon::MPCPGate(name,kind)
{
    this->signal_var = 0;
    this->sleepTime_var = 0;
    this->wakeUpTime_var = 0;
}

SLAGate::SLAGate(const SLAGate& other) : ::epon::MPCPGate(other)
{
    copy(other);
}

SLAGate::~SLAGate()
{
}

SLAGate& SLAGate::operator=(const SLAGate& other)
{
    if (this==&other) return *this;
    ::epon::MPCPGate::operator=(other);
    copy(other);
    return *this;
}

void SLAGate::copy(const SLAGate& other)
{
    this->signal_var = other.signal_var;
    this->sleepTime_var = other.sleepTime_var;
    this->wakeUpTime_var = other.wakeUpTime_var;
}

void SLAGate::parsimPack(cCommBuffer *b)
{
    ::epon::MPCPGate::parsimPack(b);
    doPacking(b,this->signal_var);
    doPacking(b,this->sleepTime_var);
    doPacking(b,this->wakeUpTime_var);
}

void SLAGate::parsimUnpack(cCommBuffer *b)
{
    ::epon::MPCPGate::parsimUnpack(b);
    doUnpacking(b,this->signal_var);
    doUnpacking(b,this->sleepTime_var);
    doUnpacking(b,this->wakeUpTime_var);
}

int SLAGate::getSignal() const
{
    return signal_var;
}

void SLAGate::setSignal(int signal)
{
    this->signal_var = signal;
}

simtime_t SLAGate::getSleepTime() const
{
    return sleepTime_var;
}

void SLAGate::setSleepTime(simtime_t sleepTime)
{
    this->sleepTime_var = sleepTime;
}

simtime_t SLAGate::getWakeUpTime() const
{
    return wakeUpTime_var;
}

void SLAGate::setWakeUpTime(simtime_t wakeUpTime)
{
    this->wakeUpTime_var = wakeUpTime;
}

class SLAGateDescriptor : public cClassDescriptor
{
  public:
    SLAGateDescriptor();
    virtual ~SLAGateDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(SLAGateDescriptor);

SLAGateDescriptor::SLAGateDescriptor() : cClassDescriptor("epon::SLAGate", "epon::MPCPGate")
{
}

SLAGateDescriptor::~SLAGateDescriptor()
{
}

bool SLAGateDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<SLAGate *>(obj)!=NULL;
}

const char *SLAGateDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int SLAGateDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount(object) : 3;
}

unsigned int SLAGateDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<3) ? fieldTypeFlags[field] : 0;
}

const char *SLAGateDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "signal",
        "sleepTime",
        "wakeUpTime",
    };
    return (field>=0 && field<3) ? fieldNames[field] : NULL;
}

int SLAGateDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "signal")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "sleepTime")==0) return base+1;
    if (fieldName[0]=='w' && strcmp(fieldName, "wakeUpTime")==0) return base+2;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *SLAGateDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "simtime_t",
        "simtime_t",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : NULL;
}

const char *SLAGateDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"enum")) return "epon::Response";
            return NULL;
        default: return NULL;
    }
}

int SLAGateDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    SLAGate *pp = (SLAGate *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string SLAGateDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    SLAGate *pp = (SLAGate *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getSignal());
        case 1: return double2string(pp->getSleepTime());
        case 2: return double2string(pp->getWakeUpTime());
        default: return "";
    }
}

bool SLAGateDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    SLAGate *pp = (SLAGate *)object; (void)pp;
    switch (field) {
        case 0: pp->setSignal(string2long(value)); return true;
        case 1: pp->setSleepTime(string2double(value)); return true;
        case 2: pp->setWakeUpTime(string2double(value)); return true;
        default: return false;
    }
}

const char *SLAGateDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    };
}

void *SLAGateDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    SLAGate *pp = (SLAGate *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(SLAReport);

SLAReport::SLAReport(const char *name, int kind) : ::epon::MPCPReport(name,kind)
{
    this->signal_var = 0;
}

SLAReport::SLAReport(const SLAReport& other) : ::epon::MPCPReport(other)
{
    copy(other);
}

SLAReport::~SLAReport()
{
}

SLAReport& SLAReport::operator=(const SLAReport& other)
{
    if (this==&other) return *this;
    ::epon::MPCPReport::operator=(other);
    copy(other);
    return *this;
}

void SLAReport::copy(const SLAReport& other)
{
    this->signal_var = other.signal_var;
}

void SLAReport::parsimPack(cCommBuffer *b)
{
    ::epon::MPCPReport::parsimPack(b);
    doPacking(b,this->signal_var);
}

void SLAReport::parsimUnpack(cCommBuffer *b)
{
    ::epon::MPCPReport::parsimUnpack(b);
    doUnpacking(b,this->signal_var);
}

int SLAReport::getSignal() const
{
    return signal_var;
}

void SLAReport::setSignal(int signal)
{
    this->signal_var = signal;
}

class SLAReportDescriptor : public cClassDescriptor
{
  public:
    SLAReportDescriptor();
    virtual ~SLAReportDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(SLAReportDescriptor);

SLAReportDescriptor::SLAReportDescriptor() : cClassDescriptor("epon::SLAReport", "epon::MPCPReport")
{
}

SLAReportDescriptor::~SLAReportDescriptor()
{
}

bool SLAReportDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<SLAReport *>(obj)!=NULL;
}

const char *SLAReportDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int SLAReportDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int SLAReportDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *SLAReportDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "signal",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int SLAReportDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "signal")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *SLAReportDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *SLAReportDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"enum")) return "epon::Response";
            return NULL;
        default: return NULL;
    }
}

int SLAReportDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    SLAReport *pp = (SLAReport *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string SLAReportDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    SLAReport *pp = (SLAReport *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getSignal());
        default: return "";
    }
}

bool SLAReportDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    SLAReport *pp = (SLAReport *)object; (void)pp;
    switch (field) {
        case 0: pp->setSignal(string2long(value)); return true;
        default: return false;
    }
}

const char *SLAReportDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    };
}

void *SLAReportDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    SLAReport *pp = (SLAReport *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(CSLAGate);

CSLAGate::CSLAGate(const char *name, int kind) : ::epon::Frame(name,kind)
{
    this->Grant1TransceiverMode_var = 0;
    this->Grant1StartTime_var = 0;
    this->Grant1Length_var = 0;
    this->Grant2StartTime_var = 0;
    this->Grant2Length_var = 0;
}

CSLAGate::CSLAGate(const CSLAGate& other) : ::epon::Frame(other)
{
    copy(other);
}

CSLAGate::~CSLAGate()
{
}

CSLAGate& CSLAGate::operator=(const CSLAGate& other)
{
    if (this==&other) return *this;
    ::epon::Frame::operator=(other);
    copy(other);
    return *this;
}

void CSLAGate::copy(const CSLAGate& other)
{
    this->Grant1TransceiverMode_var = other.Grant1TransceiverMode_var;
    this->Grant1StartTime_var = other.Grant1StartTime_var;
    this->Grant1Length_var = other.Grant1Length_var;
    this->Grant2StartTime_var = other.Grant2StartTime_var;
    this->Grant2Length_var = other.Grant2Length_var;
}

void CSLAGate::parsimPack(cCommBuffer *b)
{
    ::epon::Frame::parsimPack(b);
    doPacking(b,this->Grant1TransceiverMode_var);
    doPacking(b,this->Grant1StartTime_var);
    doPacking(b,this->Grant1Length_var);
    doPacking(b,this->Grant2StartTime_var);
    doPacking(b,this->Grant2Length_var);
}

void CSLAGate::parsimUnpack(cCommBuffer *b)
{
    ::epon::Frame::parsimUnpack(b);
    doUnpacking(b,this->Grant1TransceiverMode_var);
    doUnpacking(b,this->Grant1StartTime_var);
    doUnpacking(b,this->Grant1Length_var);
    doUnpacking(b,this->Grant2StartTime_var);
    doUnpacking(b,this->Grant2Length_var);
}

int CSLAGate::getGrant1TransceiverMode() const
{
    return Grant1TransceiverMode_var;
}

void CSLAGate::setGrant1TransceiverMode(int Grant1TransceiverMode)
{
    this->Grant1TransceiverMode_var = Grant1TransceiverMode;
}

simtime_t CSLAGate::getGrant1StartTime() const
{
    return Grant1StartTime_var;
}

void CSLAGate::setGrant1StartTime(simtime_t Grant1StartTime)
{
    this->Grant1StartTime_var = Grant1StartTime;
}

simtime_t CSLAGate::getGrant1Length() const
{
    return Grant1Length_var;
}

void CSLAGate::setGrant1Length(simtime_t Grant1Length)
{
    this->Grant1Length_var = Grant1Length;
}

simtime_t CSLAGate::getGrant2StartTime() const
{
    return Grant2StartTime_var;
}

void CSLAGate::setGrant2StartTime(simtime_t Grant2StartTime)
{
    this->Grant2StartTime_var = Grant2StartTime;
}

simtime_t CSLAGate::getGrant2Length() const
{
    return Grant2Length_var;
}

void CSLAGate::setGrant2Length(simtime_t Grant2Length)
{
    this->Grant2Length_var = Grant2Length;
}

class CSLAGateDescriptor : public cClassDescriptor
{
  public:
    CSLAGateDescriptor();
    virtual ~CSLAGateDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(CSLAGateDescriptor);

CSLAGateDescriptor::CSLAGateDescriptor() : cClassDescriptor("epon::CSLAGate", "epon::Frame")
{
}

CSLAGateDescriptor::~CSLAGateDescriptor()
{
}

bool CSLAGateDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<CSLAGate *>(obj)!=NULL;
}

const char *CSLAGateDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int CSLAGateDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 5+basedesc->getFieldCount(object) : 5;
}

unsigned int CSLAGateDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<5) ? fieldTypeFlags[field] : 0;
}

const char *CSLAGateDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "Grant1TransceiverMode",
        "Grant1StartTime",
        "Grant1Length",
        "Grant2StartTime",
        "Grant2Length",
    };
    return (field>=0 && field<5) ? fieldNames[field] : NULL;
}

int CSLAGateDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='G' && strcmp(fieldName, "Grant1TransceiverMode")==0) return base+0;
    if (fieldName[0]=='G' && strcmp(fieldName, "Grant1StartTime")==0) return base+1;
    if (fieldName[0]=='G' && strcmp(fieldName, "Grant1Length")==0) return base+2;
    if (fieldName[0]=='G' && strcmp(fieldName, "Grant2StartTime")==0) return base+3;
    if (fieldName[0]=='G' && strcmp(fieldName, "Grant2Length")==0) return base+4;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *CSLAGateDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "simtime_t",
        "simtime_t",
        "simtime_t",
        "simtime_t",
    };
    return (field>=0 && field<5) ? fieldTypeStrings[field] : NULL;
}

const char *CSLAGateDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"enum")) return "epon::Response";
            return NULL;
        default: return NULL;
    }
}

int CSLAGateDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    CSLAGate *pp = (CSLAGate *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string CSLAGateDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    CSLAGate *pp = (CSLAGate *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getGrant1TransceiverMode());
        case 1: return double2string(pp->getGrant1StartTime());
        case 2: return double2string(pp->getGrant1Length());
        case 3: return double2string(pp->getGrant2StartTime());
        case 4: return double2string(pp->getGrant2Length());
        default: return "";
    }
}

bool CSLAGateDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    CSLAGate *pp = (CSLAGate *)object; (void)pp;
    switch (field) {
        case 0: pp->setGrant1TransceiverMode(string2long(value)); return true;
        case 1: pp->setGrant1StartTime(string2double(value)); return true;
        case 2: pp->setGrant1Length(string2double(value)); return true;
        case 3: pp->setGrant2StartTime(string2double(value)); return true;
        case 4: pp->setGrant2Length(string2double(value)); return true;
        default: return false;
    }
}

const char *CSLAGateDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    };
}

void *CSLAGateDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    CSLAGate *pp = (CSLAGate *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(CSLAReport);

CSLAReport::CSLAReport(const char *name, int kind) : ::epon::Frame(name,kind)
{
    this->Request1Length_var = 0;
}

CSLAReport::CSLAReport(const CSLAReport& other) : ::epon::Frame(other)
{
    copy(other);
}

CSLAReport::~CSLAReport()
{
}

CSLAReport& CSLAReport::operator=(const CSLAReport& other)
{
    if (this==&other) return *this;
    ::epon::Frame::operator=(other);
    copy(other);
    return *this;
}

void CSLAReport::copy(const CSLAReport& other)
{
    this->Request1Length_var = other.Request1Length_var;
}

void CSLAReport::parsimPack(cCommBuffer *b)
{
    ::epon::Frame::parsimPack(b);
    doPacking(b,this->Request1Length_var);
}

void CSLAReport::parsimUnpack(cCommBuffer *b)
{
    ::epon::Frame::parsimUnpack(b);
    doUnpacking(b,this->Request1Length_var);
}

int CSLAReport::getRequest1Length() const
{
    return Request1Length_var;
}

void CSLAReport::setRequest1Length(int Request1Length)
{
    this->Request1Length_var = Request1Length;
}

class CSLAReportDescriptor : public cClassDescriptor
{
  public:
    CSLAReportDescriptor();
    virtual ~CSLAReportDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(CSLAReportDescriptor);

CSLAReportDescriptor::CSLAReportDescriptor() : cClassDescriptor("epon::CSLAReport", "epon::Frame")
{
}

CSLAReportDescriptor::~CSLAReportDescriptor()
{
}

bool CSLAReportDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<CSLAReport *>(obj)!=NULL;
}

const char *CSLAReportDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int CSLAReportDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int CSLAReportDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *CSLAReportDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "Request1Length",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int CSLAReportDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='R' && strcmp(fieldName, "Request1Length")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *CSLAReportDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *CSLAReportDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int CSLAReportDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    CSLAReport *pp = (CSLAReport *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string CSLAReportDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    CSLAReport *pp = (CSLAReport *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getRequest1Length());
        default: return "";
    }
}

bool CSLAReportDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    CSLAReport *pp = (CSLAReport *)object; (void)pp;
    switch (field) {
        case 0: pp->setRequest1Length(string2long(value)); return true;
        default: return false;
    }
}

const char *CSLAReportDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    };
}

void *CSLAReportDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    CSLAReport *pp = (CSLAReport *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(GBAGate);

GBAGate::GBAGate(const char *name, int kind) : ::epon::MPCPGate(name,kind)
{
    this->ONUSleepTime_var = 0;
}

GBAGate::GBAGate(const GBAGate& other) : ::epon::MPCPGate(other)
{
    copy(other);
}

GBAGate::~GBAGate()
{
}

GBAGate& GBAGate::operator=(const GBAGate& other)
{
    if (this==&other) return *this;
    ::epon::MPCPGate::operator=(other);
    copy(other);
    return *this;
}

void GBAGate::copy(const GBAGate& other)
{
    this->ONUSleepTime_var = other.ONUSleepTime_var;
}

void GBAGate::parsimPack(cCommBuffer *b)
{
    ::epon::MPCPGate::parsimPack(b);
    doPacking(b,this->ONUSleepTime_var);
}

void GBAGate::parsimUnpack(cCommBuffer *b)
{
    ::epon::MPCPGate::parsimUnpack(b);
    doUnpacking(b,this->ONUSleepTime_var);
}

simtime_t GBAGate::getONUSleepTime() const
{
    return ONUSleepTime_var;
}

void GBAGate::setONUSleepTime(simtime_t ONUSleepTime)
{
    this->ONUSleepTime_var = ONUSleepTime;
}

class GBAGateDescriptor : public cClassDescriptor
{
  public:
    GBAGateDescriptor();
    virtual ~GBAGateDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(GBAGateDescriptor);

GBAGateDescriptor::GBAGateDescriptor() : cClassDescriptor("epon::GBAGate", "epon::MPCPGate")
{
}

GBAGateDescriptor::~GBAGateDescriptor()
{
}

bool GBAGateDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<GBAGate *>(obj)!=NULL;
}

const char *GBAGateDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int GBAGateDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int GBAGateDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *GBAGateDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "ONUSleepTime",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int GBAGateDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='O' && strcmp(fieldName, "ONUSleepTime")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *GBAGateDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "simtime_t",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *GBAGateDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int GBAGateDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    GBAGate *pp = (GBAGate *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string GBAGateDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    GBAGate *pp = (GBAGate *)object; (void)pp;
    switch (field) {
        case 0: return double2string(pp->getONUSleepTime());
        default: return "";
    }
}

bool GBAGateDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    GBAGate *pp = (GBAGate *)object; (void)pp;
    switch (field) {
        case 0: pp->setONUSleepTime(string2double(value)); return true;
        default: return false;
    }
}

const char *GBAGateDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    };
}

void *GBAGateDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    GBAGate *pp = (GBAGate *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(GBAReport);

GBAReport::GBAReport(const char *name, int kind) : ::epon::MPCPReport(name,kind)
{
}

GBAReport::GBAReport(const GBAReport& other) : ::epon::MPCPReport(other)
{
    copy(other);
}

GBAReport::~GBAReport()
{
}

GBAReport& GBAReport::operator=(const GBAReport& other)
{
    if (this==&other) return *this;
    ::epon::MPCPReport::operator=(other);
    copy(other);
    return *this;
}

void GBAReport::copy(const GBAReport& other)
{
}

void GBAReport::parsimPack(cCommBuffer *b)
{
    ::epon::MPCPReport::parsimPack(b);
}

void GBAReport::parsimUnpack(cCommBuffer *b)
{
    ::epon::MPCPReport::parsimUnpack(b);
}

class GBAReportDescriptor : public cClassDescriptor
{
  public:
    GBAReportDescriptor();
    virtual ~GBAReportDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(GBAReportDescriptor);

GBAReportDescriptor::GBAReportDescriptor() : cClassDescriptor("epon::GBAReport", "epon::MPCPReport")
{
}

GBAReportDescriptor::~GBAReportDescriptor()
{
}

bool GBAReportDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<GBAReport *>(obj)!=NULL;
}

const char *GBAReportDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int GBAReportDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 0+basedesc->getFieldCount(object) : 0;
}

unsigned int GBAReportDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return 0;
}

const char *GBAReportDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

int GBAReportDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *GBAReportDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

const char *GBAReportDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int GBAReportDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    GBAReport *pp = (GBAReport *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string GBAReportDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    GBAReport *pp = (GBAReport *)object; (void)pp;
    switch (field) {
        default: return "";
    }
}

bool GBAReportDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    GBAReport *pp = (GBAReport *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *GBAReportDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

void *GBAReportDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    GBAReport *pp = (GBAReport *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

}; // end namespace epon

