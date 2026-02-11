using System;

namespace AutoSalon
{
    // Базовый класс Vehicle
    class Vehicle
    {
        public string Make { get; set; }   // марка
        public string Model { get; set; }  // модель
        public int Year { get; set; }      // год выпуска

        // Конструктор базового класса
        public Vehicle(string make, string model, int year)
        {
            // Передача данных: параметры присваиваются свойствам
            Make = make;
            Model = model;
            Year = year;
        }

        // Виртуальный метод (может быть переопределён в подклассах)
        public virtual string Info()
        {
            return $"{Year} {Make} {Model}";
        }
    }

    // Подкласс Car — наследует Vehicle
    class Car : Vehicle
    {
        public int Doors { get; set; }          // количество дверей
        public string BodyStyle { get; set; }   // стиль кузова

        // Конструктор подкласса Car
        public Car(string make, string model, int year, int doors, string bodyStyle)
            : base(make, model, year)   // вызов конструктора базового класса (передача make, model, year)
        {
            // Собственные атрибуты
            Doors = doors;
            BodyStyle = bodyStyle;
        }

        // Переопределённый метод (полиморфизм)
        public override string Info()
        {
            return $"{base.Info()} | {Doors} двери | кузов: {BodyStyle}";
        }
    }

    // Подкласс Truck — наследует Vehicle
    class Truck : Vehicle
    {
        public double BedLength { get; set; }    // длина кузова
        public int TowingCapacity { get; set; }   // тяговая мощность

        // Конструктор подкласса Truck
        public Truck(string make, string model, int year, double bedLength, int towingCapacity)
            : base(make, model, year)   // вызов конструктора базового класса
        {
            BedLength = bedLength;
            TowingCapacity = towingCapacity;
        }

        // Переопределённый метод (полиморфизм)
        public override string Info()
        {
            return $"{base.Info()} | кузов: {BedLength} м | тяга: {TowingCapacity} кг";
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("=== Автосалон: объекты и наследование ===\n");

            // Объект базового класса
            Vehicle vehicle = new Vehicle("Toyota", "Camry", 2020);
            Console.WriteLine("Базовый класс Vehicle:");
            Console.WriteLine(vehicle.Info());
            Console.WriteLine();

            // Объект подкласса Car
            Car car = new Car("Honda", "Civic", 2022, 4, "седан");
            Console.WriteLine("Подкласс Car (наследует Vehicle):");
            Console.WriteLine(car.Info());
            Console.WriteLine($"  Атрибуты Car: марка={car.Make}, модель={car.Model}, год={car.Year}, двери={car.Doors}, кузов={car.BodyStyle}");
            Console.WriteLine();

            // Объект подкласса Truck
            Truck truck = new Truck("Ford", "F-150", 2023, 2.5, 5000);
            Console.WriteLine("Подкласс Truck (наследует Vehicle):");
            Console.WriteLine(truck.Info());
            Console.WriteLine($"  Атрибуты Truck: марка={truck.Make}, модель={truck.Model}, год={truck.Year}, длина кузова={truck.BedLength} м, тяга={truck.TowingCapacity} кг");
        }
    }
}