/***************************************************************************
 *   Copyright (C) 2018 by Robert Middleton                                *
 *   robert.middleton@rm5248.com                                           *
 *                                                                         *
 *   This file is part of the dbus-cxx library.                            *
 *                                                                         *
 *   The dbus-cxx library is free software; you can redistribute it and/or *
 *   modify it under the terms of the GNU General Public License           *
 *   version 3 as published by the Free Software Foundation.               *
 *                                                                         *
 *   The dbus-cxx library is distributed in the hope that it will be       *
 *   useful, but WITHOUT ANY WARRANTY; without even the implied warranty   *
 *   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU   *
 *   General Public License for more details.                              *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this software. If not see <http://www.gnu.org/licenses/>.  *
 ***************************************************************************/
#ifndef DBUSCXX_CODEGENERATOR_H
#define DBUSCXX_CODEGENERATOR_H

#include <expat.h>
#include <string>
#include <map>
#include <stack>
#include <vector>
#include <cppgenerate/class.h>
#include <dbus-cxx/signatureiterator.h>

#include "signal-info.h"
#include "node-info.h"
#include "interface-info.h"
#include "property-info.h"

namespace DBus{

class CodeGenerator {
public:
    CodeGenerator();
    ~CodeGenerator();

    void setXML( const std::string& xmlDocument );

    bool parse();

    void generateProxyClasses( bool outputToFile, const std::string& output_directory );

    void generateAdapterClasses( bool outputToFile, const std::string& output_directory );

private:
    void start_element( std::string name, std::map<std::string,std::string> attrs );
    void end_element( std::string name );
    void handle_node_tag( std::map<std::string,std::string>& tagAttrs );
    void handle_method_tag( std::map<std::string,std::string>& tagAttrs );
    void handle_signal_tag( std::map<std::string,std::string>& tagAttrs );
    void handle_arg_tag( std::string parentElement, std::map<std::string,std::string>& tagAttrs );
    void handle_property_tag( std::map<std::string,std::string>& tagAttrs );

    static void start_element_handler(void* user_data, const XML_Char* name, const XML_Char** attrs );
    static void end_element_handler( void* userData, const XML_Char* name );

    std::tuple<std::string,std::vector<std::string>> getTemplateArgsFromSignature( SignatureIterator iter );

    void generateProxyMethods( cppgenerate::Class* cls,
                               cppgenerate::Constructor* constructor,
                               std::vector<MethodInfo> methods );
    void generateProxySignals( cppgenerate::Class* cls,
                               cppgenerate::Constructor* constructor,
                               std::vector<SignalInfo> signals );
    void generateProxyProperties( cppgenerate::Class* cls,
                                  cppgenerate::Constructor* constructor,
                                  std::vector<PropertyInfo> properties );

    void generateAdapterMethods( cppgenerate::Class* cls,
                               cppgenerate::Constructor* constructor,
                                 cppgenerate::Class* virtualClass,
                               std::vector<MethodInfo> methods );
    void generateAdapterSignals( cppgenerate::Class* cls,
                               cppgenerate::Constructor* constructor,
                               std::vector<SignalInfo> signals );
    void generateAdapterProperties( cppgenerate::Class* cls,
                                  cppgenerate::Constructor* constructor,
                                  std::vector<PropertyInfo> properties );

private:
    XML_Parser m_parser;
    std::string m_xmlData;
    std::stack<std::string> m_tagStack;
    SignalInfo m_currentSignal;
    int m_argNum;
    NodeInfo m_rootNode;
    InterfaceInfo m_currentInterface;
    MethodInfo m_currentMethodInfo;
    bool m_ignoreInterface;
};

}

#endif
