#ifndef  INCLUDED_SERIALIZATION_UTIL_H
#define  INCLUDED_SERIALIZATION_UTIL_H
#include "BitSerialization.h"

#define BEGIN_SERIALIZATION\
	template<typename Stream> inline bool Serialize(Stream& stream)\
	{

#define SERIALIZE_BOOL(data)\
	if(!SingularityEngine::Core::Serialize_bool(stream, data)) return false;

#define SERIALIZE_STRING(data, length)\
	if(!SingularityEngine::Core::Serialize_String(stream, albedoFileName, length)) return false;

#define SERIALIZE_FLOAT(data)\
	if(!SingularityEngine::Core::Serialize_Float(stream, data)) return false;

#define SERIALIZE_COMPRESSED_FLOAT(data, min, max)\
	if(!SingularityEngine::Core::Serialize_Compressed_Float(stream, data, min, max)) return false;

#define SERIALIZE_BYTES(data, datatype,length)\
	if(!SingularityEngine::Core::Serialize_Bytes(stream, reinterpret_cast<uint8_t*>(data), sizeof(datatype) * length)) return false;

#define SERIALIZE_INT(data)\
	if(!SingularityEngine::Core::Serialize_Int(stream, data)) return false;

#define SERIALIZE_COMPRESSED_INT(data, min, max)\
	if(!SingularityEngine::Core::Serialize_Int_Compressed(stream, data, min, max)) return false;


#define END_SERIALIZATION\
		return true;\
	}

#endif