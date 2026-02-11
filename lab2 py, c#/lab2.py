# Базовый класс Vehicle
class Vehicle:
    def __init__(self, make, model, year):
        """
        Конструктор базового класса.
        Передача данных: параметры make, model, year передаются при создании объекта
        и сохраняются в атрибуты экземпляра через self.
        """
        self.make = make      # марка
        self.model = model    # модель
        self.year = year      # год выпуска

    def info(self):
        """Метод для вывода информации о транспортном средстве."""
        return f"{self.year} {self.make} {self.model}"


# Подкласс Car — наследует Vehicle
class Car(Vehicle):
    def __init__(self, make, model, year, doors, body_style):
        """
        Конструктор подкласса Car.
        Передача данных: make, model, year передаются в базовый класс Vehicle через super().
        doors и body_style — собственные атрибуты Car.
        """
        super().__init__(make, model, year)  # вызов конструктора базового класса
        self.doors = doors
        self.body_style = body_style

    def info(self):
        """Переопределённый метод (полиморфизм)."""
        return f"{super().info()} | {self.doors} двери | кузов: {self.body_style}"


# Подкласс Truck — наследует Vehicle
class Truck(Vehicle):
    def __init__(self, make, model, year, bed_length, towing_capacity):
        """
        Конструктор подкласса Truck.
        Передача данных: make, model, year передаются в базовый класс Vehicle.
        bed_length и towing_capacity — собственные атрибуты Truck.
        """
        super().__init__(make, model, year)
        self.bed_length = bed_length      # длина кузова
        self.towing_capacity = towing_capacity  # тяговая мощность

    def info(self):
        """Переопределённый метод (полиморфизм)."""
        return f"{super().info()} | кузов: {self.bed_length} м | тяга: {self.towing_capacity} кг"


# Создание объектов и вывод результата
if __name__ == "__main__":
    print("=== Автосалон: объекты и наследование ===\n")

    # Объект базового класса
    vehicle = Vehicle("Toyota", "Camry", 2020)
    print("Базовый класс Vehicle:")
    print(vehicle.info())
    print()

    # Объект подкласса Car
    car = Car("Honda", "Civic", 2022, 4, "седан")
    print("Подкласс Car (наследует Vehicle):")
    print(car.info())
    print(f"  Атрибуты Car: марка={car.make}, модель={car.model}, год={car.year}, двери={car.doors}, кузов={car.body_style}")
    print()

    # Объект подкласса Truck
    truck = Truck("Ford", "F-150", 2023, 2.5, 5000)
    print("Подкласс Truck (наследует Vehicle):")
    print(truck.info())
    print(f"  Атрибуты Truck: марка={truck.make}, модель={truck.model}, год={truck.year}, длина кузова={truck.bed_length} м, тяга={truck.towing_capacity} кг")