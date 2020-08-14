#include "l.h"

template <typename T>
T data_to_real(FILE *file, T range, T begin) {
	T k = 0;
	for (int i = 0; i < range; i++) {
		fseek(file, begin + range - i - 1, SEEK_SET);
		k += fgetc(file);
	}

	return k;
}

class CreateFileZW{
public:

	CreateFileZW(string NameFile) noexcept : name(NameFile) {
		HandleFile_cfzw(name);
	}


private:

	int ie_search(FILE *file, string str) {
		int n = 0, c = 0;
		cout << "VA " << str << " ";
		for (int i = 0; i < 8; i++) {
			c = fgetc(file);
			cout << hex << c;
			if (!c) {
				n++;
			}
		}
		if (n == 8) {
			return 1;
		}
		return 0;
	}

	string fmzpeopt(FILE *file) {
		if (fgetc(file) == 'M') {
			if (fgetc(file) == 'Z') {

				fseek(file, 0x3c, SEEK_SET);
				int dw = data_to_real(file, 4, 0x3c);

				if (dw) {
					cout << "coff PA " << hex << dw << endl;
					fseek(file, dw, SEEK_SET);

					if (fgetc(file) == 'P') {
						if (fgetc(file) == 'E') {
							fseek(file, 21, SEEK_CUR);
							
							switch (fgetc(file)){
								case 1:{
									if (fgetc(file) == 0x0b) {
										fseek(file, 97, SEEK_CUR);
										
										int e = ie_search(file, "export 0x10b");//export
										int i = ie_search(file, "import 0x10b");//import

									}
									else {
										return "problem with pe_option_header(10b)";
									}
									break;
								}
								case 2:{
									if (fgetc(file) == 0x0b) {
										fseek(file, 111, SEEK_CUR);
										
										int e = ie_search(file, "export 0x20b");//export
										int i = ie_search(file, "import 0x20b");//import

									}
									else{
										return "problem with pe_option_header(20b)";
									}
									break;
								}
								default: {
									return "problem with pe_option_header";
									break;
								}
							}
							return 0;
						}
						else {
							return "coff sign not found (E)";
						}
					}
					else {
						return "coff sign not found (P)";
					}
				}
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