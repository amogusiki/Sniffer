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
    printf("Results:\nAddress           Quantity\n");
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
int main() {
    char filename[100];
    char line[1500];
    printf("Enter file location: ");
entername:
    scanf("%s", filename);
    address_list list;
    list.data = NULL;
    list.size = 0;

    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("File does not exist. Try again: ");
        goto entername;
    }
    else {
        printf("File found. Started processing...\n");
    }
    if (fp) {
        while ((fgets(line, 1000, fp)) != NULL) {
            ProcessLine(line, &list);
        }
    }
    printf("Finished Processing\n\n");
    PrintAdresses(&list);
    return 0;
}
