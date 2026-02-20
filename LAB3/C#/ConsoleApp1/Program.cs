using System;
using System.Runtime.InteropServices;
using Excel = Microsoft.Office.Interop.Excel;

namespace ReadExcelInterop
{
    class Program
    {
        static void Main(string[] args)
        {
            // полный путь к файлу
            string filePath = @"C:\Users\FoxOS_User\source\repos\ConsoleApp1\ConsoleApp1\lab3.xlsx";

            Excel.Application excelApp = null;
            Excel.Workbook workbook = null;
            Excel.Worksheet worksheet = null;
            Excel.Range range = null;

            try
            {
                // Запуск Excel
                excelApp = new Excel.Application();
                excelApp.Visible = false;
                excelApp.DisplayAlerts = false;

                // Открытие книги
                workbook = excelApp.Workbooks.Open(filePath);

                // Лист Студенты
                Console.WriteLine("=== Студенты ===");
                worksheet = workbook.Worksheets["Студенты"];
                range = worksheet.UsedRange;

                int rowCount = range.Rows.Count;
                int colCount = range.Columns.Count;

                for (int row = 2; row <= rowCount; row++) // начало со 2 строки
                {
                    int id = Convert.ToInt32(range.Cells[row, 1].Value2);
                    string name = range.Cells[row, 2].Value2?.ToString() ?? "";
                    int age = Convert.ToInt32(range.Cells[row, 3].Value2);
                    string group = range.Cells[row, 4].Value2?.ToString() ?? "";
                    Console.WriteLine($"{id}\t{name}\t{age}\t{group}");
                }

                // Лист Оценки
                Console.WriteLine("\n=== Оценки ===");
                worksheet = workbook.Worksheets["Оценки"];
                range = worksheet.UsedRange;

                rowCount = range.Rows.Count;
                for (int row = 2; row <= rowCount; row++)
                {
                    int id = Convert.ToInt32(range.Cells[row, 1].Value2);
                    int math = Convert.ToInt32(range.Cells[row, 2].Value2);
                    int physics = Convert.ToInt32(range.Cells[row, 3].Value2);
                    int cs = Convert.ToInt32(range.Cells[row, 4].Value2);
                    Console.WriteLine($"{id}\t{math}\t{physics}\t{cs}");
                }

                // Лист Преподаватели
                Console.WriteLine("\n=== Преподаватели ===");
                worksheet = workbook.Worksheets["Преподаватели"];
                range = worksheet.UsedRange;

                rowCount = range.Rows.Count;
                for (int row = 2; row <= rowCount; row++)
                {
                    int id = Convert.ToInt32(range.Cells[row, 1].Value2);
                    string fullName = range.Cells[row, 2].Value2?.ToString() ?? "";
                    string subject = range.Cells[row, 3].Value2?.ToString() ?? "";
                    Console.WriteLine($"{id}\t{fullName}\t{subject}");
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Ошибка: " + ex.Message);
            }
            finally
            {
                // Освобождение ресурсов
                if (range != null) Marshal.ReleaseComObject(range);
                if (worksheet != null) Marshal.ReleaseComObject(worksheet);
                if (workbook != null)
                {
                    workbook.Close(false);
                    Marshal.ReleaseComObject(workbook);
                }
                if (excelApp != null)
                {
                    excelApp.Quit();
                    Marshal.ReleaseComObject(excelApp);
                }

                Console.WriteLine("\nНажмите Enter для выхода...");
                Console.ReadLine();
            }
        }
    }
}