#include "Header.h"


string read(HANDLE hFile)
{
	char buf = '\0';
	string message;

	DWORD bytesReaded;
	do
	{
		if (!ReadFile(hFile, &buf, 1, &bytesReaded, NULL))
			cout << GetLastError() << endl;
		if (buf == '\n') break;
		message += buf;
	} while (1);

	cout << "Received DATA: " << message << endl;
	message = unpackData(message);
	cout << "Unpacked DATA: " << message << endl;

	return decode(message);
}


bool write(HANDLE hFile, string buffer)
{
	int i = 0;

	buffer.pop_back();
	buffer = encode(buffer);
	cout << "Encoded: " << buffer << endl;
	buffer = packData(buffer);
	cout << "Packed: " << buffer << endl;
	buffer += '\n';

	cout << endl;
	while (i < buffer.size())
	{
		WriteFile(hFile, &(buffer[i++]), 1, NULL, NULL);
	}

	buffer = unpackData(buffer);
	buffer = decode(buffer);
	buffer += '\n';
	if (!strcmp(buffer.c_str(), "exit\n"))
		return false;
	else
		return true;
}

DWORD setBaudrate()
{
	DWORD baudRate;

	cout << "Select baudrate:" << endl << "0. 110" << endl << "1. 9600" << endl << "2. 14400" << endl << "3. 19200" << endl << "4. 38400" << endl << "5. 56000" << endl << "6. 256000" << endl;
	switch (_getch()) {

	case('0'):
		system("cls");
		baudRate = CBR_110;
		break;

	case('1'):
		system("cls");
		baudRate = CBR_9600;
		break;

	case('2'):
		system("cls");
		baudRate = CBR_14400;
		break;

	case('3'):
		system("cls");
		baudRate = CBR_19200;
		break;

	case('4'):
		system("cls");
		baudRate = CBR_38400;
		break;

	case('5'):
		system("cls");
		baudRate = CBR_56000;
		break;

	case('6'):
		system("cls");
		baudRate = CBR_256000;
		break;

	default:
		system("cls");
		baudRate = CBR_19200;
		cout << "Wrong input. Default 19200 baudrate is set" << endl;
		break;

	}
	return baudRate;
}

string encode(string buffer) {

	string encodedData;
	int i = 0;
	while (i < buffer.size())
		encodedData += bitset<8>(buffer[i++]).to_string();

	return encodedData;
}

string decode(string buffer) {
	string decodedData;
	for (int i = 0; i < buffer.size() / 8; i++)
	{
		decodedData += strtol(buffer.substr(i * 8, 8).c_str(), NULL, 2);
	}
	return decodedData;
}

string packData(string buffer) {

	int i = 0;
	int parityBit = 0;
	size_t pos = 0;
	while (1) {

		if ((pos = buffer.find(flag)) != -1) {

			pos += 7;
			buffer.insert(pos, "1");
		}
		else
			break;
	}
	while (i < buffer.size())
		if (buffer[i++] == '1')
			parityBit++;
	if (parityBit % 2)
		buffer += "1";
	else
		buffer += "0";
	return flag + buffer;
}

string unpackData(string packedData) {

	string buffer;
	int i = 0;
	int parity = 0;
	size_t pos = 0;

	if ((pos = packedData.find(flag)) == -1)
		return buffer;
	packedData.erase(0, pos + 8);
	while (i < packedData.size())
		if (packedData[i++] == '1')
			parity++;
	if (parity % 2)
		return buffer;
	buffer = packedData.erase(packedData.size() - 1, 1);
	while (1) {

		if ((pos = buffer.find(stuffed)) != -1) {

			buffer.erase(pos + 7, 1);
		}
		else
			break;
	}
	return buffer;
}	