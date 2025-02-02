//
// Created by Dev on 10/18/2017.
//

#include <Serialization/Archive.h>
#include <Serialization/ISerializerHost.h>
#include <Core/Common.h>

DJINN_NS2(Serialization);

Archive::Archive(ISerializerHost* host, Core::IStream *stream) : host(host), BinaryStream(stream) {

}

COM::Result Archive::operator<<(COM::IUnknown *com) {
    return host->Serialize(*this, com);
}

COM::Result Archive::operator>>(COM::IUnknown **com) {
    return host->Deserialize(*this, com);
}

COM::Result Archive::operator>>(COM::IUnknown *com) {
    return host->Deserialize(*this, com);
}
