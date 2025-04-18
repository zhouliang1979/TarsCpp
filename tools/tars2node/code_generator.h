﻿/**
 * Tencent is pleased to support the open source community by making Tars available.
 *
 * Copyright (C) 2016THL A29 Limited, a Tencent company. All rights reserved.
 *
 * Licensed under the BSD 3-Clause License (the "License"); you may not use this file except 
 * in compliance with the License. You may obtain a copy of the License at
 *
 * https://opensource.org/licenses/BSD-3-Clause
 *
 * Unless required by applicable law or agreed to in writing, software distributed 
 * under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR 
 * CONDITIONS OF ANY KIND, either express or implied. See the License for the 
 * specific language governing permissions and limitations under the License.
 */

#ifndef _CODEGENERATOR_H
#define _CODEGENERATOR_H

#include "parse.h"
#include "util/tc_file.h"
#include "util/tc_encoder.h"
#include <set>

#ifndef TAB
    #define TAB     g_parse->getTab()
#endif

#ifndef INC_TAB
    #define INC_TAB g_parse->incTab()
#endif

#ifndef DEL_TAB
    #define DEL_TAB g_parse->delTab()
#endif

#define TO_LOWER_STRING(str) TC_Common::lower(str)

#define DEFINE_STRING(str) string(CSTR(str))
#define CSTR(str) #str
#define IDL_NAMESPACE_STR DEFINE_STRING(IDL_NAMESPACE)

#define GET_CONST_GRAMMAR_PTR_V(name, ptr) \
    ptr->getConst##name##Ptr()
#define GET_CONST_GRAMMAR_PTR_BASE(name, ptr) \
    GET_CONST_GRAMMAR_PTR_V(name, ptr)
#define GET_CONST_GRAMMAR_PTR(ptr) \
    GET_CONST_GRAMMAR_PTR_BASE(GRAMMAR_NAME, ptr)

#define CONST_GRAMMAR_V(name, val) \
    Const##name::val
#define CONST_GRAMMAR_BASE(name, val) \
    CONST_GRAMMAR_V(name, val)
#define CONST_GRAMMAR(val) \
    CONST_GRAMMAR_BASE(GRAMMAR_NAME, val)

#define PROTOCOL_V(space, protocol, type) \
    space + "Stream." + protocol + "." + TC_Common::upper(protocol) + "_" + type
#define PROTOCOL_SIMPLE PROTOCOL_V(IDL_NAMESPACE_STR, PROTOCOL_NAME, "SIMPLE")
#define PROTOCOL_COMPLEX PROTOCOL_V(IDL_NAMESPACE_STR, PROTOCOL_NAME, "COMPLEX")
#define PROTOCOL_VAR TO_LOWER_STRING(PROTOCOL_NAME)

#define PROTOCOL_VJ(space, protocol, type) \
    space + "Stream." + protocol + "." + type + "_VERSION"
#define PROTOCOL_JSON PROTOCOL_VJ(IDL_NAMESPACE_STR, PROTOCOL_NAME, "JSON")

#define DISABLE_ESLINT "/* eslint-disable */"
#define DISABLE_TSLINT "/* tslint:disable */"

using namespace TC_NAMESPACE;

class CodeGenerator
{
public:
    CodeGenerator()
        : uiNameIndex(0),
          _sRpcPath(RPC_MODULE_PATH),
          _sStreamPath(STREAM_MODULE_PATH),
          _sToPath("./"),
          _bClient(false),
          _bServer(false),
          _bWeb(false),
          _bRecursive(false),
//          _bUseSpecialPath(false),
          _iLongType(Number),
          _bStringBinaryEncoding(false),
          _bEnumReverseMappings(false),
          _bMinimalMembers(false),
          _bTS(false),
          _bDTS(false),
          _iOptimizeLevel(O0) {}

    void createFile(const string &file, const bool bEntry = true);

    void setRpcPath(const string & sPath) { _sRpcPath = sPath; }

    void setStreamPath(const string & sPath) { _sStreamPath = sPath; }

    void setEnableClient(bool bEnable) { _bClient = bEnable; }

