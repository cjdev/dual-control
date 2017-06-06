#include <iostream>
#include <iomanip>
#include <vector>
#include <unordered_map>

uint64_t calculate_padding_bytes(uint64_t extra_bytes) {
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

std::string &pad_string(uint64_t extra_bytes, std::string &input) {
    auto padding_chars = calculate_padding_bytes(extra_bytes);
    if (padding_chars > 0) {
        auto replace_end   = input.end();
        auto replace_start = replace_end - padding_chars;
        auto replace_count = replace_end - replace_start;

        input.replace(replace_start, replace_end, replace_count, '=');
    }
    return input;
}

static const std::vector<char> alphabet = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', '2', '3', '4', '5', '6', '7'
};

std::string encodeBase32(const std::vector<uint8_t> data) {
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
                 batch.push_back(0);
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
            result.push_back(alphabet[idx]);
        }
    }

    result = pad_string(extra_bytes, result);
    return result;
}

std::unordered_map <char, unsigned char> construct_lookup_table() {
    std::unordered_map<char, unsigned char> lookup_table;
    for (int i = 0; i < alphabet.size(); i += 1) {
        lookup_table[alphabet[i]] = i;
    }
    return lookup_table;
}

void print_bits(uint8_t input, int width = 8) {
    for (int counter = 0; counter < width; counter++) {
        unsigned int bit = input & (1 << (width - 1));
        input <<= 1;
        bit >>= (width - 1);

        printf("%d", bit);
        if (counter == (width-1)/2) {
            printf("|");
        }
    }
}

uint8_t extract_bits_from_string(std::string input, std::string::size_type byte_offset, uint8_t bit_offset, uint8_t window_size = 5) {
    auto relevant_characters = input.substr(byte_offset, byte_offset + 2);
    uint16_t bit_end_position = bit_offset + window_size;
    uint16_t result = relevant_characters[0] << 8;
    uint16_t mask = (1 << window_size) - 1;
    uint16_t shift_offset = 16 - bit_end_position;
    if (bit_end_position > 8) {
        result |= relevant_characters[1];
    }

    return (result & (mask << shift_offset)) >> shift_offset;
}

std::pair<uint8_t, uint8_t> split_value(uint8_t item, uint8_t pos, uint8_t width) {
    auto shift = width-pos;
    uint8_t part2_mask = (1 << (shift)) - 1;
    uint8_t part2 = item & part2_mask;
    item >>= shift;
    return { item, part2 };
}

void set_vector_at_bit(std::vector<uint8_t> &data, uint8_t item, std::vector<uint8_t>::size_type byte_offset, uint8_t bit_offset_from_start, uint8_t width) {
    bool need_to_split = bit_offset_from_start + width > 8;
    uint8_t bit_offset_from_end = 7 - bit_offset_from_start;

    if (need_to_split) {
        uint8_t split_pos = bit_offset_from_end + 1;
        auto pieces = split_value(item, split_pos, width);
        data[byte_offset] |= pieces.first;

        uint8_t leftover_bits = width - split_pos;
        uint8_t second_part_shift = 8 - leftover_bits;
        data[byte_offset+1] |= pieces.second << second_part_shift;
    } else {
        uint8_t last_bit = (bit_offset_from_end - width) + 1;
        data[byte_offset] |= item << last_bit;
    }
}


std::string::size_type calculate_decoded_size(std::string input) {
    std::string::size_type input_size = input.size();
    std::string::size_type first_equals = input.find_first_of('=');
    if (first_equals != std::string::npos) {
        input_size = first_equals;
    }
    return (input_size * 5) / 8;
}

std::vector<unsigned char> decodeBase32(std::string input) {
    auto lookup_table = construct_lookup_table();
    auto input_size = calculate_decoded_size(input);

    std::vector<unsigned char> result(input_size, 0);

    unsigned long long bits_written = 0;
    for (std::string::size_type idx = 0; idx < input.size(); idx++) {
        uint8_t val = lookup_table[input[idx]];
        uint8_t start_bit = bits_written % 8;
        std::vector<unsigned char>::size_type current_byte = bits_written/8;

        set_vector_at_bit(result, val, current_byte, start_bit, 5);

        bits_written += 5;
    }
    return result;
}

