/*************************************************************************
 * Dicomifier - Copyright (C) Universite de Strasbourg
 * Distributed under the terms of the CeCILL-B license, as published by
 * the CEA-CNRS-INRIA. Refer to the LICENSE file or to
 * http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.html
 * for details.
 ************************************************************************/

#define BOOST_TEST_MODULE ModuleJavascriptVM
#include <boost/test/unit_test.hpp>

#include <map>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <jsoncpp/json/json.h>
#include <v8.h>

#include "core/DicomifierException.h"
#include "javascript/JavascriptVM.h"

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Constructor / Destructor
 */
BOOST_AUTO_TEST_CASE(Constructor)
{
    dicomifier::javascript::JavascriptVM * vm =
            new dicomifier::javascript::JavascriptVM();

    BOOST_REQUIRE(vm != NULL);
}

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Run script
 */
BOOST_AUTO_TEST_CASE(Run)
{
    dicomifier::javascript::JavascriptVM vm;

    v8::Local<v8::Value> result = vm.run("JSON.stringify({ 'hello' : 'world' });");
    v8::String::Utf8Value utf8(result);

    std::stringstream stream;
    stream << *utf8;

    BOOST_CHECK_EQUAL("{\"hello\":\"world\"}", stream.str());
}

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Run script file
 */
struct TestDataRunFile
{
    std::string filename;

    TestDataRunFile():
        filename("./tmp_test_moduleJavaScriptVM.js")
    {
        std::ofstream myfile;
        myfile.open(filename);
        myfile << "JSON.stringify({ 'hello' : 'world' });\n";
        myfile.close();
    }

    ~TestDataRunFile()
    {
        remove(filename.c_str());
    }
};

BOOST_FIXTURE_TEST_CASE(RunFile, TestDataRunFile)
{
    dicomifier::javascript::JavascriptVM vm;

    v8::Local<v8::Value> result = vm.run_file(filename);
    v8::String::Utf8Value utf8(result);

    std::stringstream stream;
    stream << *utf8;

    BOOST_CHECK_EQUAL("{\"hello\":\"world\"}", stream.str());
}

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Test function divideArray descibes into function.js
 */
BOOST_AUTO_TEST_CASE(Functions_divideArray)
{
    dicomifier::javascript::JavascriptVM vm;

    std::stringstream streaminput;
    streaminput << "var array1 = [ 8, 27, 64 ];"
                << "var array2 = [ 2, 3, 4 ];"
                << "String(divideArray(array1, array2));";

    v8::Local<v8::Value> result = vm.run(streaminput.str());
    v8::String::Utf8Value utf8(result);

    std::stringstream stream;
    stream << *utf8;

    BOOST_CHECK_EQUAL("4,9,16", stream.str());
}

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Test function dictionaryMapper descibes into function.js
 */
BOOST_AUTO_TEST_CASE(Functions_dictionaryMapper)
{
    dicomifier::javascript::JavascriptVM vm;

    std::map<std::string, std::string> const testedvalues =
            { { "first", "hello" }, { "second", "world" } };

    std::string const dict =
            "var dictionary = { 'first': 'hello', 'second': 'world' };";

    typedef std::map<std::string, std::string>::const_iterator const_iterator;
    for (const_iterator it = testedvalues.begin();
         it != testedvalues.end(); ++it)
    {
        std::stringstream streaminput;
        streaminput << dict << "String(['" << it->first
                    << "'].map(dictionaryMapper(dictionary)));";

        v8::Local<v8::Value> result = vm.run(streaminput.str());
        v8::String::Utf8Value utf8(result);

        std::stringstream stream;
        stream << *utf8;

        BOOST_CHECK_EQUAL(it->second, stream.str());
    }
}

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Test function dateTimeMapper descibes into function.js
 */
BOOST_AUTO_TEST_CASE(Functions_dateTimeMapper)
{
    dicomifier::javascript::JavascriptVM vm;

    std::map<std::string, std::string> const testedvalues =
            { { "date", "20150219" }, { "time", "085940" } };

    typedef std::map<std::string, std::string>::const_iterator const_iterator;
    for (const_iterator it = testedvalues.begin();
         it != testedvalues.end(); ++it)
    {
        std::stringstream streaminput;
        streaminput << "var array = [ '2015-02-19T09:59:40,234+0100' ];"
                    << "String(array.map(dateTimeMapper('"
                    << it->first << "')));";

        v8::Local<v8::Value> result = vm.run(streaminput.str());
        v8::String::Utf8Value utf8(result);

        std::stringstream stream;
        stream << *utf8;

        BOOST_CHECK_EQUAL(it->second, stream.str());
    }
}

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Test function toDicom descibes into function.js
 */
