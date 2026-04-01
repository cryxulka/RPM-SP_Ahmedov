using System;
using System.Threading;
using OfficeOpenXml;

class Program
{
    static void Main(string[] args)
    {
        ExcelPackage.LicenseContext = LicenseContext.NonCommercial;

        string filePath = @"C:\Users\FoxOS_User\Documents\LAB6.xlsx";

        Thread thread1 = new Thread(() => ReadExcelSheet(filePath, "Лист1", false));
        Thread thread2 = new Thread(() => ReadExcelSheet(filePath, "Лист2", true));

        thread1.Name = "Поток для Лист1";
        thread2.Name = "Поток для Лист2";

        Console.WriteLine($"Статус {thread1.Name}: {thread1.ThreadState}");
        Console.WriteLine($"Статус {thread2.Name}: {thread2.ThreadState}");

        thread1.Start();
        thread2.Start();

        Console.WriteLine($"\nПосле запуска:");
        Console.WriteLine($"{thread1.Name}: IsAlive = {thread1.IsAlive}, State = {thread1.ThreadState}");
        Console.WriteLine($"{thread2.Name}: IsAlive = {thread2.IsAlive}, State = {thread2.ThreadState}");

        thread1.Join();
        thread2.Join();

        Console.WriteLine("\nОба потока завершены.");
        Console.ReadKey();
    }

    static void ReadExcelSheet(string filePath, string sheetName, bool withDelay)
    {
        Console.WriteLine($"\n[{Thread.CurrentThread.Name}] Начало чтения листа '{sheetName}'");

        if (!System.IO.File.Exists(filePath))
        {
            Console.WriteLine($"[{Thread.CurrentThread.Name}] Файл '{filePath}' не найден.");
            return;
        }

        using (var package = new ExcelPackage(new System.IO.FileInfo(filePath)))
        {
            var worksheet = package.Workbook.Worksheets[sheetName];
            if (worksheet == null)
            {
                Console.WriteLine($"[{Thread.CurrentThread.Name}] Лист '{sheetName}' не найден.");
                return;
            }

            int rows = worksheet.Dimension.Rows;
            int cols = worksheet.Dimension.Columns;
            Console.WriteLine($"[{Thread.CurrentThread.Name}] Размер листа: {rows} x {cols}");

            if (withDelay)
            {
                Console.WriteLine($"[{Thread.CurrentThread.Name}] Задержка 2 секунды...");
                Thread.Sleep(2000);
            }

            Console.WriteLine($"\nСодержимое листа '{sheetName}':");
            for (int r = 1; r <= rows; r++)
            {
                string line = "";
                for (int c = 1; c <= cols; c++)
                {
                    line += worksheet.Cells[r, c].Text + "\t";
                }
                Console.WriteLine(line);
            }
        }

        Console.WriteLine($"\n[{Thread.CurrentThread.Name}] Завершил чтение листа '{sheetName}'");
    }
}