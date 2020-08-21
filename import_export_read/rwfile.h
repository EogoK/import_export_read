#include "import.h"


class CreateFileZW{
public:

	CreateFileZW(string NameFile) noexcept : name(NameFile) {
		HandleFile_cfzw(name);
	}


private:

	int ie_search(FILE *file, string str) {
		int n = 0, c = 0;
		cout << hex;
		cout << "VA " << str << " " ;
		for (int i = 0; i < 8; i++) {
			c = fgetc(file);
			if (c != -1) {
				if (c <= 15) {
					cout << "0" << c;
				}
				else {
					cout << c;
				}
			}
			if (!c) {
				n++;
			}
		}
		if (n == 8) {
			cout << endl;
			return 1;
		}
		cout << endl;
		cout << dec;
		return 0;
	}

	string sort_section_header(FILE *file, string str, string section_name, int numberOfSections) {
		int cp = ftell(file);
		int tableSize = 0;
		unsigned long VAsec = 0, RAWsec = 0;
		cout << hex;
		cout << endl << "section header ("+str+"): " << endl;
		string sn;

		sn.append(section_name);

		if (sn.length() != 8) {
			for (int i = sn.length(); i < 8; i++) {
				sn.append(" ");
			}
		}
		int n = 0, st = 0;
		while (numberOfSections) {
			n = 0;
			for (int i = 0; i < 8; i++) {
				st = fgetc(file);
				cout << (char)st << "";
				if ((st == (int)sn[i]) || ((st == 0) && (sn[i] == ' '))) {
					n++;
				}
			}
			if (n == 8) {
				cout << "nice "+section_name << endl;

				fseek(file, 4, SEEK_CUR);
				VAsec = data_to_real(file, 4, ftell(file));

				fseek(file, 4, SEEK_CUR);
				tableSize = data_to_real(file, 4, ftell(file));

				fseek(file, 4, SEEK_CUR);
				RAWsec = data_to_real(file, 4, ftell(file));

				fseek(file, RAWsec, SEEK_SET);

				ImportFuncCheck imp(file, RAWsec, VAsec);

				break;
			}
			else {
				fseek(file, 32, SEEK_CUR);
			}
			cout << endl;
			numberOfSections--;
		}

		fseek(file, cp, SEEK_SET);
		return "ok";
	}

	string fmzpeopt(FILE *file) {

		int NumberOfSections = 0;

		if (fgetc(file) == 'M') {
			if (fgetc(file) == 'Z') {

				fseek(file, 0x3c, SEEK_SET);
				int dw = data_to_real(file, 4, 0x3c);

				if (dw) {
					cout << "coff PA " << hex << dw << endl;
					fseek(file, dw, SEEK_SET);

					fseek(file, dw + 6, SEEK_SET);
				    NumberOfSections = data_to_real(file, 2, dw + 6);

					fseek(file, dw, SEEK_SET);
					if (fgetc(file) == 'P') {
						if (fgetc(file) == 'E') {
							cout << ftell(file) << endl;

							fseek(file, 22, SEEK_CUR);

							if (fgetc(file) == 0x0b) {
								switch (fgetc(file)) {
								case 1: {
									fseek(file, 94, SEEK_CUR);

									int e = ie_search(file, "export 0x10b");//export
									int i = ie_search(file, "import 0x10b");//import

									fseek(file, 112, SEEK_CUR);

									if (!e) {
										sort_section_header(file, "export", ".edata", NumberOfSections);
									}
									else {
										cout << "export table not found" << endl;
									}

									if (!i) {
										sort_section_header(file, "import", ".idata", NumberOfSections);
									}
									else {
										cout << "import table not found" << endl;
									}
									break;
								}
								case 2: {
									cout << ftell(file) << endl;

									fseek(file, 110, SEEK_CUR);

									cout << ftell(file) << endl;

									int e = ie_search(file, "export 0x20b");//export
									int i = ie_search(file, "import 0x20b");//import

									fseek(file, 112, SEEK_CUR);
									if (!e) {
										sort_section_header(file, "export", ".edata", NumberOfSections);
									}
									else {
										cout << "export table not found" << endl;
									}

									if (!i) {
										sort_section_header(file, "import", ".idata", NumberOfSections);
									}
									else {
										cout << "import table not found" << endl;
									}
									break;
								}
								default: {
									return "problem with pe_option_header";
									break;
								}
								}
							}
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
		return "ok";
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