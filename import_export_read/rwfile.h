#include "l.h"

template <typename T>
T data_to_real(FILE *file, T range) {

	for (int i = 0; i < range; i++) {
		fseek(file, range - i - 1, SEEK_SET);
		cout << (char)fgetc(file) << endl;
	}

	return 0;
}

class CreateFileZW{
public:

	CreateFileZW(string NameFile) noexcept : name(NameFile) {
		HandleFile_cfzw(name);
	}


private:
	
	void fmzpeopt(FILE *file) {
		if (fgetc(file) == 'M') {
			if (fgetc(file) == 'Z') {
				fseek(file, 0x3c, SEEK_SET);
				data_to_real(file, 4);
			}
		}
	}

	void HandleFile_cfzw(string files) {

		FILE *file = fopen(files.c_str(), "rb+");

		fmzpeopt(file);

		fseek(file, 0, SEEK_SET);

		int s = 0, n = 0;
		while (s != EOF) {
			s = fgetc(file);
			if (s != -1) {
				if (s <= 15) {
					cout << "0" << hex << s << " ";
				}
				else {
					cout << hex << s << " ";
				}
			}
			else {
				break;
			}
			n++;
		} 

		fclose(file);
	}

	string name;

};