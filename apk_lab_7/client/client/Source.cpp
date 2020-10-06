#include "Header.h"

void PORT2(HANDLE readEnd, HANDLE writeEnd, HANDLE hFile, HANDLE hExit, HANDLE hWork)
{
    string buffer;
    int size;
    while (WaitForSingleObject(hExit, 1) == WAIT_TIMEOUT)
    {
        if (WaitForSingleObject(hWork, 1) != WAIT_TIMEOUT)
        {
            WaitForSingleObject(writeEnd, INFINITE);
            if (WaitForSingleObject(hExit, 1) != WAIT_TIMEOUT)
                return;
            buffer.clear();
            buffer = read(hFile);
            cout << "Message from COM1: " << buffer << endl;
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

int Client()
{
    HANDLE hFile = CreateFile("COM2", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hFile == NULL)
    {
        printf("Error %d\n", GetLastError());
        exit(GetLastError());
    }
    SetCommMask(hFile, EV_RXCHAR); // Устанавливаем маску на события порта.
    SetupComm(hFile, 1500, 1500);  // Инициализирует коммуникационные параметры для заданного устройства (Дескриптор, буфер ввода-вывода)

    COMMTIMEOUTS CommTimeOuts;                     // Структура, характеризующая временные параметры последовательного порта.
    CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF; // Mаксимальное время для интервала между поступлением двух символов по линии связи.
    CommTimeOuts.ReadTotalTimeoutMultiplier = 0;   // Множитель, используемый, чтобы вычислить полный период времени простоя для операций чтения.
    CommTimeOuts.ReadTotalTimeoutConstant = 1200;  // Константа, используемая, чтобы вычислить полный (максимальный) период времени простоя для операций чтения.
    CommTimeOuts.WriteTotalTimeoutMultiplier = 0;  // Множитель, используемый, чтобы вычислить полный период времени простоя для операций записи.
    CommTimeOuts.WriteTotalTimeoutConstant = 1200; // Константа, используемая, чтобы вычислить полный период времени простоя для операций записи.

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

    HANDLE readEnd = OpenEvent(EVENT_ALL_ACCESS, FALSE, "readEnd");
    HANDLE writeEnd = OpenEvent(EVENT_ALL_ACCESS, FALSE, "writeEnd");
    HANDLE hExit = OpenEvent(EVENT_ALL_ACCESS, FALSE, "exit");
    HANDLE hWork = OpenEvent(EVENT_ALL_ACCESS, FALSE, "work");

    PORT2(readEnd, writeEnd, hFile, hExit, hWork);

    CloseHandle(hFile);
    return 0;
}

int main()
{
    system("cls");
    puts("COM2 - port");
    int response = Client();
    if (!response)
        puts("Work ended");
    else
        puts("COM2 can not be opened");
    return 0;
}