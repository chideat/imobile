/**
 *
 */

#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <libimobiledevice/libimobiledevice.h>
#include <libimobiledevice/lockdown.h>
#include <libimobiledevice/afc.h>
#include <objbase.h>

#define AFC_SERVICE_NAME "com.apple.afc"
#define HOUSE_ARREST_SERVICE_NAME "com.apple.mobile.house_arrest"
#define RINGTONES_PATH "/iTunes_Control/Ringtones"
#define RINGTONES_PATH_PLIST "/iTunes_Control/iTunes/Ringtones.plist"

idevice_t device = NULL;
lockdownd_client_t control = NULL;

const uint16_t CONNECT_PORT=3124;


typedef struct {
	char path[256];
	char key[56];
	plist_t name;
	plist_t artist;
	plist_t album;
	plist_t total_time;
	plist_t pid;
	plist_t guid;
	plist_t genre;
	plist_t protected_content;
} Ringtone;

int ringtone_upload(afc_client_t afc, const Ringtone *item) {
	if (item == NULL) 
		return 1;
	uint64_t handle;
	char path[128];
	sprintf(path, "%s/%s", RINGTONES_PATH, item->key);
    // open config file
	if (AFC_E_SUCCESS != afc_file_open(afc, path, AFC_FOPEN_WRONLY, &handle)) {
        printf("open iphone file failed£¬ %s\n", path);
        return EXIT_FAILURE;
    }

	char data[81920];
    uint32_t bytes_written, data_len;
	int file = open(item->path, O_RDONLY | O_BINARY);
	if (file <= 0) {
		fprintf(stderr, "file open failed, %s\n", item->path);
		return EXIT_FAILURE;
	}
	do {
		data_len = read(file, data, sizeof(data));
		if (data_len <= 0)
			break;
	    printf("len of music: %d\n", data_len);
		if (AFC_E_SUCCESS != afc_file_write(afc, handle, data, data_len, &bytes_written)) {
			afc_file_close(afc, handle);
			printf("write failed\n");
			return EXIT_FAILURE;
		}
	    printf("write size: %d\n", bytes_written);
	} while(1);
	// end
	close(file);
	afc_file_close(afc, handle);
}

int ringtone_plist_read(afc_client_t afc, plist_t *plist) {
	char data[8192];
	uint32_t bytes_read;
    uint64_t handle;
	// open config file
	if (AFC_E_SUCCESS != afc_file_open(afc, RINGTONES_PATH_PLIST, AFC_FOPEN_RW, &handle)) {
        printf("open file failed, read\n");
        return -1;
    }
	// read
	if (AFC_E_SUCCESS != afc_file_read(afc, handle, data, sizeof(data), &bytes_read)) {
		printf("read %s failed\n", RINGTONES_PATH_PLIST);
		exit(EXIT_FAILURE);
		return -1;
	}

	// to plist
	plist_from_bin((char *)data, bytes_read, plist);
	//for(int i = 0; i < bytes_read; i ++ ) {
	//	printf("%c", data[i]);
	//}
	//exit(EXIT_FAILURE);
	if(bytes_read == 0) {
		*plist = plist_new_dict();
		plist_t plist_ringtone = plist_new_dict();
		plist_dict_insert_item(*plist, "Ringtones", plist_ringtone);
	}
	afc_file_close(afc, handle);
	return 0;
}


int ringtone_plist_write(afc_client_t afc, plist_t plist) {
	uint64_t handle;
    // open config file
	if (AFC_E_SUCCESS != afc_file_open(afc, RINGTONES_PATH_PLIST, AFC_FOPEN_WRONLY, &handle)) {
        printf("open file failed write\n");
        return EXIT_FAILURE;
    }
	char *data = nullptr;
	uint32_t data_len = 0, bytes_written = 0, point = 0;

	plist_to_bin(plist, (char **)&data, &data_len);
	while(data_len > 0) {
		if (AFC_E_SUCCESS != afc_file_write(afc, handle, data + point, data_len, &bytes_written)) {
			printf("write file failed\n");
			return EXIT_FAILURE;
		}
		data_len = data_len - bytes_written;
		point += bytes_written;
	}
	// write ok, then close it
	afc_file_close(afc, handle);
	// free data
	free(data);
	return 0;
}


