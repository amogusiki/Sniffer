#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

struct Address {
    std::string mac;
    int quantity;
};

class AddressList {
    std::vector<Address> data;

public:
    void AddAddress(const std::string& addr) {
        std::string newAddress = addr.substr(3, 17);

        auto it = std::find_if(data.begin(), data.end(), [&](const Address& a) { return a.mac == newAddress; });

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

        if (bssid_pos != std::string::npos && line[bssid_pos - 1] != '/') {
            bssid_pos = std::string::npos;
        }

        if (bssid_pos != std::string::npos) {
            AddAddress(line.substr(bssid_pos+3));
        }
        if (ra_pos != std::string::npos) {
            AddAddress(line.substr(ra_pos));
        }
        if (ta_pos != std::string::npos) {
            AddAddress(line.substr(ta_pos));
        }
        if (sa_pos != std::string::npos) {
            AddAddress(line.substr(sa_pos));
        }
    }

    void PrintAddresses() {
        std::cout << "Results:\nAddress    Quantity\n";

        std::sort(data.begin(), data.end(), [](const Address& a, const Address& b) {
                return a.quantity > b.quantity;
        });

        for (const auto& entry : data) {
            std::cout << entry.mac << " " << entry.quantity << "\n";
        }
        std::cout << "\n";
    }
};

int main() {
    AddressList list;
    std::string filename;

    while (true) {
        std::cout << "Enter file location: ";
        std::getline(std::cin, filename);

        std::ifstream file(filename);
        if (file) {
            std::cout << "File found. Started processing...\n";
            std::string line;

            while (std::getline(file, line)) {
                list.ProcessLine(line);
            }

            std::cout << "Finished Processing\n\n";
            list.PrintAddresses();
            break;
        }

        std::cout << "File does not exist. Try again.\n";
    }

    return 0;
}
