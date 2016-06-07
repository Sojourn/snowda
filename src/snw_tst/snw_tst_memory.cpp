#include "snw_tst.h"

using namespace Snowda;

void Snowda::tstMemory()
{
    TestModule module("Memory");
    tstRegion();
}

static void tstRegionAssign()
{
    TestCase test("RegionAssign");
    {
        Region region;
    }
    {
        Region region;
        region = Region();
    }
    {
        Region region(4096);
    }
    {
        Region region;
        region = Region(4096);
    }
}

static void tstRegionModify()
{
    TestCase test("RegionModify");
    {
        Region region(4096);
        region.modify(RegionProtection::None);
    }
    {
        Region region(4096);
        region.modify(RegionProtection::Read);
    }
    {
        Region region(4096);
        region.modify(RegionProtection::ReadWrite);
    }
    {
        Region region(4096);
        region.modify(RegionProtection::ReadExecute);
    }
    {
        Region region(8192);
        region.modify(0, 4096, RegionProtection::Read);
        region.modify(4096, 4096, RegionProtection::ReadWrite);
        memset(region.data() + 4096, 7, 4096);
    }
    {
        Region region(4096);
        region.modify(RegionProtection::ReadWrite);
        memset(region.data(), 7, region.size());
        region.modify(RegionProtection::None);
        region.modify(RegionProtection::Read);
        for (size_t i = 0; i < region.size(); ++i) {
            assert(region.data()[i] == 7);
        }
    }
}

void Snowda::tstRegion()
{
    tstRegionAssign();
    tstRegionModify();
}