    void setEnableServer(bool bEnable) { _bServer = bEnable; }

    void setEnableWeb(bool bEnable) { _bWeb = bEnable; }

    void setTargetPath(const string & sPath) { _sToPath = sPath + "/"; }

    void setRecursive(bool bEnable) { _bRecursive = bEnable; }

//    void setUseSpecialPath(bool bEnable) { _bUseSpecialPath = bEnable; }

    void setLongType(int iLongType) { _iLongType = iLongType; }

    void setStringBinaryEncoding(bool bEnable) { _bStringBinaryEncoding = bEnable; }

    void setEnumReverseMappings(bool bEnable) { _bEnumReverseMappings = bEnable; }

    void setMinimalMembers(bool bEnable) { _bMinimalMembers = bEnable; }

    void setDependent(set<string>& deps) { _depMembers = deps; }

    void setEnableTS(bool bEnable) { _bTS = bEnable; }

    void setEnableDTS(bool bEnable) { _bDTS = bEnable; }

    void setOptimize(int iLevel) { _iOptimizeLevel = iLevel; }

    enum OPTIMIZE_LEVEL {O0 = 0, Os};

    enum LONG_TYPE {Number = 0, String, BigInt};

private:
    struct ImportFileType
    {
        enum TYPE_T {EN_ENUM = 10000, EN_ENUM_VALUE, EN_STRUCT};
        int         iType;
        string      sNamespace;
        string      sTypeName;
        string      sName;
    };

    struct ImportFile 
    {
        string                      sFile;
        string                      sModule;
        map<string, ImportFileType> mapVars;
    };

    uint32_t uiNameIndex;

    map<string, ImportFile> _mapFiles;

    set<string> _depMembers;

    void   scan(const string & sFile, bool bNotPrefix);

    void   addTarsPingForProxy(const ContextPtr &cPtr);

    string makeName();

    string findName(const string & sNamespace, const string & sName, const bool &bBase = false);

private:
    string toFunctionName(const TypeIdPtr & pPtr, const string &sAction);

    string toObjectString(const TypeIdPtr &pPtr);

    string getDataType(const TypePtr& pPtr, const bool &bCastEnumAsAny = false);

    string getClassName(const TypePtr& pPtr);

    string getTsType(const TypePtr &pPtr, const bool bStream = true, const bool bBase = false);

    string getDefault(const TypeIdPtr & pPtr, const string &sDefault, const string & sNamespace, const bool &bGlobal = true, const bool &bCastEnumAsAny = false);

private:
    string generateJS(const StructPtr &pPtr, const string &sNamespace, bool &bNeedAssert, bool &bQuickFunc);

    string generateJS(const ConstPtr &pPtr, const string &sNamespace, bool &bNeedStream);

    string generateJS(const EnumPtr &pPtr, const string &sNamespace);

    string generateJS(const NamespacePtr &pPtr, bool &bNeedStream, bool &bNeedAssert, bool &bQuickFunc);

    bool   generateJS(const ContextPtr &pPtr);

private:
    string generateTS(const StructPtr &pPtr, const string &sNamespace, bool &bNeedAssert, bool &bQuickFunc);

    string generateTS(const ConstPtr &pPtr, const string &sNamespace, bool &bNeedStream);

    string generateTS(const EnumPtr &pPtr, const string &sNamespace);

    string generateTS(const NamespacePtr &pPtr, bool &bNeedStream, bool &bNeedAssert, bool &bQuickFunc);

    string generateTS(const NamespacePtr &pPtr, const string &sContent);

    void   generateTS(const ContextPtr &cPtr);

private:
    string generateJSProxy(const NamespacePtr &nPtr, bool &bNeedRpc, bool &bNeedStream);

    string generateJSProxy(const NamespacePtr &nPtr, const InterfacePtr &pPtr);

    string generateJSProxy(const NamespacePtr &nPtr, const InterfacePtr &pPtr, const OperationPtr &oPtr);

    bool   generateJSProxy(const ContextPtr &pPtr);

private:
    string generateTSProxy(const NamespacePtr &pPtr, bool &bNeedStream, bool &bNeedRpc);

