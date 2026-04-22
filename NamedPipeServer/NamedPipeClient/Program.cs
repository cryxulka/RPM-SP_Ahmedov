using System;
using System.IO;
using System.IO.Pipes;
using System.Text.Json;
using System.Threading.Tasks;

namespace NamedPipeClient
{
    public class Person
    {
        public int Id { get; set; }
        public string Name { get; set; } = "";
        public int Age { get; set; }
    }

    class Program
    {
        static async Task Main(string[] args)
        {
            Console.WriteLine("Клиент запущен. Подключение к серверу...");

            // Подключаемся к именованному каналу
            using (var client = new NamedPipeClientStream(".", "MyDataPipe", PipeDirection.InOut))
            {
                client.Connect();
                Console.WriteLine("Подключение установлено.");

                using (var reader = new StreamReader(client))
                using (var writer = new StreamWriter(client) { AutoFlush = true })
                {
                    // Создаём объект для отправки
                    var person = new Person
                    {
                        Id = 1,
                        Name = "Иван Иванов",
                        Age = 30
                    };

                    string json = JsonSerializer.Serialize(person);
                    Console.WriteLine($"Отправляем объект: {json}");
                    await writer.WriteLineAsync(json);

                    // Получаем ответ от сервера
                    string? responseJson = await reader.ReadLineAsync();
                    if (responseJson != null)
                    {
                        var modifiedPerson = JsonSerializer.Deserialize<Person>(responseJson);
                        Console.WriteLine($"Получен изменённый объект: Id = {modifiedPerson?.Id}, Name = {modifiedPerson?.Name}, Age = {modifiedPerson?.Age}");
                    }
                }
            }

            Console.WriteLine("Сеанс клиента завершён. Нажмите любую клавишу...");
            Console.ReadKey();
        }
    }
}