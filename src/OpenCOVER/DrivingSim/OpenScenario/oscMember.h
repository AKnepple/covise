/* This file is part of COVISE.

You can use it under the terms of the GNU Lesser General Public License
version 2.1 or later, see lgpl-2.1.txt.

* License: LGPL 2+ */

#ifndef OSC_MEMBER_H
#define OSC_MEMBER_H

#include <oscExport.h>
#include <oscMemberValue.h>
#include <string>
#include <xercesc/util/XercesDefs.hpp>
XERCES_CPP_NAMESPACE_BEGIN
class DOMDocument;
class DOMElement;
XERCES_CPP_NAMESPACE_END

namespace OpenScenario {

class OpenScenarioBase;
class oscObjectBase;


/// \class This class represents a generic OpenScenario Member variable
class OPENSCENARIOEXPORT oscMember
{
protected:

    std::string name;
    oscMemberValue *value;
    enum oscMemberValue::MemberTypes type;

public:
    oscMember(); ///< constructor
    void setName(const char *n){name = n;};
    void setName(std::string &n){name = n;};
    void registerWith(oscObjectBase* owner); ///< constructor
    

    virtual ~oscMember(); ///< destructor
    
    virtual oscMemberValue * getValue() {return value;};
    virtual void setValue(oscMemberValue *v) {value = v;};
    virtual void setValue(oscObjectBase *t){};
    void setType(oscMemberValue::MemberTypes t) {type = t;};
    oscMemberValue::MemberTypes getType() {return type;}; ///< return the type of this member
    std::string &getName(){return name;};

};


}

#endif //OSC_MEMBER_H