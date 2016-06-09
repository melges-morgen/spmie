//
// Created by melge on 08.06.2016.
//

#ifndef SPMIE_COMPLEXNUMBERTEST_H
#define SPMIE_COMPLEXNUMBERTEST_H

#include <string>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>

#include "librsim/astro/TLEReader.h"

class TLEReaderTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( TLEReaderTest );
        CPPUNIT_TEST( simpleTleReadTest );
  CPPUNIT_TEST_SUITE_END();

  public:
    void simpleTleReadTest();
};



CPPUNIT_TEST_SUITE_REGISTRATION( TLEReaderTest );

int main() {
    CppUnit::TextUi::TestRunner runner;
    CppUnit::TestFactory &registry =
            CppUnit::TestFactoryRegistry::getRegistry();
    runner.addTest( registry.makeTest() );
    return !runner.run();
}

#endif //SPMIE_COMPLEXNUMBERTEST_H