int main(int argc, char** argv) {
    std::string data = "hi";
    while (std::getline(std::cin,data)) {
        std::cout << std::endl;
        std::vector<unsigned char> input(data.begin(), data.end());
        // std::vector<unsigned char> input = { 0b10001100, 0b01100011, 0b00011000, 0b11000110, 0b10101010, 0b10101010 };

        std::cout << "data:\t";
        for (auto v: input) {print_bits(v); std::cout << ":";}
        std::cout << std::endl;

        std::string encoded = encodeBase32(input);
        std::vector<unsigned char> decoded = decodeBase32(encoded);

        std::cout << "output:\t";
        for (auto v: decoded) {print_bits(v); std::cout << ":";}
        std::cout << std::endl;
    }

}

// To test the encoder at the command line:
/*
int main(int argc, char** argv) {
    std::string data;
    while (std::getline(std::cin,data)) {
        std::vector<unsigned char> v(data.begin(), data.end());

        std::string res = encodeBase32(v);
        std::cout << "{\"ORIGINAL\": \"" << data << "\", \"BASE32\": \"" << res << "\"}" << std::endl;
    }
}

export tmp=`mktemp`
(while true; do
  (paste <(./a.out < /usr/share/dict/words | tee /tmp/my_implementation | jq -r .BASE32) <(tr '[a-z]' '[A-Z]' < /tmp/oracle) /usr/share/dict/words | awk '$1 == $2 { printf("^[[32m") } $1 != $2 {printf("^[[31m") } {printf("%s\t%s\ttest: %s\toracle: %s", $1==$2, $3, $1, $2); print "^[[0m"} ' | sample.awk -v n=65 | sort > $tmp ) ;
  sleep 5;
  clear;
  printf '\e]50;ClearScrollback\a';
  column -s $'\t' -t $tmp;
done)
*/

// int main(int argc, char** argv) {
//     std::vector<unsigned char> input;
//     input = { 0, 0 ,0 ,0 ,0 ,0 ,0};
//     std::cout << "orig vector: "; for (auto v: input) {print_bits(v);}; std::cout << std::endl;
//     set_vector_at_bit(input, 0b11111, 0, 7, 5);
//     std::cout << "rslt vector: "; for (auto v: input) {print_bits(v);}; std::cout << std::endl;

//     input = { 0, 0 ,0 ,0 ,0 ,0 ,0};
//     std::cout << "orig vector: "; for (auto v: input) {print_bits(v);}; std::cout << std::endl;
//     set_vector_at_bit(input, 0b11111, 0, 6, 5);
//     std::cout << "rslt vector: "; for (auto v: input) {print_bits(v);}; std::cout << std::endl;

//     input = { 0, 0 ,0 ,0 ,0 ,0 ,0};
//     std::cout << "orig vector: "; for (auto v: input) {print_bits(v);}; std::cout << std::endl;
//     set_vector_at_bit(input, 0b11111, 0, 5, 5);
//     std::cout << "rslt vector: "; for (auto v: input) {print_bits(v);}; std::cout << std::endl;

//     input = { 0, 0 ,0 ,0 ,0 ,0 ,0};
//     std::cout << "orig vector: "; for (auto v: input) {print_bits(v);}; std::cout << std::endl;
//     set_vector_at_bit(input, 0b11111, 0, 4, 5);
//     std::cout << "rslt vector: "; for (auto v: input) {print_bits(v);}; std::cout << std::endl;

//     input = { 0, 0 ,0 ,0 ,0 ,0 ,0};
//     std::cout << "orig vector: "; for (auto v: input) {print_bits(v);}; std::cout << std::endl;
//     set_vector_at_bit(input, 0b11111, 0, 3, 5);
//     std::cout << "rslt vector: "; for (auto v: input) {print_bits(v);}; std::cout << std::endl;
// }

template class std::vector<unsigned char>;