    string generateTSProxy(const NamespacePtr &nPtr, const InterfacePtr &pPtr);

    string generateTSProxy(const NamespacePtr &nPtr, const InterfacePtr &pPtr, const OperationPtr &oPtr);

    bool   generateTSProxy(const ContextPtr &pPtr);

private:
    string generateJSServer(const NamespacePtr &pPtr, bool &bNeedStream, bool &bNeedRpc, bool &bNeedAssert);

    string generatePing(const NamespacePtr &nPtr, const InterfacePtr &pPtr);

    string generateAsync(const NamespacePtr &nPtr, const InterfacePtr &pPtr, const OperationPtr &oPtr);

    string generateDispatch(const NamespacePtr &nPtr, const InterfacePtr &pPtr, const OperationPtr &oPtr);

    string generateJSServer(const NamespacePtr &nPtr, const InterfacePtr &pPtr, const OperationPtr &oPtr);

    string generateJSServer(const InterfacePtr &pPtr, const NamespacePtr &nPtr);

    bool   generateJSServer(const ContextPtr &pPtr);

private:
    string generateTSServerAsync(const NamespacePtr &nPtr, const InterfacePtr &pPtr, const OperationPtr &oPtr);

    string generateTSServerDispatch(const NamespacePtr &nPtr, const InterfacePtr &pPtr, const OperationPtr &oPtr);

    string generateTSServer(const NamespacePtr &pPtr, bool &bNeedStream, bool &bNeedRpc, bool &bNeedAssert);

    string generateTSServer(const InterfacePtr &pPtr, const NamespacePtr &nPtr);

    bool   generateTSServer(const ContextPtr &pPtr);

private:
    string generateJSServerImp(const NamespacePtr &nPtr, const InterfacePtr &pPtr, const OperationPtr &oPtr);

    string generateJSServerImp(const NamespacePtr &nPtr, const InterfacePtr &pPtr);

    string generateJSServerImp(const ContextPtr &cPtr, const NamespacePtr &nPtr);

    void   generateJSServerImp(const ContextPtr &cPtr);

private:
    void   generateTSServerImp(const ContextPtr &cPtr);

private:
    string generateDTS(const StructPtr &pPtr, const string &sNamespace);

    string generateDTS(const ConstPtr &pPtr, const string &sNamespace, bool &bNeedStream);

    string generateDTS(const EnumPtr &pPtr, const string &sNamespace);

    string generateDTS(const NamespacePtr &pPtr, bool &bNeedStream);

    string generateDTS(const NamespacePtr &pPtr, const string &sContent);

    void   generateDTS(const ContextPtr &cPtr);

private:
    string generateDTSServer(const NamespacePtr &pPtr, bool &bNeedStream, bool &bNeedRpc);

    string generateDTSServer(const NamespacePtr &nPtr, const InterfacePtr &pPtr);

    void   generateDTSServer(const ContextPtr &cPtr);

private:
    string generateDTSProxy(const InterfacePtr &pPtr);

    string generateDTSProxy(const NamespacePtr &pPtr, bool &bNeedStream, bool &bNeedRpc);

    void   generateDTSProxy(const ContextPtr &pPtr);

private:
    void   makeUTF8File(const string & sFileName, const string & sFileContent);

    bool   isSimple(const TypePtr & pPtr) const;

    bool   isBinBuffer(const TypePtr & pPtr) const;

    string representArgument(const TypePtr& pPtr) const;

    bool   isDependent(const string & sNamespace, const string & sName) const;

    string getRealFileInfo(const string & sPath);

    string printHeaderRemark(const string & sTypeName);

private:
    string _sRpcPath;

    string _sStreamPath;

    string _sToPath;

    bool   _bClient;

    bool   _bServer;

    bool   _bWeb;

    bool   _bRecursive;

//    bool   _bUseSpecialPath;

    int    _iLongType;

    bool   _bStringBinaryEncoding;

    bool   _bEnumReverseMappings;

    bool   _bMinimalMembers;

    bool   _bEntry;

    string _sIdlFile;

    bool   _bTS;

    bool   _bDTS;

    int    _iOptimizeLevel;
};

#endif