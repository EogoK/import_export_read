#include "l.h"
#include "import.h"

class ExportFuncCheck {
public:

	ExportFuncCheck(FILE *file, unsigned long offset, unsigned long va) noexcept : files(file), raw_offset_s(offset), va_offset_s(va){
		cout << hex;
		ExportDirectoryTable(file, va_offset_s, raw_offset_s);
		for (int i = 0; i < NumberOfNamePointers; i++) {
			fseek(file, ExportAddressTable, SEEK_SET);
			cout << "Address Export: " << ExportAddressTable_s(file, i) << endl;
			fseek(file, NamePointer, SEEK_SET);
			cout << "Name Export: ";
			ExportNameTable_s(file, i);
		}
	}

private:

	void ExportDirectoryTable(FILE* file, unsigned long va, unsigned long offset) {
		unsigned long temp = 0;
		fseek(file, 20, SEEK_CUR);
		AddressTableEntries = data_to_real(file, 4, ftell(file));
		fseek(file, 4, SEEK_CUR);
		NumberOfNamePointers = data_to_real(file, 4, ftell(file));
		fseek(file, 4, SEEK_CUR);
		ExportAddressTable = RvaToRaw(file, 4, va_offset_s, raw_offset_s, temp);
		cout << "ExportAddressTable: " << temp << endl;
		fseek(file, 4, SEEK_CUR);
		NamePointer = RvaToRaw(file, 4, va_offset_s, raw_offset_s, temp);
		cout << "NamePointer: " << temp << endl;
		fseek(file, 4, SEEK_CUR);
		OrdinalTable = RvaToRaw(file, 4, va_offset_s, raw_offset_s, temp);
		cout << "OrdinalTable: " << temp << endl;
		fseek(file, 4, SEEK_CUR);
	}

	unsigned long ExportAddressTable_s(FILE *file, size_t it) {
		fseek(file, it << 2, SEEK_CUR);
		if (ftell(file)+4 > NamePointer){
			return 0;
		}
		return data_to_real(file, 4, ftell(file));
	}
	unsigned long ExportNameTable_s(FILE *file, size_t it) {
		fseek(file, it << 2, SEEK_CUR);
		unsigned long tmp;
		char s;
		unsigned long temp = RvaToRaw(file, 4, va_offset_s, raw_offset_s, tmp);

		fseek(file, temp, SEEK_CUR);
		while (1) {
			s = fgetc(file);
			if (s != 0) {
				cout << s;
			}
			else {
				cout << endl;
				break;
			}
		}
	}

	FILE *files;
	unsigned long AddressTableEntries;
	unsigned long NumberOfNamePointers;
	unsigned long ExportAddressTable;
	unsigned long NamePointer;
	unsigned long OrdinalTable;
	unsigned long raw_offset_s;
	unsigned long va_offset_s;
};