struct TestDataToDicom
{
    dicomifier::javascript::JavascriptVM vm;
    std::stringstream streamscript;

    TestDataToDicom()
    {
        // Create input
        std::stringstream streambruker;
        streambruker << "{ "
                     << "\"VisuCoreFrameCount\": [ \"3\" ],"
                     << "\"VisuCoreSize\": [ 256, 256 ],"
                     << "\"VisuSubjectId\" : [ \"MySubjectID\" ],"
                     << "\"VisuSubjectName\" : [ \"MyName\" ],"
                     << "\"VisuSubjectSex\" : [ \"MALE\" ],"
                     << "\"VisuExperimentNumber\": [ \"1\" ],"
                     << "\"VisuFGOrderDesc\": [[3, \"FG_SLICE\", \"\", 0, 2]],"
                     << "\"VisuFGOrderDescDim\": [ \"1\" ],"
                     << "\"VisuGroupDepVals\": [[ \"VisuCoreOrientation\", 0 ],"
                     << "[ \"VisuCorePosition\", 0 ]]"
                     << "}";

        Json::Value jsonbruker;
        streambruker >> jsonbruker;

        std::stringstream stream;
        stream << "dicomifier.inputs[0] = "
               << jsonbruker.toStyledString() << ";";

        vm.run(stream.str());

        streamscript << "var dataset = {}; "
                     << "var brukerDataset = dicomifier.inputs[0]; "
                     << "var frameGroups = "
                     <<     "dicomifier.getFrameGroups(brukerDataset); "
                     << "var indexGenerator = "
                     <<     "new dicomifier.frameIndexGenerator(frameGroups); ";
    }

    ~TestDataToDicom()
    {

    }
};

BOOST_FIXTURE_TEST_CASE(Functions_toDicom, TestDataToDicom)
{
    for (unsigned int i = 1; i < 4; ++i)
    {
        std::stringstream stream;
        stream << streamscript.str()
               << "toDicom(indexGenerator, dataset, 'PatientID', "
               <<         "brukerDataset, 'VisuSubjectId', " << i << "); "
               << "JSON.stringify(dataset);";

        auto result = vm.run(stream.str());
        v8::String::Utf8Value utf8(result);

        std::stringstream datasetstream;
        datasetstream << *utf8;

        BOOST_CHECK_EQUAL(
                datasetstream.str(),
                "{\"00100020\":{\"vr\":\"LO\",\"Value\":[\"MySubjectID\"]}}");
    }
}

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Test function toDicom descibes into function.js
 */
BOOST_FIXTURE_TEST_CASE(Functions_toDicom_NoValue, TestDataToDicom)
{
    // Type = 1: see test Functions_toDicom_NoValue_type1
    // Type = 2
    {
        std::stringstream stream;
        stream << streamscript.str()
               << "toDicom(indexGenerator, dataset, 'PatientID', "
               <<         "brukerDataset, null, 2); "
               << "JSON.stringify(dataset);";

        auto result = vm.run(stream.str());
        v8::String::Utf8Value utf8(result);

        std::stringstream datasetstream;
        datasetstream << *utf8;

        BOOST_CHECK_EQUAL(datasetstream.str(), "{\"00100020\":{\"vr\":\"LO\"}}");
    }
    // Type = 3
    {
        std::stringstream stream;
        stream << streamscript.str()
               << "toDicom(indexGenerator, dataset, 'PatientID', "
               <<         "brukerDataset, null, 3); "
               << "JSON.stringify(dataset);";

        auto result = vm.run(stream.str());
        v8::String::Utf8Value utf8(result);

        std::stringstream datasetstream;
        datasetstream << *utf8;

        BOOST_CHECK_EQUAL(datasetstream.str(), "{}");
    }
}

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Test function toDicom descibes into function.js
 */
