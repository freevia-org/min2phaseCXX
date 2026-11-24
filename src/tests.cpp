/**
 * min2phaseCXX Copyright (C) 2022 Borgo Federico
 * This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
 * This is free software, and you are welcome to redistribute it
 * under certain conditions; type `show c' for details.
 */

#include <cassert>
#include <min2phase/tools.h>
#include <iostream>
#include <min2phase/min2phase.h>
#include "CubieCube.h"
#include "info.h"
#include "tests.h"
#include "Search.h"

namespace min2phase{ namespace tests{
    const uint8_t N_CUBE_TESTS = 255;

    //verify error input
    void testInput(){
        //YYWOYYGBO GGOOBRWBG RRYOOWWRR OWBOWWBYR RGYWGYYBB GRBGRGWBO
        assert(tools::verify("YYWOYYGBOGGOOBRWBGRRYOOWWRROWBOWWBYRRGYWGYYBBGRBGRGWBO") == info::NO_ERROR);

        //YYWOYYGBO GGOOBRWBG RRYOOWWRR OWBOWWBYR RGYWGYYBB GRBGRGWB
        assert(tools::verify("YYWOYYGBOGGOOBRWBGRRYOOWWRROWBOWWBYRRGYWGYYBBGRBGRGWB") == info::MALFORMED_STRING);

        //YYWOYGGBO GYOOBRWBG RRYOOWWRR OWBOWWBYR RGYWGYYBB GRBGRGWBO
        assert(tools::verify("YYWOYGGBOGYOOBRWBGRRYOOWWRROWBOWWBYRRGYWGYYBBGRBGRGWBO") == info::FLIPPED_EDGE);

        //YYWOYYGBG YGOOBRWBG RROOOWWRR OWBOWWBYR RGYWGYYBB GRBGRGWBO
        assert(tools::verify("YYWOYYGBGYGOOBRWBGRROOOWWRROWBOWWBYRRGYWGYYBBGRBGRGWBO") == info::TWISTED_CORNER);

        for(uint8_t i = 0; i < N_CUBE_TESTS; i++)
            assert(tools::verify(tools::randomCube()) == info::NO_ERROR);
    }

    // Public cubie arrays can be mutated directly. Invalid encoded values
    // must be rejected before check() uses them in shift expressions.
    void testMalformedCubieValues(){
        CubieCube cube;

        cube.edges[0] = -1;
        assert(cube.check() == info::MISSING_EDGE);
        assert(cube.getEPerm() == UINT16_MAX);
        assert(cube.getEPermSym() == UINT16_MAX);

        cube = CubieCube();
        cube.edges[0] = info::NUMBER_EDGES * 2;
        assert(cube.check() == info::MISSING_EDGE);

        cube = CubieCube();
        cube.corners[0] = -1;
        assert(cube.check() == info::MISSING_CORNER);
        assert(cube.getCPerm() == UINT16_MAX);
        assert(cube.getCPermSym() == UINT16_MAX);

        cube = CubieCube();
        for (uint8_t i = 0; i < info::NUMBER_CORNER; ++i)
            cube.corners[i] = 0;
        assert(cube.getCPerm() == UINT16_MAX);
        assert(cube.getCPermSym() == UINT16_MAX);

        cube = CubieCube();
        for (uint8_t i = 0; i < info::NUMBER_EDGES; ++i)
            cube.edges[i] = 23;
        assert(cube.getEPerm() == UINT16_MAX);
        assert(cube.getEPermSym() == UINT16_MAX);
        assert(cube.getMPerm() == -1);

        cube = CubieCube();
        cube.corners[0] = info::NUMBER_CORNER * info::CORNE_ORI_CASES;
        assert(cube.check() == info::MISSING_CORNER);

        cube = CubieCube();
        assert(cube.check() == info::NO_ERROR);
        assert(cube.getCPerm() == 0);
        assert(cube.getEPerm() == 0);
    }

    //test search errors
    void testSearchError(){
        Search s;

        //YYWOYYGBO GGOOBRWBG RRYOOWWRR OWBOWWBYR RGYWGYYBB GRBGRGWBO
        assert(s.solve("YYWOYYGBOGGOOBRWBGRRYOOWWRROWBOWWBYRRGYWGYYBBGRBGRGWBO", 31, 0, 0, 0, nullptr) == std::to_string(info::PROBE_LIMIT));

        s = Search();
        //YYWOYYGBO GGOOBRWBG RRYOOWWRR OWBOWWBYR RGYWGYYBB GRBGRGWBO
        assert(s.solve("YYWOYYGBOGGOOBRWBGRRYOOWWRROWBOWWBYRRGYWGYYBBGRBGRGWBO", 1, 100000, 0, 0, nullptr) == std::to_string(info::SHORT_DEPTH));

        coords::coords.isInitialized = false;

        s = Search();
        //YYWOYYGBO GGOOBRWBG RRYOOWWRR OWBOWWBYR RGYWGYYBB GRBGRGWBO
        assert(s.solve("YYWOYYGBOGGOOBRWBGRRYOOWWRROWBOWWBYRRGYWGYYBBGRBGRGWBO", 31, 100000, 0, 0, nullptr) == std::to_string(info::MISSING_COORDS));

        coords::coords.isInitialized = true;
    }

    //test the solver
    void testSearch(){
        Search s;
        std::string cube;

        for(uint8_t i = 0; i < N_CUBE_TESTS; i++){
            cube = tools::randomCube();
            s = Search();

            assert(tools::fromScramble(s.solve(cube, 31, 100000, 0, min2phase::INVERSE_SOLUTION, nullptr)) == cube);
        }
    }

}   }
