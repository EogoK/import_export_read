#include "l.h"

template <typename T>
T data_to_real(FILE *file, T range, long begin) {
	T k = 0, r = 0;
	for (int i = 0; i < range; i++) {
		fseek(file, begin + range - i - 1, SEEK_SET);
		k = fgetc(file);
		r += k << (8 * (range - i - 1));
	}
	fseek(file, begin, SEEK_SET);
	return r;
}
template<typename T>
T RvaToRaw(FILE *file, T range, unsigned long va, unsigned long raw, unsigned long &tmp) {
	T n = 0, temp = 0;
	temp = data_to_real(file, range, ftell(file));
	n = temp ? temp - va + raw : 0;
	tmp = temp;
	return n;
}

class ImportFuncCheck {
public:
	
	ImportFuncCheck(FILE *file, unsigned long offset, unsigned long va) noexcept : files(file), raw_offset_s(offset), va_offset_s(va){
		StandartImportFunc(file);
	}

private:

	void StandartImportFunc(FILE *file) {
		cout << hex;
		fseek(file, raw_offset_s, SEEK_SET);
		long long OriginalFirstTnunk = 0, FirstThunk = 0, TimeDateStamp = 0, ForwarderChain = 0, RawEndSection = 0;
		long Name = 0;
		unsigned long temp = 0;
		int t2 = 0;
		while (1) {
			if (RawEndSection <= (ftell(file)+20) && RawEndSection != 0) {
				break;
			}
			cout << endl;
			cout << "i tut: " << ftell(file) << endl;
			OriginalFirstTnunk = RvaToRaw(file, 4, va_offset_s, raw_offset_s, temp);
			cout << "OriginalFirstThunk: " << OriginalFirstTnunk << " " << temp << endl;
			fseek(file, 4, SEEK_CUR);
			TimeDateStamp = data_to_real(file, 4, ftell(file));
			cout << "TimeDateStamp: " << TimeDateStamp << endl;
			fseek(file, 4, SEEK_CUR);
			ForwarderChain = data_to_real(file, 4, ftell(file));
			cout << "ForwarderChain: " << ForwarderChain << endl;
			fseek(file, 4, SEEK_CUR);
			Name = RvaToRaw(file, 4, va_offset_s, raw_offset_s, temp);
			cout << "Name_RAW: " << Name << " " << temp << endl;
			fseek(file, 4, SEEK_CUR);
			FirstThunk = RvaToRaw(file, 4, va_offset_s, raw_offset_s, temp);
			cout << "FirstThunk: " << FirstThunk <<" "<< temp << endl;
			fseek(file, 4, SEEK_CUR);
			if (t2 == 1) {
				t2 = 0;
				RawEndSection = OriginalFirstTnunk;
			}
			temp = 0;

			temp = ftell(file);
			fseek(file, Name, SEEK_SET);
			if (Name != 0) {
				while (!0) {
					t2 = fgetc(file);
					if (t2 != 0) {
						cout << (char)t2;
					}
					else {
						t2 = 0;
						break;
					}
				}
			}
			cout << endl;
			
			if ((OriginalFirstTnunk | FirstThunk | Name | ForwarderChain | TimeDateStamp) == 0) {
				t2 = 1;
			}

			fseek(file, temp, SEEK_SET);
			temp = 0;
		}
		cout << dec;
	}

	FILE *files;
	unsigned long raw_offset_s;
	unsigned long va_offset_s;
};