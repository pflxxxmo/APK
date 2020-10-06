#include "Header.h"

void PORT1(HANDLE readEnd, HANDLE writeEnd, HANDLE hFile, HANDLE hExit, HANDLE hWork)
{
    string buffer;
    while (WaitForSingleObject(hExit, 1) == WAIT_TIMEOUT)
    {
        if (WaitForSingleObject(hWork, 1) != WAIT_TIMEOUT)
        {
            WaitForSingleObject(writeEnd, INFINITE);
            if (WaitForSingleObject(hExit, 1) != WAIT_TIMEOUT)
                return;
            buffer.clear();
            buffer = read(hFile);
            cout << "Message from COM2: " << buffer << endl;
            SetEvent(readEnd);
        }
        SetEvent(hWork);
        printf("Message: ");
        getline(cin, buffer);
        buffer += '\n';
        if (!write(hFile, buffer))
        {
            SetEvent(hExit);
            SetEvent(writeEnd);
            return;
        }
        else
        {
            SetEvent(writeEnd);
            WaitForSingleObject(readEnd, INFINITE);
        }
    }
}

int Server()
{
    HANDLE readEnd = CreateEvent(NULL, FALSE, FALSE, "readEnd");
    HANDLE writeEnd = CreateEvent(NULL, FALSE, FALSE, "writeEnd");
    HANDLE hExit = CreateEvent(NULL, FALSE, FALSE, "exit");
    HANDLE hWork = CreateEvent(NULL, FALSE, FALSE, "work");

    HANDLE hFile = CreateFile("COM1", GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hFile == NULL)
    {
        printf("Error\n");
        exit(GetLastError());
    }
    SetCommMask(hFile, EV_RXCHAR);                      // Устанавливаем маску на события порта.
    SetupComm(hFile, 1500, 1500);                       // Инициализирует коммуникационные параметры для заданного устройства (Дескриптор, буфер ввода-вывода)

    COMMTIMEOUTS CommTimeOuts;                          // Структура, характеризующая временные параметры последовательного порта.
    CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;      // Mаксимальное время для интервала между поступлением двух символов по линии связи.
    CommTimeOuts.ReadTotalTimeoutMultiplier = 0;        // Множитель, используемый, чтобы вычислить полный период времени простоя для операций чтения.
    CommTimeOuts.ReadTotalTimeoutConstant = 1200;       // Константа, используемая, чтобы вычислить полный (максимальный) период времени простоя для операций чтения.
    CommTimeOuts.WriteTotalTimeoutMultiplier = 0;       // Множитель, используемый, чтобы вычислить полный период времени простоя для операций записи.
    CommTimeOuts.WriteTotalTimeoutConstant = 1200;      // Константа, используемая, чтобы вычислить полный период времени простоя для операций записи.

    if (!SetCommTimeouts(hFile, &CommTimeOuts))
    {
        CloseHandle(hFile);
        hFile = INVALID_HANDLE_VALUE;
        return 1;
    }

    DCB ComDCM;                               // Структура, характеризующая основные параметры последовательного порта.
    memset(&ComDCM, 0, sizeof(ComDCM));       // Выделение памяти под структуру.
    ComDCM.DCBlength = sizeof(DCB);           // Задает длину, в байтах, структуры.
    GetCommState(hFile, &ComDCM);             // Извлекает данные о текущих настройках управляющих сигналов для указанного устройства.
    ComDCM.BaudRate = setBaudrate();            // Скорость передачи данных.
    ComDCM.ByteSize = 8;                      // Определяет число информационных бит в передаваемых и принимаемых байтах.
    ComDCM.Parity = NOPARITY;                 // Определяет выбор схемы контроля четности (Бит честности отсутствует).
    ComDCM.StopBits = ONESTOPBIT;             // Задает количество стоповых бит (Один бит).
    ComDCM.fAbortOnError = TRUE;              // Задает игнорирование всех операций чтения/записи при возникновении ошибки.
    ComDCM.fDtrControl = DTR_CONTROL_DISABLE; // Задает режим управления обменом для сигнала DTR.
    ComDCM.fRtsControl = RTS_CONTROL_DISABLE; // Задает режим управления потоком для сигнала RTS.
    ComDCM.fBinary = TRUE;                    // Включает двоичный режим обмена.
    ComDCM.fParity = FALSE;                   // Включает режим контроля четности.
    ComDCM.fInX = FALSE;                      // Задает использование XON/XOFF управления потоком при приеме.
    ComDCM.fOutX = FALSE;                     // Задает использование XON/XOFF управления потоком при передаче.
    ComDCM.XonChar = 0;                       // Задает символ XON используемый как для приема, так и для передачи.
    ComDCM.XoffChar = (unsigned char)0xFF;    // Задает символ XOFF используемый как для приема, так и для передачи.
    ComDCM.fErrorChar = FALSE;                // Задает символ, использующийся для замены символов с ошибочной четностью.
    ComDCM.fNull = FALSE;                     // Указывает на необходимость замены символов с ошибкой четности на символ задаваемый полем ErrorChar.
    ComDCM.fOutxCtsFlow = FALSE;              // Включает режим слежения за сигналом CTS.
    ComDCM.fOutxDsrFlow = FALSE;              // Включает режим слежения за сигналом DSR.
    ComDCM.XonLim = 128;                      // Задает минимальное число символов в приемном буфере перед посылкой символа XON.
    ComDCM.XoffLim = 128;                     // Определяет максимальное количество байт в приемном буфере перед посылкой символа XOFF.

    if (!SetCommState(hFile, &ComDCM))
    {
        CloseHandle(hFile);
        hFile = INVALID_HANDLE_VALUE;
        return 2;
    }

    PORT1(readEnd, writeEnd, hFile, hExit, hWork);

    CloseHandle(hFile);
    return 0;
}

int main()
{
    STARTUPINFO si;
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    PROCESS_INFORMATION procInfo;
    ZeroMemory(&procInfo, sizeof(PROCESS_INFORMATION));
    if (!CreateProcess(NULL, (char*)"D:\\Visual Studio\\5\\TOKS\\client\\Debug\\client.exe", NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &procInfo)) \\ПУТЬ К КЛИЕНТУ!?!?!?!?!
    {
        cout << "CreateProcess failed " << GetLastError() << endl;
        return 0;
    }

    system("cls");
    puts("COM1 - port | Print 'exit' to quit");
    int response = Server();
    if (!response)
        puts("Work ended");
    else
        puts("COM1 can not be opened");
    return 0;
}