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

int precomputed_values()
{
    // given
    std::map<std::string, std::string> precomputed = {
        {"consimilate", "MNXW443JNVUWYYLUMU======"},
        {"defacing", "MRSWMYLDNFXGO==="},
        {"downcome", "MRXXO3TDN5WWK==="},
        {"filchery", "MZUWYY3IMVZHS==="},
        {"Galatic", "I5QWYYLUNFRQ===="},
        {"hearthrug", "NBSWC4TUNBZHKZY="},
        {"heterotypic", "NBSXIZLSN52HS4DJMM======"},
        {"kinase", "NNUW4YLTMU======"},
        {"Lycopodiales", "JR4WG33QN5SGSYLMMVZQ===="},
        {"mosker", "NVXXG23FOI======"},
        {"ornithosaurian", "N5ZG42LUNBXXGYLVOJUWC3Q="},
        {"quilkin", "OF2WS3DLNFXA===="},
        {"swartly", "ON3WC4TUNR4Q===="},
        {"teleost", "ORSWYZLPON2A===="},
        {"thinglet", "ORUGS3THNRSXI==="},
        {"unpregnant", "OVXHA4TFM5XGC3TU"},
        {"unreachably", "OVXHEZLBMNUGCYTMPE======"},
        {"unusableness", "OVXHK43BMJWGK3TFONZQ===="},
        {"wickawee", "O5UWG23BO5SWK==="},
        {"yareta", "PFQXEZLUME======"},
    };

    // The token for key 76I6WTYEUTNCJUREMGKVM45PMA and time '2017/01/01 00:00:00' is 258675
    base32 codec = base32();

    for (const auto &entry: precomputed) {
        std::string encoded = codec.encode ( std::vector<uint8_t>
                                             (entry.first.begin(), entry.first.end()) );
        check (encoded == entry.second, "precomputed value didn't match");
    }

    succeed();
}

int roundtrip()
{
    // given
    std::vector<std::vector<uint8_t>> values = {
        { 0 }, { 1, 1 }, { 2, 2, 2 }, { 3, 3, 3, 3 }, { 4, 4, 4, 4, 4 },
        { 5, 5, 5, 5, 5, 5, }, { 6, 6, 6, 6, 6, 6, 6, }
    };

    // The token for key 76I6WTYEUTNCJUREMGKVM45PMA and time '2017/01/01 00:00:00' is 258675
    base32 codec = base32();

    for (auto expected: values) {
        std::string encoded = codec.encode (expected);
        std::vector<std::uint8_t> actual = codec.decode (encoded);

        check (expected == actual, "precomputed value didn't match");
    }

    succeed();
}

int run_tests()
{
    test (precomputed_values);
    test (roundtrip);
    succeed();
}

int main (int argc, char *argv[])
{
    return !run_tests();
}

