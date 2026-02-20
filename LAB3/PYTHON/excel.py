import pandas as pd

df = pd.read_excel("lab3.xlsx") #Чтение первого листа
print(df)
print()

df = pd.read_excel("lab3.xlsx", sheet_name="Оценки") #Чтение конкретного листа
print(df)
print()

df = pd.read_excel("lab3.xlsx", sheet_name=["Студенты", "Оценки"]) #Чтение нескольких листов сразу
print()
print(df["Студенты"])
print(df["Оценки"])
print()

df = pd.read_excel("lab3.xlsx", sheet_name="Студенты", skiprows=1) #Пропуск строк (skiprows)
print(df)
print()

df = pd.read_excel("lab3.xlsx", sheet_name="Студенты", usecols=["Имя", "Группа"]) #Чтение только определённых столбцов
print(df)
print()

df = pd.read_excel("lab3.xlsx", sheet_name="Студенты", nrows=2) #Ограничение количества строк
print(df)
print()