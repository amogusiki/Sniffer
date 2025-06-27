#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <gtest/gtest.h>

struct Address {
    std::string mac;
    int quantity;
};

class AddressList {
    std::vector<Address> data;

public:
    size_t GetCount() const {
        return data.size();
    }
    void AddAddress(const std::string& addr) {
        std::string newAddress = addr.substr(addr.find('=') + 1, 17);

        auto it = std::find_if(data.begin(), data.end(),
            [&](const Address& a) { return a.mac == newAddress; });

        if (it != data.end()) {
            it->quantity++;
        }
        else {
            data.push_back({ newAddress, 1 });
        }
    }

    void ProcessLine(const std::string& line) {
        size_t ra_pos = line.find("RA=");
        size_t ta_pos = line.find("TA=");
        size_t sa_pos = line.find("SA=");
        size_t bssid_pos = line.find("BSSID=");

        if (bssid_pos != std::string::npos) AddAddress(line.substr(bssid_pos));
        if (ra_pos != std::string::npos) AddAddress(line.substr(ra_pos));
        if (ta_pos != std::string::npos) AddAddress(line.substr(ta_pos));
        if (sa_pos != std::string::npos) AddAddress(line.substr(sa_pos));
    }

    void PrintAddresses() {
        std::cout << "Results:\nAddress    Quantity\n";

        std::sort(data.begin(), data.end(),
            [](const Address& a, const Address& b) {
                return a.quantity > b.quantity;
            });

        for (const auto& entry : data) {
            std::cout << entry.mac << " " << entry.quantity << "\n";
        }
        std::cout << "\n";
    }
    std::vector<int> GetQuantities() const {
        std::vector<int> quantities;
        for (const auto& entry : data) {
            quantities.push_back(entry.quantity);
        }
        std::sort(quantities.begin(), quantities.end(), std::greater<int>());
        return quantities;
    }
};

TEST(FramesParserAcceptanceTest, NumberOfAdressesTest) {
    const std::string filename = "C:\\Users\\drysf\\Desktop\\frames_parser.log";
    AddressList list;

    std::ifstream file(filename);
    if (file) {
        std::string line;

        while (std::getline(file, line)) {
            list.ProcessLine(line);
        }
    }
    int expected_quantity = 13;
    EXPECT_EQ(expected_quantity, list.GetCount());
}
TEST(FramesParserAcceptanceTest, NumberForEveryAddressTest) {
    const std::string filename = "C:\\Users\\drysf\\Desktop\\frames_parser.log";
    AddressList list;

    std::ifstream file(filename);
    if (file) {
        std::string line;

        while (std::getline(file, line)) {
            list.ProcessLine(line);
        }
    }
    const std::vector<int> expected_numbers = { 15235, 5812, 5307, 3713, 196, 124, 107, 98, 68, 14, 13, 4, 3 };
    std::vector<int> real_numbers;
    real_numbers = list.GetQuantities();
    for (size_t i = 0; i < expected_numbers.size(); ++i) {
        EXPECT_EQ(expected_numbers[i], real_numbers[i]) << i;
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
