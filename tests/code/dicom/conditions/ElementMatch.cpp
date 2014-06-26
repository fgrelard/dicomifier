#define BOOST_TEST_MODULE ModuleElementMatch
#include <boost/test/unit_test.hpp>

#include <dcmtk/dcmdata/dctk.h>

#include "dicom/conditions/ElementMatch.h"

struct TestDataCS
{
    DcmDataset * dataset;
 
    TestDataCS()
    {
        dataset = new DcmDataset();
        // Insert testing value
        std::string test = "value1\\value2\\value3";
        dataset->putAndInsertOFStringArray(DCM_Modality, test.c_str());     // insert CS
    }
 
    ~TestDataCS()
    {
        delete dataset;
    }
};

BOOST_FIXTURE_TEST_SUITE(ElementCS, TestDataCS)

BOOST_AUTO_TEST_CASE(MatchCS01)
{
    auto testmatch = 
        router::conditions::ElementMatch<EVR_CS>::New(dataset, 
                                                      DCM_Modality, 
                                                      {"value1", "value2", "value3"});
    BOOST_CHECK_EQUAL(testmatch->eval(), true);
}

BOOST_AUTO_TEST_CASE(MatchCS02)
{
    auto testmatch = 
        router::conditions::ElementMatch<EVR_CS>::New(dataset, 
                                                      DCM_Modality, 
                                                      {"value1", "badValue", "value3"});
    BOOST_CHECK_EQUAL(testmatch->eval(), false);
}

BOOST_AUTO_TEST_CASE(MatchCS03)
{
    auto testmatch = 
        router::conditions::ElementMatch<EVR_CS>::New(dataset, 
                                                      DCM_PatientSex, 
                                                      {"value1", "value2", "value3"});
    BOOST_CHECK_EQUAL(testmatch->eval(), false);
}

BOOST_AUTO_TEST_SUITE_END()

struct TestDataDS
{
    DcmDataset * dataset;
 
    TestDataDS()
    {
        dataset = new DcmDataset();
        // Insert testing value
        dataset->putAndInsertOFStringArray(DCM_PatientWeight, "60");        // insert DS
    }
 
    ~TestDataDS()
    {
        delete dataset;
    }
};

BOOST_FIXTURE_TEST_SUITE(ElementDS, TestDataDS)

BOOST_AUTO_TEST_CASE(MatchDS01)
{
    auto testmatch = 
        router::conditions::ElementMatch<EVR_DS>::New(dataset, 
                                                      DCM_PatientWeight, 
                                                      (Float64)60);
    BOOST_CHECK_EQUAL(testmatch->eval(), true);
}

BOOST_AUTO_TEST_SUITE_END()

struct TestDataIS
{
    DcmDataset * dataset;
 
    TestDataIS()
    {
        dataset = new DcmDataset();
        // Insert testing value
        dataset->putAndInsertOFStringArray(DCM_StageNumber, "12");          // insert IS
    }
 
    ~TestDataIS()
    {
        delete dataset;
    }
};

BOOST_FIXTURE_TEST_SUITE(ElementIS, TestDataIS)

BOOST_AUTO_TEST_CASE(MatchIS01)
{
    auto testmatch = 
        router::conditions::ElementMatch<EVR_IS>::New(dataset, 
                                                      DCM_StageNumber, 
                                                      (Sint32)12);
    BOOST_CHECK_EQUAL(testmatch->eval(), true);
}

BOOST_AUTO_TEST_SUITE_END()