BOOST_FIXTURE_TEST_CASE(Functions_toDicom_PN, TestDataToDicom)
{
    std::stringstream stream;
    stream << streamscript.str()
           << "toDicom(indexGenerator, dataset, 'PatientName', "
           <<         "brukerDataset, 'VisuSubjectName', 2); "
           << "JSON.stringify(dataset);";

    auto result = vm.run(stream.str());
    v8::String::Utf8Value utf8(result);

    std::stringstream datasetstream;
    datasetstream << *utf8;

    BOOST_CHECK_EQUAL(datasetstream.str(),
      "{\"00100010\":{\"vr\":\"PN\",\"Value\":[{\"Alphabetic\":\"MyName\"}]}}");
}

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Test function toDicom descibes into function.js
 */
BOOST_FIXTURE_TEST_CASE(Functions_toDicom_Setter, TestDataToDicom)
{
    std::stringstream stream;
    stream << streamscript.str()
           << "toDicom(indexGenerator, dataset, 'PatientSex', "
           <<         "brukerDataset, 'VisuSubjectSex', 2, "
           <<         "dictionaryMapper({'MALE': 'M', 'FEMALE': 'F',"
           <<                           "'UNDEFINED': 'O', 'UNKNOWN': 'O' })); "
           << "JSON.stringify(dataset);";

    auto result = vm.run(stream.str());
    v8::String::Utf8Value utf8(result);

    std::stringstream datasetstream;
    datasetstream << *utf8;

    BOOST_CHECK_EQUAL(
                datasetstream.str(),
                "{\"00100040\":{\"vr\":\"CS\",\"Value\":[\"M\"]}}");
}

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Test function toDicom descibes into function.js
 */
BOOST_FIXTURE_TEST_CASE(Functions_toDicom_Getter, TestDataToDicom)
{
    std::stringstream stream;
    stream << streamscript.str()
           << "toDicom(indexGenerator, dataset, 'PatientSex', "
           <<         "brukerDataset, 'VisuSubjectSex', 2, undefined, "
           <<         "function(brukerDS) { return ['F']; }); "
           << "JSON.stringify(dataset);";

    auto result = vm.run(stream.str());
    v8::String::Utf8Value utf8(result);

    std::stringstream datasetstream;
    datasetstream << *utf8;

    BOOST_CHECK_EQUAL(
                datasetstream.str(),
                "{\"00100040\":{\"vr\":\"CS\",\"Value\":[\"F\"]}}");
}

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Test function toDicom descibes into function.js
 */
BOOST_FIXTURE_TEST_CASE(Functions_toDicom_NullValue, TestDataToDicom)
{
    // type 1: see test Functions_toDicom_NullValueType1
    // type 2
    {
    std::stringstream stream;
    stream << streamscript.str()
           << "toDicom(indexGenerator, dataset, 'PatientSex', "
           <<         "brukerDataset, 'VisuSubjectSex', 2, undefined, "
           <<         "function(brukerDS) { return null; }); "
           << "JSON.stringify(dataset);";

    auto result = vm.run(stream.str());
    v8::String::Utf8Value utf8(result);

    std::stringstream datasetstream;
    datasetstream << *utf8;

    BOOST_CHECK_EQUAL(
                datasetstream.str(),
                "{\"00100040\":{\"vr\":\"CS\"}}");
    }
    // type 3
    {
    std::stringstream stream;
    stream << streamscript.str()
           << "toDicom(indexGenerator, dataset, 'PatientSex', "
           <<         "brukerDataset, 'VisuSubjectSex', 3, undefined, "
           <<         "function(brukerDS) { return null; }); "
           << "JSON.stringify(dataset);";

    auto result = vm.run(stream.str());
    v8::String::Utf8Value utf8(result);

    std::stringstream datasetstream;
    datasetstream << *utf8;

    BOOST_CHECK_EQUAL(
                datasetstream.str(),
                "{\"00100040\":{\"vr\":\"CS\"}}");
    }
}

/*************************** TEST Error *********************************/
/**
 * Error test case: Generate an exception
 */
BOOST_AUTO_TEST_CASE(Exception)
{
    dicomifier::javascript::JavascriptVM vm;

    BOOST_REQUIRE_THROW(vm.run("throw new dicomifier.Exception('my fault');"),
                        dicomifier::DicomifierException);
}

/*************************** TEST Error *********************************/
/**
 * Error test case: Test function dictionaryMapper descibes into function.js
 *                  Unknown value
 */
