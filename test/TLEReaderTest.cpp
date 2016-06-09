//
// Created by melge on 08.06.2016.
//

#include "TLEReaderTest.h"

void TLEReaderTest::simpleTleReadTest()
{
    std::map<int, Orbit> orbits =
            TLEReader::ReadSatellitesFromFile("data/test_tle.tle");

    CPPUNIT_ASSERT(orbits.size() == 2);

    CPPUNIT_ASSERT(orbits[25544].GetSatelliteName() == "ISS (ZARYA)");
    CPPUNIT_ASSERT(orbits[25543].GetSatelliteName() == "No name");
}

