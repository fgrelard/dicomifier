/*************************************************************************
 * Dicomifier - Copyright (C) Universite de Strasbourg
 * Distributed under the terms of the CeCILL-B license, as published by
 * the CEA-CNRS-INRIA. Refer to the LICENSE file or to
 * http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.html
 * for details.
 ************************************************************************/

#include <fstream>
#include <memory>

#include <odil/DataSet.h>
#include <odil/json_converter.h>
#include <odil/Reader.h>

#include "lib/bruker/json_converter.h"

int main(int argc, char** argv)
{
    for(int i=1; i<argc; ++i)
    {
        std::ifstream stream(argv[i], std::ios::in | std::ios::binary);

        std::pair<odil::DataSet, odil::DataSet> file;
        try
        {
            file = odil::Reader::read_file(stream);
        }
        catch(std::exception & e)
        {
            std::cout << "Could not read " << argv[i] << ": " << e.what() << "\n";
            continue;
        }

        auto const json = odil::as_json(file.second);

        std::cout << dicomifier::bruker::as_string(json) << std::endl;
    }

    return EXIT_SUCCESS;
}
