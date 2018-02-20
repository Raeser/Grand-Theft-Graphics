#include <stdafx.h>
#include "JsonDocument.h"
#include <fstream>

#include "FileDialog.h"

#include "Parser/rapidjson/prettywriter.h"
#include "Parser/rapidjson/stringbuffer.h"
#include "Parser/rapidjson/writer.h"

#include <iostream>

JsonDocument::JsonDocument()
{
	myDocument.SetObject();
}

JsonDocument::JsonDocument(const char * aFileLocation)
{
	LoadFile(aFileLocation);
}

JsonDocument::~JsonDocument()
{
}

void JsonDocument::LoadFile(const char * aFileLocation, bool aEncode)
{
	myDocument.Parse(Corgi::OpenFile(aFileLocation, aEncode).c_str());
}

void JsonDocument::SaveFile(const char * aFileLocation, bool aPrettify, bool aEncode)
{
	rapidjson::StringBuffer buffer;
	if (aPrettify)
	{
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
		myDocument.Accept(writer);
	}
	else
	{
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		myDocument.Accept(writer);
	}

	Corgi::SaveFile(aFileLocation, buffer.GetString(), aEncode);
}

void JsonDocument::AddString(const char * aName, const char * aString)
{
	rapidjson::Value name(aName, myDocument.GetAllocator());
	rapidjson::Value value(aString, myDocument.GetAllocator());
	myDocument.AddMember(name, value.Move(), myDocument.GetAllocator());
}

void JsonDocument::AddObject(const char * aName, JsonObject& aObject)
{
	rapidjson::Value v(aObject.myDocument, myDocument.GetAllocator());
	rapidjson::Value name(aName, myDocument.GetAllocator());
	myDocument.AddMember(name, v, myDocument.GetAllocator());
}

void JsonDocument::AddArray(const char * aName, JsonArray& aArray)
{
	rapidjson::Value name(aName, myDocument.GetAllocator());
	myDocument.AddMember(name, aArray.myArray, myDocument.GetAllocator());
}

void JsonDocument::AddArray(const char * aName)
{
	rapidjson::Value name(aName, myDocument.GetAllocator());
	JsonArray arr;
	myDocument.AddMember(name, arr.myArray, myDocument.GetAllocator());
}

bool JsonDocument::Find(const char * aName) const
{
	return myDocument.FindMember(aName) != myDocument.MemberEnd();
}

void JsonValue::SetString(const char * aString)
{
	myValue->SetString(aString, myDocument->GetAllocator());
}

void JsonValue::AddObject(const char * aName, JsonObject& aObject)
{
	rapidjson::Value v(aObject.myDocument, myDocument->GetAllocator());
	rapidjson::Value name(aName, myDocument->GetAllocator());
	myDocument->AddMember(name, v, myDocument->GetAllocator());
}

void JsonValue::AddArray(const char * aName, JsonArray & aArray)
{
	assert(myValue->IsArray() || myValue->IsObject());
	rapidjson::Value name(aName, myDocument->GetAllocator());
	myValue->AddMember(name, aArray.myArray, myDocument->GetAllocator());
}

bool JsonValue::Find(const char * aName) const
{
	return myValue->FindMember(aName) != myValue->MemberEnd();
}

int JsonValue::FindIndex(const char * aName) const
{
	return myValue->FindMember(aName) != myValue->MemberEnd();
}


void JsonObject::AddString(const char * aName, const char * aString)
{
	rapidjson::Value name(aName, myDocument.GetAllocator());
	rapidjson::Value value(aString, myDocument.GetAllocator());
	myDocument.AddMember(name, value.Move(), myDocument.GetAllocator());
}

void JsonObject::AddObject(const char * aName, JsonObject& aObject)
{
	rapidjson::Value v(aObject.myDocument, myDocument.GetAllocator());
	rapidjson::Value name(aName, myDocument.GetAllocator());
	myDocument.AddMember(name, v, myDocument.GetAllocator());
}

void JsonObject::AddArray(const char * aName, JsonArray & aArray)
{
	rapidjson::Value name(aName, myDocument.GetAllocator());
	myDocument.AddMember(name, aArray.myArray, myDocument.GetAllocator());
}

void JsonObject::AddArray(const char * aName)
{
	rapidjson::Value name(aName, myDocument.GetAllocator());
	JsonArray arr;
	myDocument.AddMember(name, arr.myArray, myDocument.GetAllocator());
}

bool JsonObject::Find(const char * aName) const
{
	return myDocument.FindMember(aName) != myDocument.MemberEnd();
}

void JsonArray::PushBackObject(JsonObject& aObject)
{
	rapidjson::Value v(aObject.myDocument, myDocument.GetAllocator());
	myArray.PushBack(v, myDocument.GetAllocator());
}

void JsonArray::PushBackArray(JsonArray & aArray)
{
	rapidjson::Value v;
	v.CopyFrom(aArray.myArray, myDocument.GetAllocator());
	myArray.PushBack(v, myDocument.GetAllocator());
}