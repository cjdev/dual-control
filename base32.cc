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

#include <bitset>
#include <unordered_map>
#include <memory>

#include "base32.h"

static const std::vector<char> alphabet = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', '2', '3', '4', '5', '6', '7'
};

namespace
{
class base32_impl : public base32_ifc
{
private:
    uint64_t calculate_padding_bytes (uint64_t extra_bytes)
    {
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

        return padding_chars;
    }

    std::string &pad_string (uint64_t extra_bytes, std::string &input)
    {
        auto padding_chars = calculate_padding_bytes (extra_bytes);

        if (padding_chars > 0) {
            auto replace_end   = input.end();
            auto replace_start = replace_end - padding_chars;
            auto replace_count = replace_end - replace_start;

            input.replace (replace_start, replace_end, replace_count, '=');
        }

        return input;
    }

public:
    std::string encode (const std::vector<uint8_t> data) override
    {
        std::string result;

        uint8_t tmp = 0;

        auto data_size = data.size();
        auto extra_bytes = data_size % 5;

        auto begin = data.begin();
        auto end = data.end();
        auto leftover = (5 - extra_bytes) % 5;

        for (auto cur = begin; cur+5 < (end + leftover + 1); cur+=5) {
            std::vector<uint8_t> batch;

            if (cur+5 < end) {
                batch = std::vector<uint8_t> (cur, cur+5);
            } else {
                batch = std::vector<uint8_t> (cur, end);

                for (int x = 0; x < leftover; x++) {
                    batch.push_back (0);
                }
            }

            std::bitset<40> everything;

            for (int x = 0, y = 32; x < 5; x+=1, y=32-x*8) {
                uint64_t item = batch[x];
                everything |= item << y;
            }

            std::bitset<40> mask = 31;
            int offset = 35;
            mask <<= offset;

            for (/* see above */; offset >= 0; mask >>= 5, offset -= 5) {
                auto idx = ((everything & mask) >> offset).to_ullong();
                result.push_back (alphabet[idx]);
            }
        }

        result = pad_string (extra_bytes, result);
        return result;
    }

private:

    std::unordered_map <char, unsigned char> construct_lookup_table()
    {
        std::unordered_map<char, unsigned char> lookup_table;

        for (int i = 0; i < alphabet.size(); i += 1) {
            lookup_table[alphabet[i]] = i;
        }

        return lookup_table;
    }

    std::pair<uint8_t, uint8_t> split_value (uint8_t item, uint8_t pos,
            uint8_t width)
    {
        auto shift = width-pos;
        uint8_t part2_mask = (1 << (shift)) - 1;
        uint8_t part2 = item & part2_mask;
        item >>= shift;
        return { item, part2 };
    }

    void set_vector_at_bit (std::vector<uint8_t> &data, uint8_t item,
                            std::vector<uint8_t>::size_type byte_offset, uint8_t bit_offset_from_start,
                            uint8_t width)
    {
        bool need_to_split = bit_offset_from_start + width > 8;
        uint8_t bit_offset_from_end = 7 - bit_offset_from_start;

        if (need_to_split) {
            uint8_t split_pos = bit_offset_from_end + 1;
            auto pieces = split_value (item, split_pos, width);
            data[byte_offset] |= pieces.first;

            uint8_t leftover_bits = width - split_pos;
            uint8_t second_part_shift = 8 - leftover_bits;
            data[byte_offset+1] |= pieces.second << second_part_shift;
        } else {
            uint8_t last_bit = (bit_offset_from_end - width) + 1;
            data[byte_offset] |= item << last_bit;
        }
    }

    std::string::size_type calculate_decoded_size (std::string input)
    {
        std::string::size_type input_size = input.size();
        std::string::size_type first_equals = input.find_first_of ('=');

        if (first_equals != std::string::npos) {
            input_size = first_equals;
        }

        return (input_size * 5) / 8;
    }

public:

    std::vector<uint8_t> decode (std::string input) override
    {
        auto lookup_table = construct_lookup_table();
        auto input_size = calculate_decoded_size (input);

        std::vector<uint8_t> result (input_size, 0);

        unsigned long long bits_written = 0;

        for (std::string::size_type idx = 0; idx < input.size(); idx++) {
            uint8_t val = lookup_table[input[idx]];

            uint8_t start_bit = bits_written % 8;
            std::vector<unsigned char>::size_type current_byte = bits_written/8;

            set_vector_at_bit (result, val, current_byte, start_bit, 5);

            bits_written += 5;
        }

        return result;
    }
};
}

template class std::vector<unsigned char>;

base32::base32 ():
    delegate_ (std::make_shared<base32_impl> ())
{}
