/**
 *
 */

#include <stdio.h>
#include <plist/plist.h>
#include <stdlib.h>
#include <libimobiledevice/libimobiledevice.h>
#include <libimobiledevice/lockdown.h>
#include <libimobiledevice/afc.h>
#include <libimobiledevice/house_arrest.h>

#define AFC_SERVICE_NAME "com.apple.afc"
#define HOUSE_ARREST_SERVICE_NAME "com.apple.mobile.house_arrest"

idevice_t phone = NULL;
lockdownd_client_t control = NULL;
house_arrest_client_t house_arrest = NULL;

const uint16_t CONNECT_PORT=3124;

int main(int argc, char **argv) {
    idevice_error_t err;
    lockdownd_service_descriptor_t service_descriptor = NULL;

    err = idevice_new(&phone, 0);
    if (IDEVICE_E_SUCCESS != err) {
        perror("Connection Failed: ");
        return err;
    }

    err = lockdownd_client_new_with_handshake(phone, &control, "appwill");
    if (err != LOCKDOWN_E_SUCCESS) {
        fprintf(stderr, "Connect error");
        if (err == LOCKDOWN_E_PASSWORD_PROTECTED) {
            fprintf(stderr, "Password protected");
        }
        return EXIT_FAILURE;
    }

    plist_t global = NULL;
    plist_t value = NULL;
    plist_t ptr = NULL;
    char *str = NULL;

    // run query and get format plist
    lockdownd_get_value(control, NULL, NULL, &global);
    // lockdownd_get_value(control, "com.apple.mobile.iTunes", NULL, &value);

    ptr = plist_dict_get_item(global, "SerialNumber");
    plist_get_string_val(ptr, &str);
    if (str != NULL) {
        ptr = plist_new_string(str);
        printf("%s\n", str);
        free(str);
    }

    ptr = plist_dict_get_item(global, "ProductVersion");
    plist_get_string_val(ptr, &str);
    if (str != NULL) {
        ptr = plist_new_string(str);
        printf("%s\n", str);
        free(str);
    }

    
    // err = lockdownd_start_service(control, HOUSE_ARREST_SERVICE_NAME, &service_descriptor);
    // if (! service_descriptor) {
    //     printf("empty service\n");
    // }
    // printf("port: %d\n", service_descriptor->port);

    // err = house_arrest_client_new(phone, service_descriptor, &house_arrest);
    // if (!house_arrest) {
    //     printf("err: %d, ", err); // }

    // err = house_arrest_send_command(house_arrest, "VendContainer", "chinaxiahaifeng@gmail.com");
    // if (err != HOUSE_ARREST_E_SUCCESS) {
    //     printf("send command failed, err: %d\n", err);
    //     return err;
    // }

    // plist_t dict = NULL;
    // err = house_arrest_get_result(house_arrest, &dict);
    // if(err != HOUSE_ARREST_E_SUCCESS) {
    //     printf("Get result Failed. err: %d\n", err);
    //     return err;
    // }
    // str = NULL;
    // plist_t node = plist_dict_get_item(dict, "Error");
    // if (node) {
    //     plist_get_string_val(node, &str);
    //     printf("Error: %s\n", str);
    //     if (str) free(str);
    // }
    // plist_free(dict);

    // idevice_connection_t con;
    // err = idevice_connect(phone, CONNECT_PORT, &con);
    // if(IDEVICE_E_SUCCESS != err) {
    //     perror("Connection Error: ");
    //     printf("success, - %d\n", err);
    //     return err;
    // }
    printf("SUCCESS\n");
    return 0;
}
