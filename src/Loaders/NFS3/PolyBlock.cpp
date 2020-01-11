#include "PolyBlock.h"

PolyBlock::PolyBlock(std::ifstream &frd, uint32_t nTrackBlockPolys) : m_nTrackBlockPolys(nTrackBlockPolys), obj {}
{
    ASSERT(this->_SerializeIn(frd), "Failed to serialize PolyBlock from file stream");
}

bool PolyBlock::_SerializeIn(std::ifstream &frd)
{
    for (uint32_t polyBlockIdx = 0; polyBlockIdx < NUM_POLYGON_BLOCKS; polyBlockIdx++)
    {
        SAFE_READ(frd, &sz[polyBlockIdx], sizeof(uint32_t));
        if (sz[polyBlockIdx] != 0)
        {
            SAFE_READ(frd, &szdup[polyBlockIdx], sizeof(uint32_t));
            if (szdup[polyBlockIdx] != sz[polyBlockIdx])
            {
                return false;
            }
            poly[polyBlockIdx] = std::vector<PolygonData>(sz[polyBlockIdx]);
            SAFE_READ(frd, poly[polyBlockIdx].data(), sizeof(PolygonData) * sz[polyBlockIdx]);
        }
    }

    // Sanity check
    if (sz[4] != m_nTrackBlockPolys)
    {
        return false;
    }

    for(auto & o : obj)
    {
        SAFE_READ(frd, &o.n1, sizeof(uint32_t));
        if (o.n1 > 0)
        {
            SAFE_READ(frd, &o.n2, sizeof(uint32_t));

            o.types.resize(o.n2);
            o.numpoly.resize(o.n2);
            o.poly.resize(o.n2);

            uint32_t polygonCount = 0;
            o.nobj = 0;

            for (uint32_t k = 0; k < o.n2; ++k)
            {
                SAFE_READ(frd, &o.types[k], sizeof(uint32_t));

                if (o.types[k] == 1)
                {
                    SAFE_READ(frd, &o.numpoly[o.nobj], sizeof(uint32_t));

                    o.poly[o.nobj] = std::vector<PolygonData>(o.numpoly[o.nobj]);
                    SAFE_READ(frd, o.poly[o.nobj].data(), sizeof(PolygonData) * o.numpoly[o.nobj]);

                    polygonCount += o.numpoly[o.nobj];
                    ++o.nobj;
                }
            }
            // n1 == total nb polygons
            if (polygonCount != o.n1)
            {
                return false;
            }
        }
    }

    return true;
}

void PolyBlock::SerializeOut(std::ofstream &frd)
{
    for (uint32_t polyBlockIdx = 0; polyBlockIdx < NUM_POLYGON_BLOCKS; polyBlockIdx++)
    {
        frd.write((char*) &sz[polyBlockIdx], sizeof(uint32_t));
        if (sz[polyBlockIdx] != 0)
        {
            frd.write((char*) &szdup[polyBlockIdx], sizeof(uint32_t));
            frd.write((char*) poly[polyBlockIdx].data(), sizeof(PolygonData) * sz[polyBlockIdx]);
        }
    }

    for(auto & o : obj)
    {
        frd.write((char*) &o.n1, sizeof(uint32_t));
        if (o.n1 > 0)
        {
            frd.write((char*) &o.n2, sizeof(uint32_t));
            o.nobj = 0;
            for (uint32_t k = 0; k < o.n2; ++k)
            {
                frd.write((char*) &o.types[k], sizeof(uint32_t));
                if (o.types[k] == 1)
                {
                    frd.write((char*) &o.numpoly[o.nobj], sizeof(uint32_t));
                    frd.write((char*) o.poly[o.nobj].data(), sizeof(PolygonData) * o.numpoly[o.nobj]);
                    ++o.nobj;
                }
            }
        }
    }
}

