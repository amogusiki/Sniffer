#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gtest/gtest.h>

#pragma warning(disable:4996)

struct address {
    char mac[18];
    int quantity;
};

struct address_list {
    address* data;
    size_t size;
};

void AddAddress(address_list* list, const char* addr) {
    char newaddress[18] = { 0 };
    strncpy(newaddress, addr + 3, 17);
    newaddress[17] = '\0';
    int found = 0;
    for (int i = 0;i < list->size;i++) {
        if (strcmp(newaddress, list->data[i].mac) == 0) {
            list->data[i].quantity++;
            found = 1;
            break;
        }
    }
    if (found == 0) {
        list->data = (address*)realloc(list->data, sizeof(address) * (list->size + 1));
        strcpy(list->data[list->size].mac, newaddress);
        list->data[list->size].quantity = 1;
        list->size++;
    }
}

void ProcessLine(char* line, address_list* list) {

    char* RA_pos = strstr(line, "RA=");
    char* TA_pos = strstr(line, "TA=");
    char* SA_pos = strstr(line, "SA=");
    char* BSSID_pos = strstr(line, "BSSID=");
    if (BSSID_pos != NULL) {
        if ((BSSID_pos - 1)[0] != '/') {
            BSSID_pos = NULL;
        }
    }
    if (BSSID_pos != NULL) {
        AddAddress(list, BSSID_pos + 3);
    }
    if (RA_pos != NULL) {
        AddAddress(list, RA_pos);
    }
    if (TA_pos != NULL) {
        AddAddress(list, TA_pos);
    }
    if (SA_pos != NULL) {
        AddAddress(list, SA_pos);
    }
}

void PrintAdresses(address_list* list) {
    printf("Results:\nAddress    Quantity\n");
    int maxquantity = 0;
    for (int i = 0;i < list->size;i++) {
        if (list->data[i].quantity > maxquantity) {
            maxquantity = list->data[i].quantity;
        }
    }
    for (int i = maxquantity;i > 0;i--) {
        for (int j = 0;j < list->size;j++) {
            if (list->data[j].quantity == i) {
                printf("%s %d\n", list->data[j].mac, list->data[j].quantity);
            }
        }
    }
    printf("\n");
}

void ProcessFile(const char* filename, address_list* list) {
    char line[1500];
    FILE* fp = fopen(filename, "r");
    if (fp) {
        while ((fgets(line, sizeof(line), fp)) != NULL) {
            ProcessLine(line, list);
        }
        fclose(fp);
    }
}

TEST(FramesParserAcceptanceTest, RealFileTest) {
    const char* filename = "C:\\Users\\drysf\\Desktop\\frames_parser.log";
    address_list list = { NULL, 0 };

    testing::internal::CaptureStdout();

    printf("Enter file location: %s\n", filename);
    ProcessFile(filename, &list);
    printf("File found. Started processing...\n");
    printf("Finished Processing\n\n");
    PrintAdresses(&list);

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
        "84:c5:a6:07:38:66 124\n"
        "6e:52:4e:5f:f9:eb 107\n"
        "ff:ff:ff:ff:ff:ff 98\n"
        "4a:5f:99:ae:ea:99 98\n"
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