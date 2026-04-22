using System;
using System.IO;
using System.IO.Pipes;
using System.Text.Json;
using System.Threading.Tasks;

namespace NamedPipeServer
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
            Console.WriteLine("Сервер запущен. Ожидание подключения клиента...");

            // Создаём именованный канал с именем "MyDataPipe"
            using (var server = new NamedPipeServerStream("MyDataPipe", PipeDirection.InOut))
            {
                // Ожидаем подключения клиента
                server.WaitForConnection();
                Console.WriteLine("Клиент подключён.");

                // Создаём reader и writer для работы с текстовыми данными
                using (var reader = new StreamReader(server))
                using (var writer = new StreamWriter(server) { AutoFlush = true })
                {
                    // Получаем данные от клиента
                    string? receivedJson = await reader.ReadLineAsync();
                    if (receivedJson != null)
                    {
                        var person = JsonSerializer.Deserialize<Person>(receivedJson);
                        Console.WriteLine($"Получен объект Person: Id = {person?.Id}, Name = {person?.Name}, Age = {person?.Age}");

                        // Модифицируем объект (например, увеличиваем возраст)
                        if (person != null)
                        {
                            person.Age += 1;
                            person.Name = person.Name.ToUpper();

                            // Отправляем изменённый объект обратно клиенту
                            string responseJson = JsonSerializer.Serialize(person);
                            await writer.WriteLineAsync(responseJson);
                            Console.WriteLine($"Отправлен изменённый объект: {responseJson}");
                        }
                    }
                }

                // Закрытие канала происходит автоматически при выходе из using
            }

            Console.WriteLine("Сеанс сервера завершён. Нажмите любую клавишу...");
            Console.ReadKey();
        }
    }
}