#include "LangFile.h"

LangFile::LangFile() {
}

LangFile::LangFile(const char* FileName) {
	OpenFile(FileName);
}

LangFile::~LangFile() {
}

void LangFile::OpenFile(const char* FileName) {
	FILE* file = fopen(FileName,"rb");
	if(file) {
		char linebuffer[512];
		char textbuffer[512];
		string formatspec = "csdioxXufFeEaAgGnp";
		int ID;
		while(fgets(linebuffer,512,file)) {
			if(sscanf(linebuffer,"%d=%[^\r\n]",&ID,textbuffer) == 2) {
				LangItem item;
				item.ID = ID;
				item.text = textbuffer;
				item.formatspec = "";

				// Find formatting specifiers
				// Loop through format options
				int pos = item.text.find('%',0);
				while(pos != string::npos) {
					item.formatspec += '%';
					int offset = pos+1;
					while(offset < item.text.length()) {
						item.formatspec += item.text[offset];
						if(formatspec.find(item.text[offset]) != string::npos) {
							break;
						}


						offset++;
					}
					item.formatspec += ",";
					pos = item.text.find('%',pos+1);
				}

				// Add to list
				contents.push_back(item);
			}
		}

		Log("INFO: Found %d items in file %s\r\n",contents.size(),FileName);

		fclose(file);
	}
}

LangItem* LangFile::FindID(int ID) {
	for(unsigned int i = 0; i < contents.size(); i++) {
		if(contents[i].ID == ID) {
			return &contents[i];
		}
	}
	return NULL;
}
