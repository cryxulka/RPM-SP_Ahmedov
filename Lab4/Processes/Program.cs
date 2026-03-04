using System.Diagnostics;

// 1. Получить все запущенные процессы
Console.WriteLine("=== Все запущенные процессы ===");
foreach (Process proc in Process.GetProcesses())
{
    // Выводим ID и имя процесса
    Console.WriteLine($"ID: {proc.Id}  Name: {proc.ProcessName}");
}

// 2. Получить все потоки процесса Visual Studio (Windows)
Console.WriteLine("\n=== Потоки процесса Visual Studio (devenv) ===");
Process[] vsProcs = Process.GetProcessesByName("devenv"); // для Windows
if (vsProcs.Length > 0)
{
    ProcessThreadCollection threads = vsProcs[0].Threads; // берём первый найденный процесс
    foreach (ProcessThread thread in threads)
    {
        Console.WriteLine($"Thread ID: {thread.Id}");
    }
}
else
{
    Console.WriteLine("Процесс Visual Studio не найден.");
}

// 3. Запустить несколько сторонних процессов
Console.WriteLine("\n=== Запуск сторонних процессов ===");

// Запускаем Блокнот (Windows)
Process.Start("notepad.exe");

// Запускаем Калькулятор (Windows)
Process.Start("calc.exe");

// Запускаем браузер Chrome с параметром (ссылкой)
Process.Start(@"C:\Program Files\Google\Chrome\Application\chrome.exe", "https://urpc.ru/");

Console.WriteLine("Процессы запущены.");