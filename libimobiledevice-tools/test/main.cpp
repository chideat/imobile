#include <stdio.h>
#include "plist/plist.h"
#include <stdlib.h>

int main(int argc, char **argv) {
	char **path = NULL;
	unsigned int xml_len = 0;

	//char *path = "C:\\Users\\thinking\Desktop\1234567.plist";
	plist_t plist_root = plist_new_dict();
	plist_t plist_ringtone = plist_new_dict();
	plist_dict_insert_item(plist_root, "Ringtones", plist_ringtone);

	//node one
	plist_t plist_node1 = plist_new_dict();
	plist_t val = plist_new_string("¶ÏÇÅ²ÐÑ©");
	plist_dict_insert_item(plist_node1, "Album", val);

	plist_dict_insert_item(plist_ringtone, "DFYQ.m4r", plist_node1);
	plist_to_xml(plist_root, path, &xml_len); 
	plist_free(plist_root);
	printf("%s\n", *path);
	free(path);
	return 0;
}