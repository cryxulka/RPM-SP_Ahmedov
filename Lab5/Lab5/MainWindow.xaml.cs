using System;
using System.Data;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Windows;

namespace TsvLoader
{
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private void btnLoad_Click(object sender, RoutedEventArgs e)
        {
            // данные Евростата
            string fileUrl = "https://ec.europa.eu/eurostat/api/dissemination/sdmx/2.1/data/demo_gind/?format=TSV&compressed=false";
            string tempFile = Path.GetTempFileName();

            try
            {
                using (WebClient client = new WebClient())
                {
                    client.Encoding = Encoding.UTF8;
                    client.DownloadFile(fileUrl, tempFile);
                }

                DataTable dataTable = ParseTsvFile(tempFile);

                dataGrid.ItemsSource = dataTable.DefaultView;

                File.Delete(tempFile);
            }
            catch (Exception ex)
            {
                MessageBox.Show("Ошибка: " + ex.Message);
            }
        }

        private DataTable ParseTsvFile(string filePath)
        {
            var lines = File.ReadAllLines(filePath, Encoding.UTF8);
            var dataTable = new DataTable();

            var filteredLines = lines.Where(l => !string.IsNullOrWhiteSpace(l) && !l.TrimStart().StartsWith("#")).ToList();

            if (filteredLines.Count == 0)
                return dataTable;

            string headerLine = filteredLines[0];
            string[] headers = headerLine.Split('\t');
            for (int i = 0; i < headers.Length; i++)
            {
                headers[i] = headers[i].Trim().Trim('"');
                dataTable.Columns.Add(headers[i], typeof(string));
            }

            for (int rowIdx = 1; rowIdx < filteredLines.Count; rowIdx++)
            {
                string line = filteredLines[rowIdx];
                if (string.IsNullOrWhiteSpace(line))
                    continue;

                string[] fields = line.Split('\t');

                if (fields.Length < dataTable.Columns.Count)
                {
                    Array.Resize(ref fields, dataTable.Columns.Count);
                }
                else if (fields.Length > dataTable.Columns.Count)
                {
                    fields = fields.Take(dataTable.Columns.Count).ToArray();
                }

                dataTable.Rows.Add(fields);
            }

            return dataTable;
        }
    }
}