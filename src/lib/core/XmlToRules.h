/*************************************************************************
 * Dicomifier - Copyright (C) Universite de Strasbourg
 * Distributed under the terms of the CeCILL-B license, as published by
 * the CEA-CNRS-INRIA. Refer to the LICENSE file or to
 * http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.html
 * for details.
 ************************************************************************/

#ifndef _44324aea_e111_4987_bae0_f773fd192cd1
#define _44324aea_e111_4987_bae0_f773fd192cd1

#include <vector>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "core/Object.h"

namespace dicomifier
{
   
namespace XmlToRules
{

/**
 * @brief Load given XML file and return a list of Object elements
 */
std::vector<Object::Pointer> Convert(std::string const & filename);

/**
 * @brief Convert given XML tree into list of Object elements
 */
std::vector<Object::Pointer> Convert(boost::property_tree::ptree & pt);

} // namespace XmlToRules
   
} // namespace dicomifier

#endif // _44324aea_e111_4987_bae0_f773fd192cd1