BOOST_AUTO_TEST_CASE(Functions_dictionaryMapper_UnknownValue)
{
    dicomifier::javascript::JavascriptVM vm;

    std::stringstream streaminput;
    streaminput << "var dictionary = { 'first': 'hello', 'second': 'world' };"
                << "String(['UnknownValue'].map(dictionaryMapper(dictionary)));";

    BOOST_REQUIRE_THROW(vm.run(streaminput.str()),
                        dicomifier::DicomifierException);
}

/*************************** TEST Error *********************************/
/**
 * Error test case: Test function dateTimeMapper descibes into function.js
 *                  Unknown format
 */
BOOST_AUTO_TEST_CASE(Functions_dateTimeMapper_UnknownFormat)
{
    dicomifier::javascript::JavascriptVM vm;

    std::stringstream streaminput;
    streaminput << "var array = [ '2015-02-19T09:59:40,234+0100' ];"
                << "String(array.map(dateTimeMapper('Unknown')));";

    BOOST_REQUIRE_THROW(vm.run(streaminput.str()),
                        dicomifier::DicomifierException);
}

/*************************** TEST Error *********************************/
/**
 * Error test case: Test function toDicom descibes into function.js
 *                  No value for Type 1
 */
BOOST_FIXTURE_TEST_CASE(Functions_toDicom_NoValue_type1, TestDataToDicom)
{
    std::stringstream stream;
    stream << streamscript.str()
           << "toDicom(indexGenerator, dataset, 'PatientID', "
           <<         "brukerDataset, null, 1); "
           << "JSON.stringify(dataset);";

    BOOST_REQUIRE_THROW(vm.run(stream.str()), dicomifier::DicomifierException);
}

/*************************** TEST Error *********************************/
/**
 * Error test case: Test function toDicom descibes into function.js
 *                  Unknown type
 */
BOOST_FIXTURE_TEST_CASE(Functions_toDicom_UnknownType, TestDataToDicom)
{
    std::stringstream stream;
    stream << streamscript.str()
           << "toDicom(indexGenerator, dataset, 'PatientID', "
           <<         "brukerDataset, null, 9); "
           << "JSON.stringify(dataset);";

    BOOST_REQUIRE_THROW(vm.run(stream.str()), dicomifier::DicomifierException);
}

/*************************** TEST Error *********************************/
/**
 * Error test case: Test function toDicom descibes into function.js
 *                  Unknown DICOM tag
 */
BOOST_FIXTURE_TEST_CASE(Functions_toDicom_UnknownDicomTag, TestDataToDicom)
{
    std::stringstream stream;
    stream << streamscript.str()
           << "toDicom(indexGenerator, dataset, 'UnknownTag', "
           <<         "brukerDataset, null, 1); "
           << "JSON.stringify(dataset);";

    BOOST_REQUIRE_THROW(vm.run(stream.str()), dicomifier::DicomifierException);
}

/*************************** TEST Error *********************************/
/**
 * Error test case: Test function toDicom descibes into function.js
 *                  Null value and type 1
 */
BOOST_FIXTURE_TEST_CASE(Functions_toDicom_NullValueType1, TestDataToDicom)
{
    std::stringstream stream;
    stream << streamscript.str()
           << "toDicom(indexGenerator, dataset, 'PatientSex', "
           <<         "brukerDataset, 'VisuSubjectSex', 1, undefined, "
           <<         "function(brukerDS) { return null; }); "
           << "JSON.stringify(dataset);";

    BOOST_REQUIRE_THROW(vm.run(stream.str()), dicomifier::DicomifierException);
}

/*************************** TEST Error *********************************/
/**
 * Error test case: Test function toDicom descibes into function.js
 *                  Empty value and type 1
 */
BOOST_FIXTURE_TEST_CASE(Functions_toDicom_EmptyValueType1, TestDataToDicom)
{
    std::stringstream stream;
    stream << streamscript.str()
           << "toDicom(indexGenerator, dataset, 'PatientSex', "
           <<         "brukerDataset, 'VisuSubjectSex', 1, undefined, "
           <<         "function(brukerDS) { return []; }); "
           << "JSON.stringify(dataset);";

    BOOST_REQUIRE_THROW(vm.run(stream.str()), dicomifier::DicomifierException);
}