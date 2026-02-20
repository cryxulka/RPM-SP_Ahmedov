using Excel = Microsoft.Office.Interop.Excel;
using System;
using Excel = Microsoft.Office.Interop.Excel;

namespace ExcelReaderCSharp
{
    class Program
    {
        static void Main(string[] args)
        {
            // Путь к файлу Excel
            string filePath = @"lab3.xlsx"; // если файл лежит в папке Debug
            // Либо укажите полный путь: @"C:\Users\...\lab3.xlsx"

            Excel.Application excelApp = null;
            Excel.Workbook workbook = null;
            Excel.Sheets sheets = null;

            try
            {
                // Шаг 1: Запуск приложения Excel
                excelApp = new Excel.Application();
                excelApp.Visible = false; // не показывать окно Excel

                // Шаг 2: Открытие книги
                workbook = excelApp.Workbooks.Open(filePath);
                sheets = workbook.Sheets;

                // Шаг 3: Перебор всех листов
                for (int i = 1; i <= sheets.Count; i++)
                {
                    Excel.Worksheet sheet = (Excel.Worksheet)sheets[i];
                    Console.WriteLine($"\n--- Лист {i}: {sheet.Name} ---");

                    // Определяем используемый диапазон на листе
                    Excel.Range usedRange = sheet.UsedRange;
                    int rowCount = usedRange.Rows.Count;
                    int colCount = usedRange.Columns.Count;

                    // Чтение и вывод данных
                    for (int row = 1; row <= rowCount; row++)
                    {
                        for (int col = 1; col <= colCount; col++)
                        {
                            Excel.Range cell = usedRange.Cells[row, col];
                            object cellValue = cell.Value2;

                            if (cellValue != null)
                                Console.Write(cellValue.ToString().PadRight(15)); // выравнивание
                            else
                                Console.Write("".PadRight(15));
                        }
                        Console.WriteLine(); // переход на следующую строку
                    }

                    // Освобождение COM-объекта листа (важно!)
                    System.Runtime.InteropServices.Marshal.ReleaseComObject(usedRange);
                    System.Runtime.InteropServices.Marshal.ReleaseComObject(sheet);
                }

                Console.WriteLine("\nЧтение завершено. Нажмите любую клавишу для выхода...");
                Console.ReadKey();
            }
            catch (Exception ex)
            {
                Console.WriteLine("Ошибка: " + ex.Message);
            }
            finally
            {
                // Шаг 4: Закрытие книги и завершение приложения Excel
                if (workbook != null)
                {
                    workbook.Close(false); // false - не сохранять изменения
                    System.Runtime.InteropServices.Marshal.ReleaseComObject(workbook);
                }

                if (excelApp != null)
                {
                    excelApp.Quit();
                    System.Runtime.InteropServices.Marshal.ReleaseComObject(excelApp);
                }

                // Принудительная сборка мусора для освобождения COM-объектов
                GC.Collect();
                GC.WaitForPendingFinalizers();
            }
        }
    }
}