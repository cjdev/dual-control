/* Copyright (C) CJ Affiliate
 *
 * You may use, distribute and modify this code under  the
 * terms of the  GNU General Public License  version 2  or
 * later.
 *
 * You should have received a copy of the license with this
 * file. If not, you will find a copy in the "LICENSE" file
 * at https://github.com/cjdev/dual-control.
 */
#include <memory>
#include <algorithm>
#include <initializer_list>
#include <vector>
#include <climits>
#include <ctime>
#include <iostream>
#include <map>

#include "base32.h"
#include "test_util.h"

// totp test
int int_precomputed()
{
    // given
    std::map<std::string, std::string> precomputed = {
        {"consimilate", "mnxw443jnvuwyylumu======"},
        {"defacing", "mrswmyldnfxgo==="},
        {"downcome", "mrxxo3tdn5wwk==="},
        {"filchery", "mzuwyy3imvzhs==="},
        {"Galatic", "i5qwyylunfrq===="},
        {"hearthrug", "nbswc4tunbzhkzy="},
        {"heterotypic", "nbsxizlsn52hs4djmm======"},
        {"kinase", "nnuw4yltmu======"},
        {"Lycopodiales", "jr4wg33qn5sgsylmmvzq===="},
        {"mosker", "nvxxg23foi======"},
        {"ornithosaurian", "n5zg42lunbxxgylvojuwc3q="},
        {"quilkin", "of2ws3dlnfxa===="},
        {"swartly", "on3wc4tunr4q===="},
        {"teleost", "orswyzlpon2a===="},
        {"thinglet", "orugs3thnrsxi==="},
        {"unpregnant", "ovxha4tfm5xgc3tu"},
        {"unreachably", "ovxhezlbmnugcytmpe======"},
        {"unusableness", "ovxhk43bmjwgk3tfonzq===="},
        {"wickawee", "o5uwg23bo5swk==="},
        {"yareta", "pfqxezlume======"},
    };

    // The token for key 76I6WTYEUTNCJUREMGKVM45PMA and time '2017/01/01 00:00:00' is 258675
    base32 codec;


}

int run_tests()
{
    test (int_precomputed);
    succeed();
}

int main (int argc, char *argv[])
{
    return !run_tests();
}