void ringtone_plist_free(plist_t *plist) {
	plist_free(plist);
}

int ringtone_plist_add_item(plist_t r_plist, const Ringtone *item) {
	// compare first

	plist_t node = plist_new_dict();
	plist_dict_insert_item(node, "Name", item->name);
	plist_dict_insert_item(node, "GUID", item->guid);
	plist_dict_insert_item(node, "Album", item->album);
	plist_dict_insert_item(node, "Artist", item->artist);
	plist_dict_insert_item(node, "Genre", item->genre);
	plist_dict_insert_item(node, "PID", item->pid);
	plist_dict_insert_item(node, "Protected Content", item->protected_content);
	plist_dict_insert_item(node, "Total Time", item->total_time);
	// add node to root
	plist_dict_insert_item(r_plist, item->key, node);
	return 0;
}

int ringtone_plist_del_item(plist_t *r_plist, const char *key){
	plist_dict_remove_item(r_plist, key);
	return 0;
}

int main(int argc, char **argv) {
    idevice_error_t err;
    lockdownd_service_descriptor_t service_descriptor = NULL;

    err = idevice_new(&device, 0);
    if (IDEVICE_E_SUCCESS != err) {
		printf("Error Code: %d\n", err);
        return err;
    }
	
	err = lockdownd_client_new_with_handshake(device, &control, "appwill");
    if (err != LOCKDOWN_E_SUCCESS) {
        fprintf(stderr, "Connect error");
        if (err == LOCKDOWN_E_PASSWORD_PROTECTED) {
            fprintf(stderr, "Password protected");
        }
		printf("EXIT----------------------\n");
        return EXIT_FAILURE;
    }

    err = lockdownd_start_service(control, "com.apple.afc", &service_descriptor);
    if (!service_descriptor) {
        printf("empty service\n");
		return EXIT_FAILURE;
    }
    printf("port: %d\n", service_descriptor->port);


	afc_client_t afc = NULL;
    plist_t plist, r_plist;
	if (AFC_E_SUCCESS != afc_client_new(device, service_descriptor, &afc)) {
        printf("create afc client failed!");
        return EXIT_FAILURE;
    }

    GUID guid;
	HRESULT hr = CoCreateGuid(&guid);
	char gguid[17];
	sprintf(gguid, "%08X%04X%04x", guid.Data1, guid.Data2, guid.Data3);
	gguid[16] = 0;

	Ringtone  ringtone;
	ringtone.name = plist_new_string("Because");
	ringtone.artist = plist_new_string("chideat");
	ringtone.album = plist_new_string("123");
	ringtone.total_time = plist_new_uint(33000);
	ringtone.protected_content = plist_new_bool(0);
	ringtone.guid = plist_new_string(gguid);
	ringtone.pid = plist_new_uint(123456789);
	ringtone.genre = plist_new_string("POP");
	strcpy(ringtone.key, "DECF.m4r");
	strcpy(ringtone.path, "C:\\Users\\thinking\\Desktop\\123.m4r");

	// create dir
	afc_error_t afc_ret;
	afc_ret = afc_make_directory(afc, RINGTONES_PATH);
    if ((AFC_E_SUCCESS != afc_ret) && (AFC_E_OBJECT_EXISTS != afc_ret)) {
        printf("mkdir error: %d\n", afc_ret);
        return EXIT_FAILURE;
    }

	// first upload the m4r ringtone
	ringtone_upload(afc, &ringtone);

	// second update the ringtones.plist
	// read
	ringtone_plist_read(afc, &plist);

	// get ringtones node
	r_plist = plist_dict_get_item(plist, "Ringtones");
	ringtone_plist_add_item(r_plist, &ringtone);
	// write back to list 
	ringtone_plist_write(afc, plist);

	// release 
	plist_free(plist);
	afc_client_free(afc);
	lockdownd_client_free(control);
	idevice_free(device);

    printf("SUCCESS\n");
    return 0;
}