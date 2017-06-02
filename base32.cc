#include <iostream>
#include <iomanip>
#include <vector>

std::string encodeBase32(const std::vector<uint8_t> data) {
    std::vector<char> alphabet = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
        'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
        'Y', 'Z', '2', '3', '4', '5', '6', '7'
    };

    uint8_t tmp = 0;
    auto data_size = data.size();

    std::string result;
    auto begin = data.begin();
    auto end = data.end();

    auto extra_bytes = (data_size % 5);
    auto padding_chars = 0;
    switch (extra_bytes) {
    case 1:
        padding_chars = 6;
        break;
    case 2:
        padding_chars = 4;
        break;
    case 3:
        padding_chars = 3;
        break;
    case 4:
        padding_chars = 1;
        break;
    }

    auto leftover = (5 - extra_bytes) % 5;
    for (auto cur = begin; cur+5 < (end + leftover + 1); cur+=5) {
        std::vector<uint8_t> batch;
        if (cur+5 < end) {
             batch = std::vector<uint8_t> (cur, cur+5);
        } else {
             batch = std::vector<uint8_t> (cur, end);
             for (int x = 0; x < leftover; x++) {
                 batch.push_back(0);
             }
        }

        std::bitset<40> everything;
        everything |= (static_cast<uint64_t>(batch[0]) << 32);
        everything |= (static_cast<uint64_t>(batch[1]) << 24);
        everything |= (static_cast<uint64_t>(batch[2]) << 16);
        everything |= (static_cast<uint64_t>(batch[3]) <<  8);
        everything |=                        batch[4];

        uint64_t mask = 31;
        uint64_t offset = 35;
        mask <<= offset;

        int counter = 0;
        while (offset <= 35) {
            auto idx = everything;
            idx &=  mask;
            idx >>= offset;

            mask >>= 5;
            offset -= 5;

            result.push_back(alphabet[idx.to_ullong()]);
        }
    }

    if (padding_chars > 0) {
        auto replace_end   = result.end();
        auto replace_start = replace_end - padding_chars;
        auto replace_count = replace_end - replace_start;

        result.replace(replace_start, replace_end, replace_count, '=');
    }

    return result;
}

int main(int argc, char** argv) {
    std::string data;
    while (std::getline(std::cin,data)) {
        std::vector<unsigned char> v(data.begin(), data.end());

        std::string res = encodeBase32(v);
        std::cout << "{\"ORIGINAL\": \"" << data << "\", \"BASE32\": \"" << res << "\"}" << std::endl;
    }
}
