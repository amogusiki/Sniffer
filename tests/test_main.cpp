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
};

TEST(FramesParserAcceptanceTest, RealFileTest) {
    const std::string filename = "C:\\Users\\drysf\\Desktop\\frames_parser.log";
    AddressList list;

    testing::internal::CaptureStdout();

    std::cout << "Enter file location: " << filename << "\n";

    std::ifstream file(filename);
    if (file) {
        std::cout << "File found. Started processing...\n";
        std::string line;

        while (std::getline(file, line)) {
            list.ProcessLine(line);
        }

        std::cout << "Finished Processing\n\n";
        list.PrintAddresses();
    }

    std::string output = testing::internal::GetCapturedStdout();

    std::string expected =
        "Enter file location: C:\\Users\\drysf\\Desktop\\frames_parser.log\n"
        "File found. Started processing...\n"
        "Finished Processing\n\n"
        "Results:\n"
        "Address    Quantity\n"
        "b8:69:f4:7a:a5:ac 15235\n"
        "34:1c:f0:d3:40:a2 5812\n"
        "34:1c:f0:d2:78:5a 5307\n"
        "00:0c:29:65:08:ee 3713\n"
        "4a:5f:99:ae:ea:99 196\n"
        "84:c5:a6:07:38:66 124\n"
        "6e:52:4e:5f:f9:eb 107\n"
        "ff:ff:ff:ff:ff:ff 98\n"
        "b8:69:f4:7a:a5:93 68\n"
        "52:ff:20:52:16:9a 14\n"
        "70:c9:32:1b:54:e2 13\n"
        "80:b6:55:60:6f:58 4\n"
        "c8:7f:54:28:74:ac 3\n\n";

    ASSERT_EQ(output, expected);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
