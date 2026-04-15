var builder = WebApplication.CreateBuilder(args);
var app = builder.Build();

// создаем список минимум из 10 аргументов (некоторые повторяются)
List<string> subjects = new List<string>
{
    "biology", "chemistry", "physics", "astronomy", "biology",
    "art", "literature", "physics", "geometry", "history"
};

// вывод всего списка
app.MapGet("/api/subjects", () => subjects);

// вывод предпоследнего элемента и элементов по выбору
app.MapGet("/api/subjects/special", () => {
    if (subjects.Count < 2) return Results.BadRequest("Список слишком мал");

    return Results.Ok(new
    {
        Penultimate = subjects[subjects.Count - 2], // предпосл. элемент (аналог index -2)
        First = subjects[0],                        // первый элемент
        Third = subjects[2]                         // элемент на позиции 2
    });
});

// изменение элемента в определенной позиции
app.MapPut("/api/subjects/{index}", (int index, string newValue) => {
    if (index < 0 || index >= subjects.Count) return Results.NotFound("Индекс вне диапазона");

    subjects[index] = newValue; // изменяем элемент
    return Results.Ok(subjects);
});

// вывод элементов в определенном диапазоне
app.MapGet("/api/subjects/range", (int start, int count) => {
    if (start < 0 || (start + count) > subjects.Count) return Results.BadRequest("Неверный диапазон");

    var range = subjects.GetRange(start, count);
    return Results.Ok(range);
});

// добавление элемента в список (в конец)
app.MapPost("/api/subjects/append", (string item) => {
    subjects.Add(item); // Метод Add (аналог append)
    return Results.Ok(subjects);
});

// добавление элемента в середину списка
app.MapPost("/api/subjects/insert", (int index, string item) => {
    if (index < 0 || index > subjects.Count) return Results.BadRequest("Неверный индекс");

    subjects.Insert(index, item); // Метод Insert
    return Results.Ok(subjects);
});

// посчитать количество определенных элементов
app.MapGet("/api/subjects/count/{name}", (string name) => {
    int count = subjects.Count(s => s == name); // Подсчет вхождений
    return Results.Ok(new { element = name, count = count });
});

// копирование списка, изменение первого и сравнение
app.MapGet("/api/subjects/compare", () => {
    // создаем копию списка
    List<string> subjectsCopy = new List<string>(subjects);

    // изменяем первый элемент в оригинале
    if (subjects.Count > 0) subjects[0] = "MODIFIED_SUBJECT";

    return Results.Ok(new
    {
        OriginalList = subjects,
        CopiedList = subjectsCopy
    });
});

app.Run();